/**
 * @file NVM_Interface.h
 * @author tudor.coroian@altran.com
 * @brief Interface for the Non-volatile memory module. It contains functions for 
 * initializing the non-volatile storage, as well as functions that allow the user
 * to access it.
 * @version 1.2
 * @date 2021-08-05
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
 * $Source: NVM_Interface.h $
 * $Revision: 1 $
 * $Author: Tudor Coroian $
 * $Date: 2021.08.05 $
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file
 *      Interface for the Non-volatile memory module. It contains functions for 
 * initializing the non-volatile storage, as well as functions that allow the user
 * to access it.
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
 * $Log: NVM_Interface.h  $
  ============================================================================*/

#ifndef NVM_INTERFACE_H
#define NVM_INTERFACE_H

/* Includes */
/*============================================================================*/
#include "NVM_Storage.h"

/* Constants and types */
/*============================================================================*/
#define NVM_DISABLE                         FALSE     ///< Disbale flag for the NVM module
#define NVM_DEBUG                           FALSE      ///< Debug flag for the NVM module

typedef int nvm_err_t;
#define NVM_i_OK                            (int) 0   ///< Success
#define NVM_i_ERR_ON_HOLD                   (int) 1   ///< Write function is on hold
#define NVM_i_ERR_OPEN                      (int) 2   ///< Could not open the non-volatile storage
#define NVM_i_ERR_VALUE_NOT_FOUND           (int) 3   ///< The value was not intialized yet
#define NVM_i_ERR_READ                      (int) 4   ///< Could not read the value from the non-volatile storage
#define NVM_i_ERR_WRITE                     (int) 5   ///< Could not write the value in the non-volatile storage
#define NVM_i_ERR_NO_CREDENTIALS_FOUND      (int) 6   ///< No network credentials were stored
#define NVM_i_ERR_INIT                      (int) 7   ///< Error when initiallizing the non-volatile storage
#define NVM_i_STORAGE_INITIALIZED           (int) 8   ///< Non-volatile storage was initialized
#define NVM_i_STORAGE_NOT_INITIALIZED       (int) 9   ///< Non-volatile storage was not initialized
#define NVM_i_ERR_EMPTY_PARAM               (int) 10  ///< Function expected a non-empty parameter
#define NVM_i_ERR_NON_EMPTY_PARAM           (int) 11  ///< Function expected an empty parameter
#define NVM_i_ERR_VALUE_OUTSIDE_BOUNDARIES  (int) 12  ///< Value passed as parameter is outside of the range
#define NVM_i_ERR_UNSPECIFIED_SIZE          (int) 13  ///< Size of the value to be stored was not specified

/**
 * @brief Provide consistent and controlled access to the fan related functions.
 * 
 */
typedef enum {
    NVM_i_MODE_AUTO = 0,                    ///< Automatic functioning mode
    NVM_i_MODE_MANUAL,                      ///< Manual functioning mode
    NVM_i_SPEED_LOW,                        ///< Low speed (for manual mode)
    NVM_i_SPEED_MEDIUM,                     ///< Medium speed (for manual mode)
    NVM_i_SPEED_HIGH,                       ///< High speed (for manual mode)
    NVM_i_STEP_1,                           ///< Step 1 for speed (for automatic mode)
    NVM_i_STEP_2,                           ///< Step 2 for speed (for automatic mode)
    NVM_i_STEP_3,                           ///< Step 3 for speed (for automatic mode)
    NVM_i_STEP_4,                           ///< Step 4 for speed (for automatic mode)
    NVM_i_STEP_5,                           ///< Step 5 for speed (for automatic mode)
    NVM_i_STEP_6,                           ///< Step 6 for speed (for automatic mode)
    NVM_i_STEP_7,                           ///< Step 7 for speed (for automatic mode)
    NVM_i_STEP_8,                           ///< Step 8 for speed (for automatic mode)
} nvm_fan_funcParam_t;


/* Exported Variables */
/*============================================================================*/


/* Exported functions prototypes */
/*============================================================================*/
extern nvm_err_t NVM_t_init(void);
extern nvm_err_t NVM_t_status(void);
extern nvm_err_t NVM_t_getWifiCredentials(char* ssid, char* pwd);
extern nvm_err_t NVM_t_setWifiCredentials(char* ssid, char* pwd);
extern nvm_err_t NVM_t_getTempOffset(int* offset);
extern nvm_err_t NVM_t_setTempOffset(int offset);
extern nvm_err_t NVM_t_getHumidityOffset(int* offset);
extern nvm_err_t NVM_t_setHumidityOffset(int offset);
extern nvm_err_t NVM_t_getFanSpeed(int* speed);
extern nvm_err_t NVM_t_setFanSpeed(int speed);
extern nvm_err_t NVM_t_getFanMode(int* mode);
extern nvm_err_t NVM_t_setFanMode(int mode);
extern nvm_err_t NVM_t_getFanDirection(int *direction);
extern nvm_err_t NVM_t_setFanDirection(int direction);

#endif