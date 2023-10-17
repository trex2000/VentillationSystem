/**
 * @file ErrorManagement.h
 * @author tudor.coroian@altran.com
 *         maria.irimus@altran.com
 * @brief 
 * @version 0.1
 * @date 2021-08-27
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
 * $Source: ErrorManagement.h $
 * $Revision: 1 $
 * $Author: Tudor Coroian $
 *          Maria Irimus  $
 * $Date: 2021.08.27 $
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file
 *      [to be written]
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
 * $Log: ErrorManagement.h  $
  ============================================================================*/
  

#ifndef ERRORMANAGEMENT_H
#define ERRORMANAGEMENT_H

/* Includes */
/*============================================================================*/
#include "ErrorManagement_Interface.h"
#include "ErrorManagement_Config.h"

/* Defines */
/*============================================================================*/

/* Error masks */
#define ERM_i_TEMP_ERROR_1_MASK         (int)       0x0001     ///< One or both sensors are not connected on the One Wire Bus
#define ERM_i_HUMI_ERROR_1_MASK         (int)       0x0002     ///< The humidity sensor was not initialized successfully
#define ERM_i_WIFI_ERROR_2_MASK         (int)       0x0004     ///< ESP32 failed to create an AP
#define ERM_i__NTP_ERROR_1_MASK         (int)       0x0008     ///< The server connection failed
#define ERM_i__RTC_ERROR_1_MASK         (int)       0x0010     ///< The time received is not correct
#define ERM_i__FAN_ERROR_1_MASK         (int)       0x0020     ///< Fan module has not been initialized.
#define ERM_i__FAN_ERROR_2_MASK         (int)       0x0040     ///< Fan inside->outside speed error.
#define ERM_i__FAN_ERROR_3_MASK         (int)       0x0080     ///< Fan outside->inside speed error.

/* Warning masks */
#define ERM_i_TEMP_WARNING_2_MASK       (int)       0x0001     ///< Sensor is malfunctioning
#define ERM_i_TEMP_WARNING_3_MASK       (int)       0x0002     ///< Read temperature is outside the given interval
#define ERM_i_HUMI_WARNING_2_MASK       (int)       0x0004     ///< The humidity could not be read correctly by the sensor
#define ERM_i_WIFI_WARNING_1_MASK       (int)       0x0008     ///< ESP32 is not connected to a network


/* Constants and types */
/*============================================================================*/
/**
 * @brief Describe the type of errors that may appear
 * 
 */
typedef enum {
    ERM_i_ERRORTYPE_ERROR = 0,      ///< Error type
    ERM_i_ERRORTYPE_WARNING,        ///< Warning type
    ERM_i_ERRORTYPE_INFO,           ///< Info type
} erm_type_t;

/**
 * @brief Structure used to store information about the error
 * 
 */
typedef struct error {
    /* Identifiers */
    const char ac_sender[5];       ///< Module identifier (on 4 letters)
    const char ac_number[4];        ///< Error number

    /* Priority */
    const int i_priority;          ///< Priority (lower number -> higher priority)

    /* Display */
    const char ac_msg[64];         ///< Message to be displayed

    /* Time */
    int i_timeHold;                ///< Time for which the error must hold in order to be displayed (in .5sec)
} erm_errorInfo_t;

/* Exported Variables */
/*============================================================================*/


/* Exported functions prototypes */
/*============================================================================*/


#endif 