/**
 * @file Comfort.c
 * @author Durus Mihai-Valer + Baleanu Sorina :) (mihai.durus@altran.com)
 * @brief 
 * @version 1.0
 * @date 2021-08-06
 * @copyright Copyright (c) 2021
 * 
 */

/*============================================================================*/
/*                                 ALTRAN                                     */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*!
 * $Source: Comfort.c $
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
 * $Log: Comfort.c  $
  ============================================================================*/

/* Includes */
/*============================================================================*/

#include "Comfort_Interface.h"
#include "Comfort_Config.h"
#include "FAN_Interface.h"
#include "Temperature_Interface.h"
#include "Humidity_Interface.h"
#include <esp_log.h>
#include <string.h>
#include "NVM_Interface.h"

/* Constants and types  */
/*============================================================================*/
/* States in non-recycling mode state machine. */
typedef enum
{
  COMF_i_WAIT_NONREC = 0,   ///< Value for the wait state in non-recylicling mode state machine.
  COMF_i_COMPUTATIONS,     ///< Value for the computations state in non-recylicling mode state machine.
  COMF_i_MODIFY_SPEED,    ///< Value for the modify speed state in non-recylicling mode state machine.

} COMF_nonRec_state_t;

/* States in recycling mode state machine. */
typedef enum
{
  COMF_i_WAIT_REC = 0,    ///< Value for the wait state in recylicling mode state machine.
  COMF_i_SET_DIRECTION,  ///< Value for the set direction state ion recycling mode state machine.
  COMF_i_SET_SPEED,     ///< Value for the computations state in recylicling mode state machine.
  COMF_i_COUNT,        ///< Value for count state in recycling mode state machine.

} comf_rec_state_t;

/* Counter stop values. */
typedef enum
{
  COMF_i_COUNTER_1_MIN = COMF_i_1MIN, ///< Value for one minute in seconds.
  COMF_i_COUNTER_2_MIN = COMF_i_2MIN, ///< Value for two minutes in seconds.
  COMF_i_COUNTER_3_MIN = COMF_i_3MIN,   ///<Value for three minutes in seconds.

} comf_counter_minutes_t;

/* TAG for ESP logs. */
static const char *TAG = "COMF";

/* Variables */
/*============================================================================*/
/*  The current mode set on the system. */
static comf_mode_t COMF_t_mode;

/* The current speed set on the system.  */
static int16_t COMF_t_speed;

/* The current fan that is operating. */
static int8_t COMF_sw_fan_direction;

/* Variables for non recycling mode without heat recovery*/
/*============================================================================*/

/* Holds the current inside temperature when non-recyling mode is activated. */
static int16_t COMF_sw_nonRec_insideTemp;

/* Holds the goal temperature for the non-recycling mode. */
static int16_t COMF_sw_nonRec_tempRef;

/* Holds the difference between the reference temperature and temperature sent from MQTT. */
static int16_t COMF_sw_nonRec_tempDiff;

/* Holds the slope value in the straight line ecuation defined in the non-recycling state machine. */
static int16_t COMF_sw_nonRec_slope;

/* Holds the offset value in the straight line ecuation defined in the non-recycling state machine. */
static int16_t COMF_sw_nonRec_offsetCoeff;

/* Holds the current state for the non-recycling state machine*/
static COMF_nonRec_state_t COMF_t_nonRec_currentState;


/* Variables for recycling mode */
/*============================================================================*/

/* Holds the current temperature sent from MQTT when non-recyling mode is activated. */
static int16_t COMF_sw_rec_tempMqttInside;

/* Holds the current temperature sent from MQTT when non-recyling mode is activated. */
static int16_t COMF_sw_rec_tempMqttOutside;

/* Holds the current state for the recycling state machine. */
static comf_rec_state_t COMF_t_rec_currentState;

/* Holds the current count for passed seconds in reycling mode. */
static uint16_t COMF_sw_rec_counter;

/* Holds the number of seconds at which the state machine changes states. */
static comf_counter_minutes_t COMF_t_rec_stop_counter;


/* Private functions prototypes */
/*============================================================================*/

/* Inline functions */
/*============================================================================*/

/* Private functions */
/*============================================================================*/

/* Exported functions */
/*============================================================================*/

/**
 * @brief Function which initializes: fan speed, fan working mode and
 *        the counter used in the recycling mode
 */
void COMF_v_Init(void)
{
  
  /* Get fan mode from non-volatile memory */
  NVM_t_getFanMode(&COMF_t_mode);

  /* Set the retrieved mode from the memory */
  COMF_v_setMode(COMF_t_mode);

  /* Get fan speed from non-volatile memory */
  NVM_t_getFanSpeed(&COMF_t_speed);

  /* Set the retrieved speed from the memory */
  COMF_v_setSpeed(COMF_t_speed);

  /* Initialize the counter used in recycling mode with 0. */
  COMF_sw_rec_counter = 0;

}

/**
 * @brief The function implements the state machine for recycling operating mode.Recycling mode has adjustable
 *  speed. In this mode both fans are working alternatively and switching relative to speed. For LOW speed 
 *  preset the fans will switch after the inside and outside temperature get to the same value or after a fixed
 *  period of time: 3 minutes, for MEDIUM speed fans will switch after 2 minutes and for HIGH speed fans will switch after 1 minute.
 */
void COMF_v_recyclingMode(void)
{

  switch (COMF_t_rec_currentState)
  {
    /* Wait state */
    case COMF_i_WAIT_REC:
    {
      /* Wait for user/MQTT command to go in the next state */
      #if COMF_DEBUG_FLAG == 1
        ESP_LOGI(TAG, "Wait recycling mode.....");
      #endif
      
      break;
    }
    case COMF_i_SET_DIRECTION:
    {
      /* Get the current temperature from inside temperature sensor */
      COMF_sw_rec_tempMqttInside = Temperature_i_getInterior();
      
      /* Get the current temperature from outside temperature sensor */
      COMF_sw_rec_tempMqttOutside = Temperature_i_getExterior();

      #if COMF_DEBUG_FLAG == 1
        ESP_LOGI(TAG, "Temperature inside: %d , temperature outside: %d", COMF_sw_rec_tempMqttInside, COMF_sw_rec_tempMqttOutside);
      #endif

      /* Set the variable used to count when the states should change to 0*/
      COMF_sw_rec_counter = 0;

      /* If the temperature outside is greater than inside temperature the fan inside should start.
        The state machine will switch to SET SPEED ITO. */
      COMF_t_rec_currentState = COMF_i_SET_SPEED;

      /* Check where is the greater temperature */
      if(COMF_sw_rec_tempMqttInside > COMF_sw_rec_tempMqttOutside)
      {
        /* If the temperature inside is greater than outside temperature the outside fan should start.
         The state will change to SET SPEED on the outside fan*/

        COMF_sw_fan_direction = FAN_i_DIRECTION_OUT;

        #if COMF_DEBUG_FLAG == 1
          ESP_LOGI(TAG, "Change the state to SPEED OTI");
        #endif
      }
      else
      {
        /* If the temperature outside is greater than inside temperature the inside fan should start.
        The state will change to SET SPEED on the inside fan. */
        COMF_sw_fan_direction = FAN_i_DIRECTION_IN;

        #if COMF_DEBUG_FLAG == 1
          ESP_LOGI(TAG, "Change the state to SPEED ITO");
        #endif
      }

      break;
    }
    case COMF_i_SET_SPEED:
    {
      /* Set the fan speed of the inside fan with the recieved preset*/
      FAN_t_setSpeed(COMF_t_speed, COMF_sw_fan_direction);

      /* Change the state to operatre on the inside fan*/
      COMF_t_rec_currentState = COMF_i_COUNT;
     
      break;
    }
    case COMF_i_COUNT:
    {
      /* Counter for how many seconds have passed is incremented.*/
      COMF_sw_rec_counter++;

      #if COMF_DEBUG_FLAG == 1
        ESP_LOGI(TAG, "SPEED ITO COUNTER SEC %d",COMF_sw_rec_counter);
       #endif
      
      /* Check if temperatures are the same, then switch direction, otherwise check if time has passed */
      if(Temperature_i_getInterior() == Temperature_i_getExterior())
      {
        /* Set the speed of the current state to the saved value from the flag */
        COMF_t_rec_currentState = COMF_i_SET_SPEED;
        
        /* Switch fan direction */
        COMF_sw_fan_direction = !COMF_sw_fan_direction;

        /* Reinitialize the counter to 0 */
        COMF_sw_rec_counter = 0;

        #if COMF_DEBUG_FLAG == 1
          ESP_LOGI(TAG, "Change the state to SPEED OTI");
        #endif
      }
      else
      {
        /* Check if the prestablished time interval has passed since the inside fan was set*/
        if(COMF_sw_rec_counter == COMF_t_rec_stop_counter)
        {
          /* Set the speed of the current state to the saved value from the flag */
          COMF_t_rec_currentState = COMF_i_SET_SPEED;
        
          /* Switch fan direction */
          COMF_sw_fan_direction = !COMF_sw_fan_direction;

          /* Reinitialize the counter to 0 */
          COMF_sw_rec_counter = 0;

          #if COMF_DEBUG_FLAG == 1
            ESP_LOGI(TAG, "Change the state to SPEED OTI");
          #endif
        }
        else
        {
          //empty
        }
      }
      break;
    }
  }
}

/**
 * @brief The function implements the state machine for non-recycling operating mode.
 * In this mode only the outside fan will operate. This mode will decrease the speed 
 * linearly using the ecuation of a straight line => rpm = slope * interior_temperature + offset.
 * 
 */
void COMF_v_nonRecyclingMode(void)
{
  switch (COMF_t_nonRec_currentState)
  {
    case COMF_i_WAIT_NONREC:
    {
      /* The state machine will stay in this state until MQTT/user inputs a command. */

      #if COMF_DEBUG_FLAG == 1
        ESP_LOGI(TAG, "Wait non recycling mode.....");
      #endif

      break;
    }
    case COMF_i_COMPUTATIONS:
    {
      /* For decreasing the speed linearly the ecuation of a straight line will be used: 
      * rpm = slope * interior_temperature + offset. */

      /* In this state fan speed is 0 rpm for the inside fan and maximum rpms for outside fan.*/
      FAN_t_setSpeed(COMF_i_HIGH_PRESET, COMF_sw_fan_direction);
      
      /* Set reference temperature to 0 initially, should be a value different from 0 if 
         conditions are eligible for non recycling mode. */
      COMF_sw_nonRec_tempRef=0;

      /* Get the inside temperature at the moment of the  MQTT/user command. */
      COMF_sw_nonRec_insideTemp = Temperature_i_getInterior();

      /* Holds the current outside temperature when non-recyling mode is activated. */
      int16_t COMF_sw_nonRec_outsideTemp = Temperature_i_getExterior();

      if(COMF_sw_nonRec_insideTemp - COMF_sw_nonRec_outsideTemp >= COMF_i_NON_REC_MIN_DIFF)
      {
        /*Set the reference tempreture to be -2 degrees. */
        COMF_sw_nonRec_tempRef = COMF_sw_nonRec_insideTemp - COMF_i_DIFF_TEMP; 
      }
      else if(COMF_sw_nonRec_insideTemp - COMF_sw_nonRec_outsideTemp <= - COMF_i_NON_REC_MIN_DIFF)
      {
        /*Set the reference tempreture to be +2 degrees. */
        COMF_sw_nonRec_tempRef = COMF_sw_nonRec_insideTemp + COMF_i_DIFF_TEMP; 
      }
      else
      {
        // empty
      }

       /* If the reference temperature remains 0, the non recycling mode shall not start */
      if(COMF_sw_nonRec_tempRef != 0)
      {
        /* Compute the difference between the goal temperature and  the current temperature. */
        COMF_sw_nonRec_tempDiff = COMF_sw_nonRec_tempRef - COMF_sw_nonRec_insideTemp;

        /* Compute the slope of the line used to decrease the fan speed lineary.*/
        COMF_sw_nonRec_slope = -COMF_i_MAX_RPM / (COMF_sw_nonRec_tempDiff);

        /* Compute the offset from staight line ecuation. */
        COMF_sw_nonRec_offsetCoeff = COMF_i_MAX_RPM  - COMF_sw_nonRec_slope * COMF_sw_nonRec_insideTemp;

        /* After the computations, state machine will go in the modify speed state.*/
        COMF_t_nonRec_currentState = COMF_i_MODIFY_SPEED;

        #if COMF_DEBUG_FLAG == 1
          ESP_LOGI(TAG, "Calculating equation parameters: DONE !");
        #endif
      }
      else
      {
         #if COMF_DEBUG_FLAG == 1
          ESP_LOGI(TAG, "There is no temperature difference of at least 5 degrees!");
        #endif
      }

      break;
    }
    case COMF_i_MODIFY_SPEED:
    {
      /* Holds the current temperature. */
      int16_t sw_tempInterior;

      /* Holds the computed value for rpm value. */
      int16_t sw_rpmValue;

      /* Holds the computed duty value. */
      int16_t sw_dutyValue;

      /* Get the current interior temperature .*/
      sw_tempInterior = Temperature_i_getInterior();

      /* Check if the difference between reference tempreature and the saved temperature at the start of the mode is positive or negative. */
      if(COMF_sw_nonRec_tempDiff >= COMF_i_ZERO_DEG)
      {
        /* If the difference between reference tempreature and the saved temperature at the start of the mode is positive the state machine will
       go back to wait state only when current temperature is higher or equal to reference temperature. */
        if (COMF_sw_nonRec_tempRef <= sw_tempInterior)
        {
          /* The state machine will change its state to wait state and the fan will stop because the
           reference temperature was reached*/
          COMF_t_nonRec_currentState = COMF_i_WAIT_NONREC;

          #if COMF_DEBUG_FLAG == 1
            ESP_LOGI(TAG, "The reference temperature was hit! Go back to wait state.");
          #endif
        }
        else 
        {
            //empty
        }
      }
      else
      {
        /* If the difference between goal tempreature and temperature sent from MQTT is negative the state machine will
        go back to wait state only when current temperature is lower or equal to goal temperature .*/
        if (COMF_sw_nonRec_tempRef >= sw_tempInterior)
        {
          /* The state machine will change its state to wait state and the fan will stop because the
           goal temperature was reached.
          */
          COMF_t_nonRec_currentState = COMF_i_WAIT_NONREC;

          #if COMF_DEBUG_FLAG == 1
            ESP_LOGI(TAG, "The reference temperature was hit! Go back to wait state.");
          #endif
        }
        else 
        {
            //empty
        }
      }

      /* The current rpm value is computed by replacing current temperature in the straight line ecuation. */
      sw_rpmValue = COMF_sw_nonRec_slope * sw_tempInterior + COMF_sw_nonRec_offsetCoeff;

      /* Check if the computed duty value is lower than the maximum rpm value.*/
      if(sw_rpmValue <= COMF_i_MAX_RPM)
      {
        /* The duty value is obtained by appling the rule of 3. */
        sw_dutyValue = sw_rpmValue / 18 ;  
      }
      else
      {
        /* If the rpm is highet than maximum duty will be set to high preset. */
        sw_dutyValue = COMF_i_HIGH_PRESET;  
      }

      /* Set current fan speed for the ouside fan accordingly with computed duty value. */
      FAN_t_setSpeed(sw_dutyValue, COMF_sw_fan_direction);

      #if COMF_DEBUG_FLAG == 1
          ESP_LOGI(TAG, "Temperature: %d  RPM: %d DUTY :%d ", sw_tempInterior, sw_rpmValue, sw_dutyValue);
      #endif

      break;
    }
  }
}

/**
 * @brief This function sets the speed at which the fan will operate.
 * 
 * @param t_speed Speed for the fan
 */
void COMF_v_setSpeed(int t_speed)
{
  /* Assign the speed settings to the global variable. */
  COMF_t_speed = t_speed;

  if(t_speed == COMF_i_OFF_PRESET)
  {
    ESP_LOGI(TAG,"AM OPRIT");
    FAN_t_setSpeed(COMF_i_LOW_PRESET,COMF_sw_fan_direction);
  }
  else 
  {
    /* Set the mode to the saved one in the global variable. */
    COMF_v_setMode(COMF_t_mode);
  }
  
  /* Save the desired speed in the non volatile memory. */
  NVM_t_setFanSpeed(t_speed);

  
}

/**
 * @brief This function sets the mode in which the system will operate.
 * 
 * @param t_comf_mode The operating mode for the system
 */
void COMF_v_setMode(comf_mode_t t_comf_mode)
{
  /* Decide on the working mode */
  switch(t_comf_mode)
  {
    case COMF_i_OTI:
    {
      /*Set operating mode of the system to operate only the outside fan. */
      COMF_t_mode =  COMF_i_OTI;

      /* Set fan direction to outside fan. */
      COMF_sw_fan_direction = FAN_i_DIRECTION_OUT;

      /* Configure manual mode */
      FAN_t_setSpeed(COMF_t_speed,COMF_sw_fan_direction);

      /* Set current state of recycling mode to wait */
      COMF_t_rec_currentState = COMF_i_WAIT_REC;      

      /* Set current state of recycling mode to non recycling */
      COMF_t_nonRec_currentState= COMF_i_WAIT_NONREC; 

      break;
    }
    case COMF_i_ITO:
    {
      /*Set operating mode of the system to operate only the outside fan.*/
      COMF_t_mode =  COMF_i_ITO;

      /* Set fan direction to inside fan. */
      COMF_sw_fan_direction = FAN_i_DIRECTION_IN;

      /* Configure manual mode */
      FAN_t_setSpeed(COMF_t_speed,COMF_sw_fan_direction);

      /* Set current state of recycling mode to wait */
      COMF_t_rec_currentState = COMF_i_WAIT_REC;      

      /* Set current state of recycling mode to non recycling */
      COMF_t_nonRec_currentState= COMF_i_WAIT_NONREC; 

      break;
    }
    case COMF_i_REC:
    {
      /*Set operating mode of the system to recycling mode.*/
      COMF_t_mode =  COMF_i_REC;

      /* Set current state of non recycling mode to wait */
      COMF_t_nonRec_currentState = COMF_i_WAIT_NONREC; 

      /* Set current state of recycling mode to set direction */
      COMF_t_rec_currentState =  COMF_i_SET_DIRECTION; 

      /* According to the current speed set for recycling mode the switching of the fans should occur at different times.  */
      if (COMF_t_speed <= COMF_i_LOW_PRESET)
      {
         /* If the fans operate at low speed they shold switch after 3 minutes. */
          COMF_t_rec_stop_counter = COMF_i_COUNTER_3_MIN;
      }
      else if (COMF_t_speed <= COMF_i_MED_PRESET)
      {
        /* If the fans operate at medium speed they shold switch after 2 minutes. */
        COMF_t_rec_stop_counter = COMF_i_COUNTER_2_MIN;
      }
      else
      {
        /* If the fans operate at high speed they shold switch after 1 minutes. */
        COMF_t_rec_stop_counter = COMF_i_COUNTER_1_MIN;
      }
  
      break;
    }
      case COMF_i_NONREC:
    {
      /*Set operating mode of the system to non-recycling mode.*/
      COMF_t_mode =  COMF_i_NONREC;



      /* Set fan direction to inside fan. */
      COMF_sw_fan_direction = FAN_i_DIRECTION_OUT;

      /* Set current state of non recycling mode to computations  */
      COMF_t_nonRec_currentState = COMF_i_COMPUTATIONS;  

      /* Set current state of recycling mode to wait */
      COMF_t_rec_currentState =  COMF_i_WAIT_REC;       

      break;
    }
  }
  
  NVM_t_setFanMode(COMF_t_mode);

}

/**
 * @brief Function for processing commands recieved via MQTT
 * 
 * @param aub_MQTT_payload The payload recieved from MQTT
 */
void COMF_v_recieveMQTTCommand(char* aub_MQTT_payload)
{
    /* Check the command from MQTT */
   if(strcmp(aub_MQTT_payload ,"out") == 0)
    {
      #if COMF_DEBUG_FLAG == 1
        ESP_LOGI(TAG, "MQTT sent command for the system to operate on the outside fan.");
      #endif 

      /* Set the system to operate on outside fan */
      COMF_v_setMode(COMF_i_OTI);
    }
    else if(strcmp(aub_MQTT_payload ,"in") == 0)
    {
      #if COMF_DEBUG_FLAG == 1
        ESP_LOGI(TAG, "MQTT sent command for the system to operate on the inside fan.");
      #endif 

      /* Set the system to operate on outside fan */
      COMF_v_setMode(COMF_i_ITO);
    }
    else if(strcmp(aub_MQTT_payload ,"recycling") == 0)
    {
      #if COMF_DEBUG_FLAG == 1
        ESP_LOGI(TAG, "MQTT sent command for the system to operate on recycling mode.");
      #endif 

      /* Set the system to operate on recycling mode */
      COMF_v_setMode(COMF_i_REC);
    }
    else if(strcmp(aub_MQTT_payload ,"non-recycling") == 0)
    {
      #if COMF_DEBUG_FLAG == 1
        ESP_LOGI(TAG, "MQTT sent command for the system to operate on non-recycling mode.");
      #endif 
      /* Set the system to operate on non-recycling mode */
      COMF_v_setMode(COMF_i_NONREC);
    }
    else if(strcmp(aub_MQTT_payload ,"auto") == 0)
    {
      #if COMF_DEBUG_FLAG == 1
        ESP_LOGI(TAG, "MQTT sent command for the system to operate on auto mode.");
      #endif 
      /* Set the system to operate on non-recycling mode */
      COMF_v_setMode(COMF_i_REC);

      /* Set speed to medium preset. */
      COMF_v_setSpeed(COMF_i_MED_PRESET);
    }
    else if(strcmp(aub_MQTT_payload ,"off") == 0)
    {
      #if COMF_DEBUG_FLAG == 1
        ESP_LOGI(TAG, "MQTT sent command for turning off the fans.");
      #endif 

      /* Set the speed to off preset.*/
      COMF_v_setSpeed(COMF_i_OFF_PRESET);
    }
    else if(strcmp(aub_MQTT_payload ,"on") == 0)
    {
      #if COMF_DEBUG_FLAG == 1
        ESP_LOGI(TAG, "MQTT sent command for turning on the fans.");
      #endif 
         
      /* Set the system to operate on last mode */
      COMF_v_setSpeed(COMF_t_mode);

      /* Set the system to operate on last speed */
      COMF_v_setSpeed(COMF_t_speed);

    }
    else
    {
      /* Convert the string that contains the speed to a integer. */
      int16_t sw_speed = atoi(aub_MQTT_payload);

      #if COMF_DEBUG_FLAG == 1
        ESP_LOGI(TAG, "MQTT sent speed to %d.",sw_speed);
      #endif 

      /* Set the recieved speed. */
      COMF_v_setSpeed(sw_speed);
    }
      
}

/**
 * @brief Return the current mode.
 * 
 * @return comf_mode_t 
 */
comf_mode_t COMF_t_getMode(void)
{
  return COMF_t_mode;
}

/**
 * @brief Returns the current speed.
 * 
 * @return comf_speed_preset_t 
 */
comf_speed_preset_t COMF_t_getSpeed(void)
{
  return COMF_t_speed;
}

/**
 * @brief Request the calibrated RPM from the FAN module.
 * 
 * @param fan_direction_t 
 * @return uint16_t 
 */
uint16_t COMF_uw_requestRPM(COMF_FAN_DIRECTION fan_direction_t)
{
  /* Check if fan direction is inside->outside. */
  if(fan_direction_t==COMF_DIRECTION_IN)
  {
    /* Return calibrated values for fan inside->outside. */
    return FAN_uw_calibratedRPM_ItO();
  }
  else
  {
    /* Return calibrated values for fan outside->inside. */
    return FAN_uw_calibratedRPM_OtI();
  }
}