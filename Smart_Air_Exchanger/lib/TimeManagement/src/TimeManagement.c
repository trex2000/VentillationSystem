/**
 * @file TimeManagement.c
 * @author maria.irimus@altran.com
 * @brief 
 * @version 0.1
 * @date 2021-08-21.
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
 * $Source: TimeManagement.c $
 * $Revision: 1 $
 * $Author: Maria Irimus $
 * $Date: 2021.08.21 $
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
 * $Log: TimeManagement.c  $
  ============================================================================*/

#ifndef TimeManagement_C
#define TimeManagement_C

/* Includes */
/*============================================================================*/
#include "TimeManagement_interface.h"
#include "TimeManagement.h"
#include "TimeManagement_config.h"

/* Defines */
/*============================================================================*/

#define TimeManagement_RESET_ZERO           (int)   0

#define TimeManagement_NEW_NTP_CONNECTION   (int)   1

/* Constants and types */
/*============================================================================*/

/* Variables */
/*============================================================================*/

/* Private functions prototypes */
/*============================================================================*/

/* Inline functions */
/*============================================================================*/

/* Private functions */
/*============================================================================*/

/* Exported Variables */
/*============================================================================*/

/* Exported functions prototypes */
/*============================================================================*/

/**
 * @brief Wrapper for NTP and RTC modules; handles time events 
 * 
 */
rtc_err_t TimeManagement_t_handleTimeEvents(void);

/* Exported functions */
/*============================================================================*/

rtc_err_t TimeManagement_t_handleTimeEvents(void)
{
    // Status of the task
    rtc_err_t t_err_status = RTC_i_OK;

    // Check if WIFI connection is available
    if (WIFI_i_IsConnected())
    {
        // Retrive data from the NTP
        NTP_v_tryConnect(); 

        // Set the internal time to the time received from the NTP server
        t_err_status = NTP_t_getTime();    

        // Variable to check if new NTP connection is available
        static char sb_newNTPSync = TimeManagement_RESET_ZERO;

        // Check if the NTP has connected to the server
        if (NTP_ub_getConnectionError())
        {
            // Reset the flag for a new connection
            sb_newNTPSync = TimeManagement_NEW_NTP_CONNECTION;
        }
        else
        {
            /* Empty */
        }

        // If there is a new connection
        if (!NTP_ub_getConnectionError())
        {
            if (sb_newNTPSync)
            {
                // Update the RTC time according to the NTP server time
                t_err_status = RTC_t_updateTime();
                // Set the flag for a new sync
                sb_newNTPSync = TimeManagement_RESET_ZERO;
            }
            else
            {
                /* Empty */
            }
        }
        else
        {
            /* Empty */
        }
    }
    else
    {
        // If no WIFI connection is available, get the time from the RTC module
        t_err_status = RTC_t_getTime();
    }

    return t_err_status;
}

#endif