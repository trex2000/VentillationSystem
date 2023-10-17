/**
 * @file FAN_Interface.h
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
 * $Source: FAN_Interface.h $
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
 * $Log: FAN_Interface.h  $
  ============================================================================*/
  
#ifndef FAN_INTERFACE_H
#define FAN_INTERFACE_H

/* Includes */
/*============================================================================*/

#include <stdint.h>

/* Constants and types */
/*============================================================================*/

typedef int fan_err_t;                                 ///< Define structure for error handling

#define FAN_SPEED_DEBUG_RPM_CALIBRATED            0    ///< Used to lock or unlock debugging mode for the fan speed and display RPM calibrated values
#define FAN_SPEED_DEBUG_RPM                       0    ///< Used to lock or unlock debugging mode for the fan speed and display RPM values
#define FAN_SPEED_DEBUG_FRQ                       0    ///< Used to lock or unlock debugging mode for the fan speed and display Frequency values in Hertz

#define FAN_i_ERR_INIT                      (int) 0    ///< Error in initialization of fan 
#define FAN_i_OK_OtI                        (int) 1    ///< Fan outside->inside initialization succesfull 
#define FAN_i_OK_ItO                        (int) 2    ///< Fan inside->outside initialization succesfull 

#define FAN_i_INIT_ItO                      (int) 3    ///< Macros used to denote the initializtion status of the fan inside->outside
#define FAN_i_NOT_INIT_ItO                  (int) 4    

#define FAN_i_INIT_OtI                      (int) 5    ///< Macros used to denote the initializtion status of the fan outside->inside
#define FAN_i_NOT_INIT_OtI                  (int) 6

#define FAN_i_SET_SPEED_OK                  (int) 7    ///< Set speed command succesfully
#define FAN_i_UPDATE_SPEED_OK               (int) 8    ///< Updated speed values succesfully
#define FAN_i_ERR_SPEED_ItO                 (int) 9    ///< Fan speed error for fan inside->outside
#define FAN_i_SPEED_OK_ItO                  (int)10    ///< Fan speed OK for fan inside->outside
#define FAN_i_ERR_SPEED_OtI                 (int)11    ///< Fan speed error for fan outside->inside
#define FAN_i_SPEED_OK_OtI                  (int)12    ///< Fan speed OK for fan outside->inside

#define FAN_i_DIRECTION_OUT                       0    ///< Macros used to denote fan direction: 1 -> inside, 0 -> outside
#define FAN_i_DIRECTION_IN                        1

#define FAN_i_REVERSE_PWM                       100    ///< Macro used to revert to duty cycle for the MCPWM module

/* Exported Variables */
/*============================================================================*/


/* Exported functions prototypes */
/*============================================================================*/

extern void FAN_v_configMCPWM(void);

extern fan_err_t FAN_t_init_OtI(void);
extern fan_err_t FAN_t_init_ItO(void);

extern fan_err_t FAN_t_setSpeed(uint16_t,uint8_t);
extern uint16_t FAN_uw_getSpeed(uint8_t);
extern fan_err_t FAN_v_updateSpeedValues(void);
extern void frequencyFiltering_ItO(void);
extern void frequencyFiltering_OtI(void);
extern uint16_t FAN_uw_calibratedRPM_ItO(void);
extern uint16_t FAN_uw_calibratedRPM_OtI(void);
extern fan_err_t FAN_t_detectSpeedError_ItO(void);
extern fan_err_t FAN_t_detectSpeedError_OtI(void);
extern void FAN_v_speedLogging(void);

#endif 


