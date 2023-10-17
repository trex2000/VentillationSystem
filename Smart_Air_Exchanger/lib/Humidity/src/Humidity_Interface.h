/**
 * @file Humidity_Interface.h
 * @author Tudor Coroian (tudor.coroian@altran.com)
 *         Dragos Craciun (dragos.craciun@altran.com)
 * @brief Interface for the Humidity sensor module. It contains functions for
 * initializing the sensor, checking the sensor's status ( initialized or not initialized )
 * and data reading from the sensor. 
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
 * $Source: Humidity_Interface.h $
 * $Revision: 1 $
 * $Author: Tudor Coroian, Dragos Craciun $
 * $Date: 2021.08.19 $
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file
 * Interface for the Humidity sensor module. It contains functions for
 * initializing the sensor, checking the sensor's status ( initialized or not initialized )
 * and data reading from the sensor. 
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
 * $Log: Humidity_Interface.h  $
  ============================================================================*/

#ifndef HUMIDITY_INTERFACE_H
#define HUMIDITY_INTERFACE_H

/* Includes */
/*============================================================================*/


/* Defines */
/*============================================================================*/
#define HUMID_DEBUG     0        ///< Flag for enabling debug mode in the module

/* Constants and types */
/*============================================================================*/
typedef enum {
    HUMID_i_ERR_READ = 0xFFFFFFFF,      ///< Humidity reading is invalid
    HUMID_i_OK = 0,                     ///< Success
    HUMID_i_ERR_NOT_INITIALIZED,        ///< Humidity module was not initialized
    HUMID_i_ERR_INVALID_ARGS,           ///< Invalid arguments passed
    HUMID_i_ERR_INIT,                   ///< Error at initialization
    HUMID_i_ERR_INTERVAL,               ///< Value outside the set interval
    
} humid_err_t;

typedef enum {
    HUMID_i_INITIALIZED = 0,            ///< Humidity module was initialized
    HUMID_i_NOT_INITIALIZED,            ///< Humidity module was not initialized
} humid_status_t;


/* Exported Variables */
/*============================================================================*/


/* Exported functions prototypes */
/*============================================================================*/
extern humid_err_t HUMID_t_init();
extern humid_err_t HUMID_t_checkStatus();
extern uint32_t HUMID_i_return_different_humidity();
extern uint32_t HUMID_i_readHumidity();



#endif