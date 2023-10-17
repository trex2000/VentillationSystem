/**
 * @file Temperature_Interface.h
 * @author Ecaterina Lutac (ecaterina.lutac@altran.com)
 * @brief 
 * @version 1.2
 * @date 2021-09-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef Temperature_Interface_H
#define Temperature_Interface_H
#define DEBUG_TEMPERATURE       1       ///< set as 1 when debug mode is ON, set as 0 when debug mode is OFF
/* Includes */
/*============================================================================*/
#include <stdint.h>

/* Constants and types */
/*============================================================================*/
typedef enum {
    TEMPERATURE_i_SUCCESS = 0,              ///< No errors
    TEMPERATURE_i_DEVICE_NOT_CONNECTED,     ///< The addressed device is not connected
    TEMPERATURE_i_NOT_INITIALIZED,          ///< The bus is not initialized 
    TEMPERATURE_i_CAN_NOT_READ_TEMPERATURE, ///< No response received from the addressed device
    TEMPERATURE_i_UNKNOWN_DEVICE,           ///< Unknown rom_code found
    TEMPERATURE_i_MISSING_DEVICE,           ///< One or both temperature sensors are missing
    TEMPERATURE_i_OUTSIDE_INTERVAL,         ///< Temperature outside the limit
} temperature_err_t;

/* Exported Variables */
/*============================================================================*/

extern int32_t Temperature_i_Interior;      ///< interior value in case some module needs it (modifying this does not modify the true value of the temperature)
extern int32_t Temperature_i_Exterior;      ///< interior value in case some module needs it (modifying this does not modify the true value of the temperature)

/* Exported functions prototypes */
/*============================================================================*/
//#ifdef DEBUG_TEMPERATURE_C 
extern void Temperature_v_Init();               ///< function that initialises the one wire bus
extern void Temperature_v_Read_Temperature();   ///< function that reads the temperature values from the temperature sensors (must be repeated in a cycle)
extern int32_t Temperature_i_getInterior();     ///< function that delivers the value of the interior temperature
extern int32_t Temperature_i_getExterior();     ///< function that delivers the value of the exterior temperature
#endif