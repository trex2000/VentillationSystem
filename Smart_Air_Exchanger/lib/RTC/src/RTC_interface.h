/**
 * @file RTC_interface.h
 * @author maria.irimus@altran.com
 * @brief 
 * @version 0.1
 * @date 2021-08-08
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
 * $Source: RTC_interface.h $
 * $Revision: 1 $
 * $Author: Maria Irimus $
 * $Date: 2021.08.08 $
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
 * $Log: RTC_interface.h  $
  ============================================================================*/

#ifndef RTC_INTERFACE_H
#define RTC_INTERFACE_H

/* Includes */
/*============================================================================*/
#include "RTC_TIME.h"

/* Defines */
/*============================================================================*/

/*! Debug flag: 0 - disable info printed, 1 - enable info printed */
#define RTC_DEBUG        0    

/* Constants and types */
/*============================================================================*/

/* Structure for error types */
typedef enum {
    RTC_i_OK = 0,                   ///< Success 
    RTC_i_ERR_CREATE_LINK,          ///< An I2C command list was not created 
    RTC_i_ERR_CONVERT_TIME,         ///< The time from the timeInfo structure could not be represented 
    RTC_i_ERR_INVALID_ARG,          ///< Invalid arguments 
    RTC_i_ERR_INVALID_STATE,        
} rtc_err_t;

/* Exported Variables */
/*============================================================================*/

/* Exported functions prototypes */
/*============================================================================*/
extern rtc_err_t RTC_t_I2C_masterInit(void);
extern rtc_err_t RTC_t_getTime(void);
extern rtc_err_t RTC_t_updateTime(void);

#endif