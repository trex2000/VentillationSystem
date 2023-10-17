/**
 * @file RTC_example.c
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
 * $Source: RTC_example.c $
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
 * $Log: RTC_example.c  $
  ============================================================================*/

/* Includes */
/*============================================================================*/
#include "RTC_interface.h"

rtc_err_t RTC_t_example()
{
    // Variable for error status
    rtc_err_t t_err_status;

    /* Initialize the I2C master */
    /* ====================================================================== */

    t_err_status = RTC_t_I2C_masterInit;

    if (t_err_status != RTC_i_OK)
    {
        return t_err_status;
    }
    else
    {
        /* Empty */
    }

    /* Set the internal clock of the microcontroller  */
    /* ====================================================================== */

    t_err_status = RTC_t_getTime;

    if (t_err_status != RTC_i_OK)
    {
        return t_err_status;
    }
    else
    {
        /* Empty */
    }

     /* Sync the current system time with RTC time  */
    /* ====================================================================== */

    t_err_status = RTC_t_updateTime;

    if (t_err_status != RTC_i_OK)
    {
        return t_err_status;
    }
    else
    {
        /* Empty */
    }
}