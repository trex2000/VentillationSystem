/**
 * @file Humidity_Example.c
 * @author Dragos Craciun (dragos.craciun@altran.com)
 * @brief 
 * @version 1.2
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
 * $Source: Humidity_Example.c $
 * $Revision: 1 $
 * $Author: Dragos Craciun $
 * $Date: 2021.08.25 $
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file
 *      
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
 * $Log: Humidity_Example.c  $
  ============================================================================*/

/* Includes */
/*============================================================================*/
#include "Humidity_Interface.h"
#include "Humidity_Config.h"

void Humidity_v_example() {
    /* Initialize the module. */
    /* ====================================================================== */
    humid_err_t err = HUMID_t_init();

    if (err != HUMID_i_OK) 
    {
        /* Pass error to error handleing module */
        return;
    } 
    else 
    {
        /* Empty */
    }

    /* Check the status of the module. */
    /* ====================================================================== */
    err = HUMID_t_checkStatus();

    if (err != HUMID_i_INITIALIZED) 
    {
        /* Call initializing function */
    } 
    else 
    {
        /* Empty */
    }

    /* Read and convert data into relative humidity format. */
    /* ====================================================================== */
    uint32_t value = HUMID_i_readHumidity();
    
    if ( value == HUMID_i_ERR_READ )
    {
        /* Pass error to error handleing module */
        return;

    }
    else 
    {
        /* Empty */
    }

    /* Send each new relative humidity value to the Comfort Management module. */
    /* ====================================================================== */
    value = HUMID_i_return_different_humidity();

    
}