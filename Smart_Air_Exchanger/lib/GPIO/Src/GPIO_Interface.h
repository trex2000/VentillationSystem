/**
 * @file GPIO_Interface.h
 * @author Radu Larisa (larisa.radu@altran.com)
 * @brief 
 * @version 0.1
 * @date 2021-04-09
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
 * $Source: GPIO_Interface.h $
 * $Revision: 1 $
 * $Author: Radu Larisa $
 * $Date: 2021.08.05 $
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file
    [to be written]
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
 * $Log: GPIO_Interface.h  $
  ============================================================================*/

#ifndef GPIO_Interface_H
#define GPIO_Interface_H

/* Includes */
/*============================================================================*/
#include "GPIO_Config.h"
/* Constants and types */
/*============================================================================*/
#ifdef LOLIN_32_LITE
    #define LED_RGB_R           GPIO_15
    #define LED_RGB_G           GPIO_2
    #define LED_RGB_B           GPIO_4
    #define SCL_Humidity        GPIO_21
    #define SDA_Humidity        GPIO_22
    #define TEMP_PIN            GPIO_23
    #define PWM_ItO             GPIO_13
    #define PWM_OtI             GPIO_14
    #define SIGNAL_FAN_ItO      GPIO_12
    #define SIGNAL_FAN_OtI      GPIO_27
    #define SIGNAL_CTRL_ItO     GPIO_5
    #define SIGNAL_CTRL_OtI     GPIO_18
    //#define MOTOR_STATE_ItO     GPIO_13
    //#define MOTOR_STATE_OtI     GPIO_26
#endif
/* Exported Variables */
/*============================================================================*/


/* Exported functions prototypes */
/*============================================================================*/
extern void GPIO_v_Init(void);
extern void GPIO_v_Set(unsigned char);
extern int  GPIO_v_Get(unsigned char);
extern void GPIO_v_Clear(unsigned char);
#endif  




