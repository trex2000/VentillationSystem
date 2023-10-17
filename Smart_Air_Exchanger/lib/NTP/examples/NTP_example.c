/**
 * @file NTP_example.c
 * @author Maria Irimus (maria.irimus@altran.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-25
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
 * $Source: NTP_example.c $
 * $Revision: 1 $
 * $Author: Maria Irimus $
 * $Date: 2021.08.25 $
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
 * $Log: NTP_example.c  $
  ============================================================================*/

/* Includes */
/*============================================================================*/
#include "NTP_interface.h"

ntp_err_t NTP_t_example()
{

    // Variable for error status
    ntp_err_t t_err_status;

    /* Initialize the NTP */
    /* ====================================================================== */

    t_err_status = NTP_t_init();

    if (t_err_status != NTP_i_OK)
    {
        return t_err_status;
    }
    else
    {
        /* Empty */
    }

    /* Try to retrive data from the NTP server until the time is set correctly */
    /* ====================================================================== */
    NTP_v_tryConnect();

    /* Try to retrive data from the NTP server until the time is set correctly */
    /* ====================================================================== */

    t_err_status = NTP_t_getTime();

    if (t_err_status != NTP_i_OK)
    {
        return t_err_status;
    }
    else
    {
        /* Empty */
    }

    /* Check the status of the connection */
    /* ====================================================================== */
    t_err_status = NTP_t_connectionStatus();

    if (t_err_status != NTP_i_OK)
    {
        return t_err_status;
    }
    else
    {
        /* Empty */
    }
}