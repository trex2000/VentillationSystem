/*============================================================================*/
/*                                 ALTRAN                                     */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*!
 * $Source: RTOS_API.c $
 * $Revision: 1 $
 * $Author: George Ciuclea $
 * $Date: 2021.08.05 $
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file
    Holds RTOS runtime functions that will hold calls for various submodule
    API functions, in order to run them.
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
 * $Log: RTOS_API.c  $
  ============================================================================*/

/* Includes */
/*============================================================================*/
#include "RTOS_API.h"
#include "NVM_Interface.h"
#include "WIFI_Interface.h"
#include "FAN_Interface.h"
#include "Temperature_Interface.h"
#include "NTP_interface.h"
#include "RTC_interface.h"
#include "TimeManagement_interface.h"
#include "GPIO_GPIO.h"
#include "Humidity_Interface.h"
#include "MQTT_Interface.h"
#include "Comfort_Interface.h"
#include "LED_Interface.h"
#include "ErrorManagement_Interface.h"
/* Constants and types  */
/*============================================================================*/


/* Variables */
/*============================================================================*/


/* Private functions prototypes */
/*============================================================================*/


/* Inline functions */
/*============================================================================*/


/* Private functions */
/*============================================================================*/


/* Exported functions */
/*============================================================================*/

void rtosNetworkInitTask()
{
  WIFI_v_Init();//Initialize and connect to Wi-fi
  MQTT_v_Init();
 
}

void rtosInitTask()
{
  //PLACE YOUR FUNCTIONS HERE 
  NVM_t_init(); //Initialize the non-volatile storage.
  GPIO_v_Init(); //Initialize GPIO pins
  LED_t_init(); 
  Temperature_v_Init();
  FAN_v_configMCPWM(); // Initialize all the GPIO pins necessary for MCPWM
  FAN_t_init_ItO(); // Initialize fan responsible for inside->outside
  FAN_t_init_OtI(); // Initialize fan responsible for outside->inside
  HUMID_t_init(); // Initialize the humidity sensor
  NTP_t_init(); // Initialize the NTP server connection
  COMF_v_Init();
  //MQTT_v_Init();
}

void rtosMQTTInitTask()
{
  //MQTT_v_Init();
}

void rtos1000msTaskC1()
{
  //PLACE YOUR FUNCTIONS HERE
}

void rtos50msTask()
{
  //PLACE YOUR FUNCTIONS HERE
  
  
}

void rtos500msTask()
{
  //PLACE YOUR FUNCTIONS HERE
  TimeManagement_t_handleTimeEvents();
}

void rtos1000msTask()
{
  //PLACE YOUR FUNCTIONS HERE
  FAN_v_updateSpeedValues(); // Reads the values from the counters and updated the validity flag
  frequencyFiltering_ItO(); // filtering pulses read from the pulse counter module
  frequencyFiltering_OtI();
  FAN_v_speedLogging(); // Print data to serial monitor
  Temperature_v_Read_Temperature();
  //LED_t_setRed();
  COMF_v_recyclingMode();
  COMF_v_nonRecyclingMode();
  HUMID_i_readHumidity();
  ERM_t_displayErrorsOnHomeAssistant();  
  ERM_t_displayErrorsOnLed();
}

void rtos1minTask()
{
  
}

void rtosShutdownTask()
{
  //PLACE YOUR FUNCTIONS HERE
}