/**
 * @file LED_Diagnostic.h
 * @author tudor.coroian@altran.com
 * @brief 
 * @version 0.1
 * @date 2021-08-11
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
 * $Source: LED_Diagnostic.h $
 * $Revision: 1 $
 * $Author: Tudor Coroian $
 * $Date: 2021.08.11 $
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
 * $Log: LED_Diagnostic.h  $
  ============================================================================*/
  

#ifndef LED_DIAGNOSTIC_H
#define LED_DIAGNOSTIC_H

/* Includes */
/*============================================================================*/
#include "LED_Config.h"
#include "LED_Interface.h"

/* Defines */
/*============================================================================*/
#define LED_t_SPEED_MODE            LEDC_LOW_SPEED_MODE     ///< Choose the group of channels that operate in LOW speed mode
#define LED_t_TIMER_NUMB            LEDC_TIMER_0            ///< Select the first timer
#define LED_t_DUTY_RESOLUTION       LEDC_TIMER_8_BIT        ///< Set the duty resolution of the timer to 8 bits
#define LED_i_FREQUENCY             (int) 1000              ///< Frequency in Hz. Set the frequency to 1kHz
#define LED_t_CLOCK                 LEDC_AUTO_CLK           ///< Choose the driver clock for the timer
#define LED_t_CHANNEL_RED           LEDC_CHANNEL_0          ///< Select the channel for the RED LED
#define LED_t_CHANNEL_BLUE          LEDC_CHANNEL_1          ///< Select the channel for the BLUE LED
#define LED_t_CHANNEL_GREEN         LEDC_CHANNEL_2          ///< Select the channel for the GREEN LED
#define LED_t_INTERRUPT             LEDC_INTR_DISABLE       ///< Select the interrupt type for the channel
#define LED_t_RED_PIN               LED_RGB_R               ///< Select the pin for the RED LED
#define LED_t_BLUE_PIN              LED_RGB_B               ///< Select the pin for the BLUE LED
#define LED_t_GREEN_PIN             LED_RGB_G               ///< Select the pin for the GREEN LED
#define LED_i_INIT_DUTY             (int) 256               ///< Initial duty
#define LED_i_QUARTER_DUTY          (int) 192               ///< Quarter duty           (based on resolution) => 255 - (2**8 - 1) * 25%
#define LED_i_HALF_DUTY             (int) 128               ///< Half duty              (based on resolution) => 255 - (2**8 - 1) * 50%
#define LED_i_TRIQUARTER_DUTY       (int) 64                ///< Three quarters duty    (based on resolution) => 255 - (2**8 - 1) * 75%
#define LED_i_FULL_DUTY             (int) 0                 ///< Full duty              (based on resolution) => 255 - (2**8 - 1) * 100%
#define LED_i_INIT_HPOINT           (int) 0                 ///< Initial hpoint


/* Constants and types */
/*============================================================================*/
typedef enum {
    LED_i_CONTROL_INITIALIZED = 0,    ///< LED control module was initialized
    LED_i_CONTROL_NOT_INITIALIZED,    ///< LED control module was not initialized
    LED_i_CONTROL_IN_USE,             ///< LED control currently being used by another module
    LED_i_CONTROL_NOT_IN_USE,         ///< LED control not currently being used by another module
} led_control_status_t;

/* Exported Variables */
/*============================================================================*/


/* Exported functions prototypes */
/*============================================================================*/


#endif 