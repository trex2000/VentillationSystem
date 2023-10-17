/**
 * @file FAN_Config.h
 * @author Durus Mihai-Valer (mihai.durus@altran.com)
 * @brief 
 * @version 1.0
 * @date 2021-08-06
 * @copyright Copyright (c) 2021
 * 
 */

/*============================================================================*/
/*                                 ALTRAN                                     */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*!
 * $Source: FAN_Config.h $
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
 * $Log: MCPWM_Config.h  $
  ============================================================================*/

#ifndef FAN_CONFIG_H
#define FAN_CONFIG_H

/* Includes */
/*============================================================================*/

#include "GPIO_Interface.h"
#include <driver/mcpwm.h>
#include <driver/pcnt.h>
#include <esp_log.h>
#include "FAN_Control.h"

/* Defines */
/*============================================================================*/

#define FAN_i_PCNT_H_LIM_VAL            10000     ///< Set the maximum limit value to watch
#define FAN_i_PCNT_L_LIM_VAL           -10000     ///< Set the minimum limit value to watch

#define FAN_i_PWM_INIT_DUTY_ItO            0      ///< Initial duty cycle for fan inside->outside
#define FAN_i_PWM_TEST_DUTY_ItO            0      ///< Initial duty cycle for fan outside->inside
#define FAN_i_PWM_INIT_DUTY_OtI          100      ///< Initial duty cycle for fan outside->inside
#define FAN_i_PWM_TEST_DUTY_OtI            0      ///< Test duty cycle for fan outside->inside

#define FAN_i_PWM_FREQUENCY             25000     ///< Frequency for the pwm signal which drives the fans
#define FAN_i_PWM_TEST_FREQUENCY          300     ///< Test frequency for the pwm signal which drives the fans

#define FAN_lf_DUTY_COEFF               17.31     ///< Equation slope for the calibration of fan speed accoriding to datasheet
#define FAN_lf_OFFSET_RPM               33.22     ///< Equation offset for the calibration of fan speed accoriding to datasheet

#define FAN_i_DOUBLE_2_INT                 10     ///< Conversion from double to int
#define FAN_i_1MIN_2_SEC                   60     ///< Representation of 1 minute in seconds
#define FAN_i_2_PULSES                      2     ///< Represents the number of pulses which comes from the tachometer pin of the fan

#define FAN_i_ZERO_RPM                      0     ///< ZERO as a value
#define FAN_i_ZERO_FRQ                      0     ///< ZERO as RPM
#define FAN_i_ZERO_VAL                      0     ///< ZEROS as Frequency

#define FAN_i_RPM_CALIBRATED_MIN          380     ///< Minimum fan speed RPM after calibration
#define FAN_i_DUTY_MIN                     20     ///< Minimum value for the duty cycle after which fan speed is not affected anymore

#define FAN_i_MAX_SPEED_ERROR             500     ///< Maximum allowed speed error between the actual speed read by tachometer pin and the theoretical one from datasheet
/* Constants and types */
/*============================================================================*/

mcpwm_pin_config_t pin_config = {
    .mcpwm0a_out_num = PWM_ItO,                ///< Configuration of MCPWM0xA output pin
    .mcpwm1a_out_num = PWM_OtI                 ///< Configuration of MCPWM1xA output pin
    };


mcpwm_config_t pwm_config;                     ///< defining pwm structure for configuration

pcnt_config_t pcnt_config_i2o;                 ///< defining pulse counter structure for configuration
pcnt_config_t pcnt_config_o2i;

/* Exported Variables */
/*============================================================================*/


/* Exported functions prototypes */
/*============================================================================*/

#endif  

