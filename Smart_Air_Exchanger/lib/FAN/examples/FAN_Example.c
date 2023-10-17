/**
 * @file FAN_Example.c
 * @author Durus Mihai-Valer (mihai.durus@altran.com)
 * @brief 
 * @version 1.0
 * @date 2021-08-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */
/*============================================================================*/
/*============================================================================*/
/*                                ALTRAN                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*!
 * $Source: FAN_Example.c $
 * $Revision: 1 $
 * $Author: Durus Mihai-Valer $
 * $Date: 2021.08.06 $
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file
    short description in one sentence end with dot.
    detailed
    multiline
    description of the file
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
 * $Log: FAN_Example.c  $
  ============================================================================*/

/* Includes */
/*============================================================================*/

#include "FAN_Interface.h"

void FAN_v_example()
{
    /* Initialize the module */
    /* ====================================================================== */

    fan_err_t fan_err_ItO=FAN_t_init_ItO();
    fan_err_t fan_err_OtI=FAN_t_init_OtI();
    
    if(fan_err_t_ItO!=FAN_i_ERR_INIT)
    {
         /* Pass error to error handleing module */
        return;
    }
    else
    {
        /* Empty */
    }

    if(fan_err_t_OtI!=FAN_i_ERR_INIT)
    {
         /* Pass error to error handleing module */
        return;
    }
    else
    {
        /* Empty */
    }

    /* In this moment the fans have been initialized to the preset configuration from FAN_Control.c */

    uint16_t dutyValue=20;  // Any integer value bewtween 0 and 100
    uint8_t fanDirection=0; // 0->outside; 1->inside

    /* Speed manipulation */
    /* ====================================================================== */


    /* To set the speed of the fans call: */
    FAN_t_setSpeed(dutyValue,fanDirection);

    /* To get the speed of the fans call: */

    uint16_t getDuty=FAN_uw_getSpeed(fanDirection);

    /* In order to use other functionality like speed monitoring, filtering and calibration 
       in the real time operating system in 1 second task should be called the functions: 
       FAN_v_updateSpeedValues(); // Reads the values from the counters and updated the validity flag
       frequencyFiltering_ItO(); // filtering pulses read from the pulse counter module
       frequencyFiltering_OtI();
       FAN_v_speedLogging(); // Print data to serial monitor */
} 

