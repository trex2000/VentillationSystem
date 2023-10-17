/**
 * @file Temperature.h
 * @author Ecaterina Lutac (ecaterina.lutac@altran.com)
 * @brief 
 * @version 1.2
 * @date 2021-09-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef Temperature_H
#define Temperature_H
/* Includes */
/*============================================================================*/
#include "esp_log.h" ///< library for debug logs
#include "string.h"  ///< library for string version of the device's rom-codes
#include "ds18b20.h" ///< library for temperature sensor
#include "NVM_Interface.h"  ///< library that gives the offset
#include "MQTT_Interface.h" ///< library to send the temperature values to
#include <stdint.h> ///< library for atomic version of variables

/* Constants and types */
/*============================================================================*/

/* Exported Variables */
/*============================================================================*/

/* Exported functions prototypes */
/*============================================================================*/

#endif