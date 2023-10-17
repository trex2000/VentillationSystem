/**
 * @file NTP_interface.h
 * @author maria.irimus@altran.com
 * @brief 
 * @version 0.1
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
 * $Source: NTP_interface.h $
 * $Revision: 1 $
 * $Author: Maria Irimus $
 * $Date: 2021.08.05 $
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
 * $Log: NTP_interface.h  $
  ============================================================================*/

#ifndef NTP_INTERFACE_H
#define NTP_INTERFACE_H

/* Includes */
/*============================================================================*/
#include "NTP_TIME.h"

/* Defines */
/*============================================================================*/

/*! Debug flag: 0 - disable info printed, 1 - enable info printed */
#define DEBUG_NTP 1    

/* Constants and types */
/*============================================================================*/

/* Structure for error types */
typedef enum {
    NTP_i_OK = 0,                   ///< Success 
    NTP_i_ERR_CONNECTION,           ///< The connection failed  
    NTP_i_ERR_SYSTEM_TIME,          ///< The current system time was not read correctly 
    NTP_i_ERR_FORMAT_TIME           ///< The current system time could not be formatted 
} ntp_err_t;

/* Exported Variables */
/*============================================================================*/

/* Exported functions prototypes */
/*============================================================================*/

extern ntp_err_t NTP_t_init(void);
extern void NTP_v_tryConnect(void);
extern ntp_err_t NTP_t_getTime(void);
extern char NTP_ub_getConnectionError(void);
extern ntp_err_t NTP_t_connectionStatus(void);

extern int NTP_i_lastSync(void);
extern int NTP_i_getMin(void);
extern int NTP_i_getHour(void);
extern int NTP_i_getDay(void);
extern int NTP_i_getMonth(void);
extern int NTP_i_getYear(void);
extern int NTP_i_checkNightTime(void);

#endif