/**
 * @file FAN_Control.c
 * @author Durus Mihai-Valer (mihai.durus@altran.com)
 * @brief 
 * @version 1.0
 * @date 2021-08-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */
/*============================================================================*/
/*============================================================================*/
/*                                ALTRAN                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*!
 * $Source: FAN_Control.c $
 * $Revision: 1 $
 * $Author: Durus Mihai-Valer $
 * $Date: 2021.08.06 $
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file
    short description in one sentence end with dot.
    detailed
    multiline
    description of the file
*/
/*============================================================================*/
/*                    REUSE HISTORY - taken over from                         */
/*============================================================================*/
/*  DATABASE           |        PROJECT     | FILE VERSION (AND INSTANCE)     */
/*----------------------------------------------------------------------------*/
/*                     |                    |                                 */
/*============================================================================*/
/*                               OBJECT HISTORY                               */
/*============================================================================*/
/*
 * $Log: FAN_Control.c  $
  ============================================================================*/

/* Includes */
/*============================================================================*/

#include "FAN_Config.h"
#include "FAN_Control.h"
#include "FAN_Interface.h"
#include "GPIO_config.h"
#include "NVM_Interface.h"
#include "GPIO_Interface.h"

/* Constants and types  */
/*============================================================================*/

static const char *TAG = "FAN"; ///< Tag used for the ESP LOG DEBUG
#define FAN_i_WINDOW_SIZE 4     ///< Represents the number of past values used by the average filter

/* Variables */
/*============================================================================*/

static int FAN_i_status_ItO; ///< Hold the current status of the fan inside->outside initialization
static int FAN_i_status_OtI; ///< Hold the current status of the fan outside->inside initialization

static uint16_t FAN_uw_sum_ItO;                         ///< Sum used for the average filter
static uint16_t FAN_uw_index_ItO;                       ///< Position in the readings of the pulse counter values array
static uint16_t FAN_uw_readings_ItO[FAN_i_WINDOW_SIZE]; ///< Array of size: FAN_i_WINDOW_SIZE used to store past values of the pulse counter

static uint16_t FAN_uw_sum_OtI;                         ///< Sum used for the average filter
static uint16_t FAN_uw_index_OtI;                       ///< Position in the readings of the pulse counter values array
static uint16_t FAN_uw_readings_OtI[FAN_i_WINDOW_SIZE]; ///< Array of size: FAN_i_WINDOW_SIZE used to store past values of the pulse counter

static int16_t FAN_sw_frequencyValue_ItO;               ///< Varibales used to store the running frequnecy values of the two fans
static int16_t FAN_sw_frequencyValue_OtI;

static uint16_t FAN_uw_rpmValue_ItO;                    ///< Varibales used to store the rotations per minute of the two fans
static uint16_t FAN_uw_rpmValue_OtI;

static uint16_t FAN_uw_averagedFrequencyValue_ItO;      ///< Variables used to store the filtered values of Fan's frequency
static uint16_t FAN_uw_averagedFrequencyValue_OtI;

uint16_t FAN_uw_rpmValue_calibrated_ItO;                ///< Variables used to store the filtered and calibrated rotations per minute of the Fans
uint16_t FAN_uw_rpmValue_calibrated_OtI;

/* Private functions prototypes */
/*============================================================================*/

/* Inline functions */
/*============================================================================*/

/* Private functions */
/*============================================================================*/

/* Exported functions */
/*============================================================================*/

/**
 * @brief Function used to config MCPWM unit and pins for PWM
 * 
 */
void FAN_v_configMCPWM(void)
{
  mcpwm_set_pin(MCPWM_UNIT_0, &pin_config);
}

/**
 * @brief Function used to call nvm set function slope and offset
 * 
 */

/* Initialization for fan inside->outside */
/*============================================================================*/

/**
 * 
 * @brief Functions for In to Out
 *  Initialization of fan inside->outside mcpwm initial parameters 
 *  and setting up the pulse counter module for speed monitoring
 * @return fan_err_t 
 */
fan_err_t FAN_t_init_ItO(void)
{
  /* Variable initialization */
  FAN_uw_sum_ItO = FAN_i_ZERO_VAL;
  FAN_uw_index_ItO = FAN_i_ZERO_VAL;
  FAN_sw_frequencyValue_ItO = FAN_i_ZERO_FRQ;
  FAN_uw_rpmValue_ItO = FAN_i_ZERO_RPM;
  FAN_uw_averagedFrequencyValue_ItO = FAN_i_ZERO_FRQ;
  FAN_uw_rpmValue_calibrated_ItO = FAN_i_ZERO_RPM;

  /* Set motor on-off pin to low (off) */

  /* Initialize MCPWM configuration */
  pwm_config.frequency = FAN_i_PWM_FREQUENCY;           // Frequency = 25000Hz, value from Fan datasheet
  pwm_config.cmpr_a = FAN_i_PWM_INIT_DUTY_ItO;          // Duty cycle of PWM1xA
  pwm_config.counter_mode = MCPWM_UP_COUNTER;           // For asymmetric MCPWM
  pwm_config.duty_mode = MCPWM_DUTY_MODE_0;             // Active high duty, i.e. duty cycle proportional to high time for asymmetric MCPWM
  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config); // Configure PWM0A with above settings

  /* Initialize PCNT configuration */
  pcnt_config_i2o.pulse_gpio_num = SIGNAL_FAN_ItO; // input signal for the pulse counter
  pcnt_config_i2o.ctrl_gpio_num = SIGNAL_CTRL_ItO; // control signal for the pulse counter, HIGH=count up, LOW=count down
  pcnt_config_i2o.channel = PCNT_CHANNEL_0;
  pcnt_config_i2o.unit = PCNT_UNIT_0;
  pcnt_config_i2o.pos_mode = PCNT_COUNT_INC;            // Count up on the positive edge
  pcnt_config_i2o.neg_mode = PCNT_COUNT_DIS;            // Keep the counter value on the negative edge
  pcnt_config_i2o.lctrl_mode = PCNT_MODE_KEEP;          // Keep the primary counter mode if low
  pcnt_config_i2o.hctrl_mode = PCNT_MODE_KEEP;          // Keep the primary counter mode if high
  pcnt_config_i2o.counter_h_lim = FAN_i_PCNT_H_LIM_VAL; // Set the maximum limit value to watch
  pcnt_config_i2o.counter_l_lim = FAN_i_PCNT_L_LIM_VAL; // Set the minimum limit value to watch

  /* Initialize PCNT's counter */
  esp_err_t err_pcnt_config = pcnt_unit_config(&pcnt_config_i2o);
  esp_err_t err_pcnt_pause = pcnt_counter_pause(PCNT_UNIT_0);
  esp_err_t err_pcnt_clear = pcnt_counter_clear(PCNT_UNIT_0);

  /* Everything is set up, now go to counting */
  pcnt_counter_resume(PCNT_UNIT_0);
  if (err_pcnt_config == ESP_ERR_INVALID_STATE || err_pcnt_pause == ESP_ERR_INVALID_STATE || err_pcnt_clear == ESP_ERR_INVALID_STATE)
  {
    FAN_i_status_ItO = FAN_i_NOT_INIT_ItO; // Set fan status to NOT INIT
    return FAN_i_ERR_INIT;                 // Return error code
  }
  else
  {
    FAN_i_status_ItO = FAN_i_INIT_ItO; // Set fan status to INIT
    return FAN_i_OK_ItO;               // Return succes code
  }
}

/* Initialization for fan outside->inside */
/*============================================================================*/

/**
 * @brief Function designed to initialize all the necessary components for the fan control.
 *        MCPWM initial parameters configuration (duty cycle, frequency etc.).
 *        PCNT module parameters configuration and pin assingment (control, sginal),
 *        start of the pulse counter
 * @return fan_err_t 
 */
fan_err_t FAN_t_init_OtI(void)
{
  /* Variable initialization */
  FAN_uw_sum_OtI = FAN_i_ZERO_VAL;
  FAN_uw_index_OtI = FAN_i_ZERO_VAL;
  FAN_sw_frequencyValue_OtI = FAN_i_ZERO_FRQ;
  FAN_uw_rpmValue_OtI = FAN_i_ZERO_RPM;
  FAN_uw_averagedFrequencyValue_OtI = FAN_i_ZERO_FRQ;
  FAN_uw_rpmValue_calibrated_OtI = FAN_i_ZERO_RPM;

  /* Set motor on-off pin to low (off) */
 
  // Initialize MCPWM configuration
  pwm_config.frequency = FAN_i_PWM_FREQUENCY;           // Frequency = 25000Hz, value from Fan datasheet (300Hz for testing purposes)
  pwm_config.cmpr_a = FAN_i_PWM_INIT_DUTY_OtI;          // Duty cycle of PWM0xA
  pwm_config.counter_mode = MCPWM_UP_COUNTER;           // For asymmetric MCPWM
  pwm_config.duty_mode = MCPWM_DUTY_MODE_0;             // Active high duty, i.e. duty cycle proportional to high time for asymmetric MCPWM
  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_1, &pwm_config); // Configure PWM0A with above settings

  // Initialize PCNT configuration
  pcnt_config_o2i.pulse_gpio_num = SIGNAL_FAN_OtI;      // input signal for the pulse counter
  pcnt_config_o2i.ctrl_gpio_num = SIGNAL_CTRL_OtI;      // control signal for the pulse counter, HIGH=count up, LOW=count down
  pcnt_config_o2i.channel = PCNT_CHANNEL_0;             // Assign PCNT channel
  pcnt_config_o2i.unit = PCNT_UNIT_1;                   // Assign PCNT unit
  pcnt_config_o2i.pos_mode = PCNT_COUNT_INC;            // Count up on the positive edge
  pcnt_config_o2i.neg_mode = PCNT_COUNT_DIS;            // Keep the counter value on the negative edge
  pcnt_config_o2i.lctrl_mode = PCNT_MODE_KEEP;          // Keep the primary counter mode if low
  pcnt_config_o2i.hctrl_mode = PCNT_MODE_KEEP;          // Keep the primary counter mode if high
  pcnt_config_o2i.counter_h_lim = FAN_i_PCNT_H_LIM_VAL; // Set the maximum limit value to watch
  pcnt_config_o2i.counter_l_lim = FAN_i_PCNT_L_LIM_VAL; // Set the minimum limit value to watch

  /* Initialize PCNT's counter */
  esp_err_t err_pcnt_config = pcnt_unit_config(&pcnt_config_o2i);
  esp_err_t err_pcnt_pause = pcnt_counter_pause(PCNT_UNIT_1);
  esp_err_t err_pcnt_clear = pcnt_counter_clear(PCNT_UNIT_1);

  /* Everything is set up, now go to counting */
  pcnt_counter_resume(PCNT_UNIT_1);
  /* Check for errors in the PCNT configuration and startup */
  if (err_pcnt_config == ESP_ERR_INVALID_STATE || err_pcnt_pause == ESP_ERR_INVALID_STATE || err_pcnt_clear == ESP_ERR_INVALID_STATE)
  {
    FAN_i_status_OtI = FAN_i_NOT_INIT_OtI; // Set fan status to NOT INIT
    return FAN_i_ERR_INIT;                 // Return error code
  }
  else
  {
    FAN_i_status_OtI = FAN_i_INIT_OtI; // Set fan status to INIT
    return FAN_i_OK_OtI;               // Return succes code
  }
}

/* Functions used for speed getting/setting */
/*============================================================================*/

/**
 * @brief 
 * Function used to set fan's duty cycle to a desired value
 * watchout the duty is reversed when applied to MCPWM module,
 * that's why we use FAN_i_REVERSE_PWM - value in the code
 * @param uw_dutyValue given as an int to avoid working with double
 * @param fan_direction 0 : outside->inside | 1 : inside->outside
 * @return fan_err_t 
 */
fan_err_t FAN_t_setSpeed(uint16_t uw_dutyValue, uint8_t uw_fan_direction)
{
  /* Check for fan outside->inside */
  if (FAN_i_status_OtI == FAN_i_INIT_OtI)
  {
    if (uw_fan_direction == FAN_i_DIRECTION_OUT)
    {
      /* Set duty cycle of PWM1xA to uw_dutyValue, which has to be reversed and converted to double */
      mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_A, FAN_i_REVERSE_PWM - uw_dutyValue);
      /* Set duty cycle of PWM0xA to ZERO */
      mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, FAN_i_REVERSE_PWM);
      /* Set motor on-off pin to high (on) */
    }
    else
    {
      //empty
    }
  }
  else
  {
    return FAN_i_NOT_INIT_OtI;
  }

  if (FAN_i_status_ItO == FAN_i_INIT_ItO)
  {
    /* Check for fan inside->outside */
    if (uw_fan_direction == FAN_i_DIRECTION_IN && FAN_i_status_ItO == FAN_i_INIT_ItO)
    {
      /* Set duty cycle of PWM0xA to uw_dutyValue, which has to be reversed and converted to double */
      mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, FAN_i_REVERSE_PWM - uw_dutyValue);
      /* Set duty cycle of PWM1xA to ZERO */
      mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_A, FAN_i_REVERSE_PWM);
      /* Set motor on-off pin to high (on) */
    }
    else
    {
      // empty
    }
  }
  else
  {
    return FAN_i_NOT_INIT_ItO;
  }
  return FAN_i_SET_SPEED_OK;
}

/**
 * @brief The fan's speed returned in frequnecy [Hz]
 * @param uw_fan_direction 0 : outside->inside | 1 : inside->outside
 * @return uint16_t 
 */
uint16_t FAN_uw_getSpeed(uint8_t uw_fan_direction)
{
  /* Verify fan direction */
  if (uw_fan_direction == FAN_i_DIRECTION_IN)
  {
    return FAN_i_REVERSE_PWM - (int)mcpwm_get_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A); // Get duty cycle of PWM0xA
  }
  else
  {
    return FAN_i_REVERSE_PWM - (int)mcpwm_get_duty(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_A); // Get duty cycle of PWM1xA
  }
}

/* Functions for speed filtration, calibration and monitorization of both fans */
/*============================================================================*/

/**
 * @brief  Reads the values from the counters and updated the validity flag.
 * @return fan_err_t 
 */
fan_err_t FAN_v_updateSpeedValues(void)
{
  if (FAN_i_status_ItO == FAN_i_NOT_INIT_ItO)
  {
    return FAN_i_NOT_INIT_ItO;
  }

  if (FAN_i_status_OtI == FAN_i_NOT_INIT_OtI)
  {
    return FAN_i_NOT_INIT_OtI;
  }

  pcnt_counter_pause(PCNT_UNIT_0);                                          // pause counter
  pcnt_get_counter_value(pcnt_config_i2o.unit, &FAN_sw_frequencyValue_ItO); // get counter value
  pcnt_counter_clear(PCNT_UNIT_0);                                          // clear counter
  pcnt_counter_pause(PCNT_UNIT_1);
  pcnt_get_counter_value(pcnt_config_o2i.unit, &FAN_sw_frequencyValue_OtI);
  pcnt_counter_clear(PCNT_UNIT_1);

  pcnt_counter_resume(PCNT_UNIT_0); // resume counter
  pcnt_counter_resume(PCNT_UNIT_1);

  return FAN_i_UPDATE_SPEED_OK;
}

/**
 * @brief Moving average filter used to filter pulses from the pulse counter.
 *        A moving average filter is a basic technique that can be used to remove
 *        noise (random interference) from a signal. It is a simplified form of a
 *        low-pass filter for fan inside->outside.
 */
void frequencyFiltering_ItO()
{
  FAN_uw_sum_ItO = FAN_uw_sum_ItO - FAN_uw_readings_ItO[FAN_uw_index_ItO];                     // Remove the oldest entry from the sum
  FAN_uw_readings_ItO[FAN_uw_index_ItO] = FAN_sw_frequencyValue_ItO;                           // Add the newest reading to the window
  FAN_uw_sum_ItO = FAN_uw_sum_ItO + FAN_sw_frequencyValue_ItO;                                 // Add the newest reading to the sum
  FAN_uw_index_ItO = (FAN_uw_index_ItO + 1) % FAN_i_WINDOW_SIZE;                               // Increment the index, and wrap to 0 if it exceeds the window size
  FAN_uw_averagedFrequencyValue_ItO = FAN_uw_sum_ItO / FAN_i_WINDOW_SIZE;                      // Divide the sum of the window by the window size for the result
  FAN_uw_rpmValue_ItO = FAN_uw_averagedFrequencyValue_ItO * FAN_i_1MIN_2_SEC / FAN_i_2_PULSES; // Transform filtered frequency value into RPM
}

/**
 * @brief Moving average filter used to filter pulses from the pulse counter.
 *        A moving average filter is a basic technique that can be used to remove
 *        noise (random interference) from a signal. It is a simplified form of a
 *        low-pass filter for fan outside->inside.
 */
void frequencyFiltering_OtI(void)
{
  FAN_uw_sum_OtI = FAN_uw_sum_OtI - FAN_uw_readings_OtI[FAN_uw_index_OtI];                     // Remove the oldest entry from the sum
  FAN_uw_readings_OtI[FAN_uw_index_OtI] = FAN_sw_frequencyValue_OtI;                           // Add the newest reading to the window
  FAN_uw_sum_OtI = FAN_uw_sum_OtI + FAN_sw_frequencyValue_OtI;                                 // Add the newest reading to the sum
  FAN_uw_index_OtI = (FAN_uw_index_OtI + 1) % FAN_i_WINDOW_SIZE;                               // Increment the index, and wrap to 0 if it exceeds the window size
  FAN_uw_averagedFrequencyValue_OtI = FAN_uw_sum_OtI / FAN_i_WINDOW_SIZE;                      // Divide the sum of the window by the window size for the result
  FAN_uw_rpmValue_OtI = FAN_uw_averagedFrequencyValue_OtI * FAN_i_1MIN_2_SEC / FAN_i_2_PULSES; // Transform filtered frequency value into RPM
}

/**
 * @brief Function which calculates line equation which calibrates
 *        speed data vs duty cycle according to the datasheet for
 *        fan inside->outside
 * @return uint16_t 
 */
uint16_t FAN_uw_calibratedRPM_ItO(void)
{
  /* Get fan duty cycle */
  uint16_t duty = FAN_uw_getSpeed(FAN_i_DIRECTION_IN);
  /* In case fan inside->outside not connected check for zero RPM to not show the value of the RPM because duty is set in the init */
  if (FAN_uw_rpmValue_ItO == FAN_i_ZERO_RPM)
  {
    return FAN_i_ZERO_RPM; // Set calibrated RPM value to 0
  }
  /* If fan is running at 20% duty or lower display minimum calibrated RPM */
  if (duty <= FAN_i_DUTY_MIN)
  {
    return FAN_i_RPM_CALIBRATED_MIN;
  }

  /* If connected set calibrated value using line equation f(x)=mx+b where: f(x)=RPM; m=line slope; b=offset coefficient */
  return (int)(FAN_lf_DUTY_COEFF * duty + FAN_lf_OFFSET_RPM);
  
}

/**
 * @brief Function which calculates line equation which calibrates
 *        speed data vs duty cycle according to the datasheet for
 *        fan outside->inside
 * @return uint16_t 
 */
uint16_t FAN_uw_calibratedRPM_OtI(void)
{
  /* Get fan duty cycle */
  uint16_t duty = FAN_uw_getSpeed(FAN_i_DIRECTION_OUT);
  /* In case fan outside->inside not connected check for zero RPM to not show the value of the RPM because duty is set in the init */
  if (FAN_uw_rpmValue_OtI == FAN_i_ZERO_RPM)
  {
    return FAN_i_ZERO_RPM; // Set calibrated RPM value to 0
  }
  /* If fan is running at 20% duty or lower display minimum calibrated RPM */
  if (duty <= FAN_i_DUTY_MIN)
  {
    return FAN_i_RPM_CALIBRATED_MIN;
  }
  
  /* If connected set calibrated value using line equation f(x)=mx+b where: f(x)=RPM; m=line slope; b=offset coefficient */
  return (int)(FAN_lf_DUTY_COEFF * duty + FAN_lf_OFFSET_RPM);

}

/**
 * @brief Function which calculates difference between between the actual speed
 *        read by tachometer pin and the theoretical one from datasheet for fan
 *        inside->outside
 * @return fan_err_t 
 */
fan_err_t FAN_t_detectSpeedError_ItO(void)
{
  /* Check speed error to be lower than FAN_i_MAX_SPEED_ERROR */
  if(FAN_uw_rpmValue_ItO-FAN_uw_rpmValue_calibrated_ItO>=FAN_i_MAX_SPEED_ERROR)
  {
    return FAN_i_ERR_SPEED_ItO;     // Send error code
  }
  else
  {
    return FAN_i_SPEED_OK_ItO;      // Send succes code
  }
    
}

/**
 * @brief Function which calculates difference between between the actual speed
 *        read by tachometer pin and the theoretical one from datasheet for fan
 *        outside->inside
 * @return fan_err_t 
 */
fan_err_t FAN_t_detectSpeedError_OtI(void)
{
  /* Check speed error to be lower than FAN_i_MAX_SPEED_ERROR */
  if(FAN_uw_rpmValue_OtI-FAN_uw_rpmValue_calibrated_OtI>=FAN_i_MAX_SPEED_ERROR)
  {
    return FAN_i_ERR_SPEED_OtI;     // Send error code    
  }
  else
  {
    return FAN_i_SPEED_OK_OtI;      // Send succes code
  }
    
}

/**
 * @brief Sends filtered speed data through the built in  
 *        ESP logging debug system to the serial monitor
 */
void FAN_v_speedLogging(void)
{

  FAN_uw_rpmValue_calibrated_ItO = FAN_uw_calibratedRPM_ItO();
  FAN_uw_rpmValue_calibrated_OtI = FAN_uw_calibratedRPM_OtI();
  /*  Undefine or comment FAN_SPEED_DEBUG_RPM,
    FAN_SPEED_DEBUG_RPM_CALIBRATED, FAN_SPEED_DEBUG_FRQ
    to stop debugging of speed */

#ifdef FAN_SPEED_DEBUG_RPM
  if (FAN_SPEED_DEBUG_RPM == 1)
  {
    ESP_LOGI(TAG, "I2O=");
    ESP_LOGI(TAG, "%d", FAN_uw_rpmValue_ItO);
    ESP_LOGI(TAG, " rpm\tO2I = ");
    ESP_LOGI(TAG, "%d", FAN_uw_rpmValue_OtI);
    ESP_LOGI(TAG, "\n");
    ESP_LOGI(TAG, "rpm");
  }
#endif

#ifdef FAN_SPEED_DEBUG_RPM_CALIBRATED
  if (FAN_SPEED_DEBUG_RPM_CALIBRATED == 1)
  {
    ESP_LOGI(TAG, "I2O=");
    ESP_LOGI(TAG, "%d", FAN_uw_rpmValue_calibrated_ItO);
    ESP_LOGI(TAG, " rpm\tO2I = ");
    ESP_LOGI(TAG, "%d", FAN_uw_rpmValue_calibrated_OtI);
    ESP_LOGI(TAG, "\n");
    ESP_LOGI(TAG, "rpm");
  }
#endif

#ifdef FAN_SPEED_DEBUG_FRQ
  if (FAN_SPEED_DEBUG_FRQ == 1)
  {
    ESP_LOGI(TAG, "I2O=");
    ESP_LOGI(TAG, "%d", FAN_uw_averagedFrequencyValue_ItO);
    ESP_LOGI(TAG, " Hz\tO2I = ");
    ESP_LOGI(TAG, "%d", FAN_uw_averagedFrequencyValue_OtI);
    ESP_LOGI(TAG, "\n");
    ESP_LOGI(TAG, "Hz");
  }
#endif
}
