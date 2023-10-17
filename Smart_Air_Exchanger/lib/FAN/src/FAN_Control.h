/**
 * @file FAN_Control.h
 * @author Durus Mihai-Valer (mihai.durus@altran.com)
 * @brief 
 * @version 1.0
 * @date 2021-08-06
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
 * $Source: FAN_Control.h $
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
 * $Log: FAN_Control.h  $
  ============================================================================*/

#ifndef FAN_CONTROL_H
#define FAN_CONTROL_H

/* Includes */
/*============================================================================*/
#include "FAN_Interface.h"
/* Defines */
/*============================================================================*/



/* Constants and types */
/*============================================================================*/

/* Exported Variables */
/*============================================================================*/


/* Exported functions prototypes */
/*============================================================================*/

void FAN_v_configMCWPM(void);

void FAN_v_init_OtI(void);
void FAN_v_init_ItO(void);

uint16_t FAN_uw_getSpeed(uint8_t);
fan_err_t FAN_t_setSpeed(uint16_t,uint8_t);

fan_err_t FAN_v_updateSpeedValues(void);
void frequencyFiltering_ItO(void);
void frequencyFiltering_OtI(void);
uint16_t FAN_uw_calibratedRPM_ItO(void);
uint16_t FAN_uw_calibratedRPM_OtI(void);
fan_err_t FAN_t_detectSpeedError_ItO(void);
fan_err_t FAN_t_detectSpeedError_OtI(void);
void FAN_v_speedLogging(void);

#endif  

