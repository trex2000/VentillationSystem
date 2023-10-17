/**
 * @file Humidity_Control.c
 * @author Tudor Coroian (tudor.coroian@altran.com)
 *         Dragos Craciun (dragos.craciun@altran.com)
 * @brief This file contains the implementations of the functions found in the
 * interface. The private functions are used into the exported data reading function,
 * in order to read and filter the data collected by the sensor. The private 
 * functions are helpful in order to obtain the filtered relative humidity.
 * @version 1.2
 * @date 2021-08-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/*============================================================================*/
/*                                 ALTRAN                                     */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*!
 * $Source: Humidity_Control.c $
 * $Revision: 1 $
 * $Author: Tudor Coroian, Dragos Craciun $
 * $Date: 2021.08.11 $
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file
 * This file contains the implementations of the functions found in the
 * interface. The private functions are used into the exported data reading function,
 * in order to read and filter the data collected by the sensor. The private 
 * functions are helpful in order to obtain the filtered relative humidity.    
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
 * $Log: Humidity_Control.c  $
  ============================================================================*/


/* Includes */
/*============================================================================*/
#include "Humidity_Control.h"
#include "MQTT_Interface.h"


/* Constants and types  */
/*============================================================================*/
static const char* TAG = "HUMID";

/* Defines  */
/*============================================================================*/
#define HUMID_i_WINDOW_SIZE (uint32_t)4                            ///< Number of past values kept by the average filter        
#define HUMID_i_MINIMUM_ADMISSIBLE_HUMIDITY (uint32_t)5            ///< Minimum admissible relative humidity value to be returned
#define HUMID_i_MAXIMUM_ADMISSIBLE_HUMIDITY (uint32_t)95           ///< Maximum admissible relative humidity value to be returned
#define HUMID_i_SENSOR_FORMULA_BIT_DIVIDER (uint32_t)65536         ///< This value is 2^16 and it is used for the bit division of the humidity signal
#define HUMID_i_SENSOR_FORMULA_MULTIPLIER (uint32_t)125            ///< Multiplier from the HTU21D formula for relative humidity calculation
#define HUMID_i_SENSOR_FORMULA_MINIMUM_DIGITAL_VALUE (uint32_t)6   ///< Absolute value of the minimum digital humidity output that can be sent out ( its value is -6 )

/* Variables */
/*============================================================================*/
humid_status_t HUMID_t_status;                                     ///< Status of the humidity sensor
i2c_dev_t HUMID_t_device;                                          ///< Device variable

static uint32_t HUMID_i_sum_hum;                               ///< Sum used for the relative humidity average filter
static uint32_t HUMID_i_index_hum;                             ///< Position in the readings of the relative humidity values array
static uint32_t HUMID_i_readings_hum[HUMID_i_WINDOW_SIZE];     ///< Array of size: WINDOW_SIZE used to store past values of the relative humidity
static uint32_t HUMID_i_average_hum_value;                     ///< Variable used to store the filtered values
static uint32_t HUMID_i_prev_different_value;                  ///< Variable used to store a new and different value for the relative humidity


/* Private functions prototypes */
/*============================================================================*/

static uint32_t HUMID_i_Read();
static void HUMID_v_Filter();
static uint32_t HUMID_i_filter(uint32_t i_hum);



/* Inline functions */
/*============================================================================*/


/* Private functions */
/*============================================================================*/
/**
 * @brief 
 * Helping function which returns the initial relative humidity value in order to be filtered 
 * @return uint32_t 
 */
uint32_t HUMID_i_Read()
{

    uint32_t i_raw_humidity = read_value(TRIGGER_HUMD_MEASURE_NOHOLD);
   
    /* Return the relative humidity by using the HTU21D datasheet formula. */
    return ((i_raw_humidity * HUMID_i_SENSOR_FORMULA_MULTIPLIER) / HUMID_i_SENSOR_FORMULA_BIT_DIVIDER) - HUMID_i_SENSOR_FORMULA_MINIMUM_DIGITAL_VALUE;
}

/**
 * 
 @brief Average filter applied for the relative humidity values.
*/
void HUMID_v_Filter(void)
{
  HUMID_i_sum_hum = HUMID_i_sum_hum - HUMID_i_readings_hum[HUMID_i_index_hum];   // Remove the oldest entry from the sum.
  uint32_t HUMID_i_value = HUMID_i_Read();                                       // Save the read relative humidity to a separate variable.
  HUMID_i_readings_hum[HUMID_i_index_hum] = HUMID_i_value;                       // Add the newest reading to the window.
  HUMID_i_sum_hum = HUMID_i_sum_hum + HUMID_i_value;                             // Add the newest reading to the sum.
  HUMID_i_index_hum = (HUMID_i_index_hum + 1) % HUMID_i_WINDOW_SIZE;             // Increment the index, and wrap to 0 if it exceeds the window's size.
  HUMID_i_average_hum_value = HUMID_i_sum_hum / HUMID_i_WINDOW_SIZE;             // Divide the sum of the window by the window size for the result.
}

/**
 * @brief Clip the given value into the [5,95] % interval.
 *        Returns the given value in case of success.
 *        Returns HUMID_i_ERR_INTERVAL if the value is outside the admissible interval.
 * 
 * @param i_hum The value to be filtered
 * @return uint32_t  
 */
uint32_t HUMID_i_filter(uint32_t i_hum)
{
    
    /* Verify if the relative humidity is in the interval specified by the requirements. */
    if(i_hum < HUMID_i_MINIMUM_ADMISSIBLE_HUMIDITY || i_hum > HUMID_i_MAXIMUM_ADMISSIBLE_HUMIDITY)
        return HUMID_i_ERR_INTERVAL; 

    return i_hum;
}




/* Exported functions */
/*============================================================================*/
/**
 * @brief Initialization function for the humidity module
 *        Returns 
 *          - HUMID_i_OK in case of success
 *          - HUMID_i_ERR_INIT in case of error
 * 
 * @return humid_err_t 
 */
humid_err_t HUMID_t_init() 
{
    HUMID_i_sum_hum = 0;
    HUMID_i_index_hum = 0; 
    HUMID_i_average_hum_value = 0;  
    HUMID_i_prev_different_value = 0;
    
    /* Set the I2C port to be used */
    i2c_port_t t_port = HUMID_i_MASTER_NUM;

    /* Initialize  the I2C driver */
    int i_err_init = htu21d_init(
                            t_port,
                            HUMID_t_MASTER_SDA,     /* Master SDA GPIO pin */
                            HUMID_t_MASTER_SCL,     /* Master SCL GPIO pin */
                            GPIO_PULLUP_DISABLE,    /* Disable pullup resistor for SDA */
                            GPIO_PULLUP_DISABLE     /* Disable pullup resistor for SCL */
    );

    /* Check for error in the initializer function. */
    if(i_err_init != HTU21D_ERR_OK) 
    {
        #if HUMID_DEBUG == 1
            ESP_LOGI(TAG, "_init_\tError initializing humidity module. Error code: %#02X", i_err_init);
        #endif
        return HUMID_i_ERR_INIT;
    } 
    else 
    {
        /* Empty */
    }

    /* Perform a soft reset (get rid of previous parasite values). */
    htu21d_soft_reset();
    return HUMID_i_OK;
}

/**
 * @brief Check the status of the humidity module.
 *        Returns 
 *          - HUMID_i_OK in case the module was initialized
 *          - HUMID_i_ERR_NOT_INITIALIZED if the module was not initialized
 * 
 * @return humid_err_t 
 */
humid_err_t HUMID_t_checkStatus() 
{
    /* Check the status of the humidity module. */
    if (HUMID_t_status == HUMID_i_NOT_INITIALIZED) 
    {
        #if HUMID_DEBUG == 1
            ESP_LOGD(TAG, "_STAT_\t Humidity module was not initialized.");
        #endif
        return HUMID_i_ERR_NOT_INITIALIZED;
    } 
    else 
    {
        /* Empty */
    }

    return HUMID_i_OK;
}

/**
 * @brief Return every new value of the relative humidity during the sensor's measurement in order to be used by the Comfort Manager
 * 
 * @return uint32_t 
 */
uint32_t HUMID_i_return_different_humidity(){
    return HUMID_i_prev_different_value;
}


/**
 * @brief Read and filter the relative humidity values.
 *        Returns the read value in case of success.
 *        Returns HUMID_i_ERR_READ if the sensor could not read any relevant data.
 *        Each new relative humidity value will be sent to the MQTT module
 * 
 * @return uint32_t 
 */
uint32_t HUMID_i_readHumidity() 
{
    
    /* Get the relative humidity using the sensor and filter it. */
	HUMID_v_Filter();
    /* Verify if the current relative humidity is in the specified interval. */
    uint32_t i_hum = HUMID_i_filter(HUMID_i_average_hum_value);
    
    /* Return the HUMID_i_ERR_READ error if the sensor could not read any relevant data.*/
    if(i_hum == 0) 
    {
        #ifdef HUMID_DEBUG 
        if ( HUMID_DEBUG == 1 )
            ESP_LOGI(TAG, "_READH_\tHumidity reading is invalid.");
        #endif
        return HUMID_i_ERR_READ;
    } 
    /* Send the relative humidity value to MQTT only if it's different from the previous one. */
    else 
    {
        if ( HUMID_i_prev_different_value != i_hum )
        {
        HUMID_i_prev_different_value = i_hum;
        MQTT_v_SendUpdates(MQTT_i_SEND_HUMIDITY, i_hum);
        }
    }
	
	/* Return the real value, formula in datasheet. */
	
    #ifdef HUMID_DEBUG 
    if( HUMID_DEBUG == 1 )
        ESP_LOGI(TAG, "_READH_\tHumidity: %d", i_hum);
    #endif
    return i_hum;
}


