/**
 * @file MQTT_Interface.h
 * @author Baleanu Sorina (sorina.baleanu@altran.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-16
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
 * $Source: MQTT_Interface.h $
 * $Revision: 1 $
 * $Author: Baleanu Sorina $
 * $Date: 2021.08.16 $
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
 * $Log: MQTT_Interface.h $
  ============================================================================*/
#ifndef MQTT_INTERFACE_H
#define MQTT_INTERFACE_H

/* Includes */
/*============================================================================*/

#include <stdint.h>

/* Constants and types */
/*============================================================================*/
#define MQTT_DEBUG FLAG 0 ///<Debug flag for MQTT module

typedef enum {
    MQTT_i_SEND_HUMIDITY = 0,      ///< Flag for humidity updates
    MQTT_i_SEND_TEMP_IN,          ///< Flag for inside temperature sensor updates
    MQTT_i_SEND_TEMP_OUT,         ///< Flag for outside temperature sensor updates
}mqtt_sent_t;

/* Exported Variables */
/*============================================================================*/


/* Exported functions prototypes */
/*============================================================================*/
extern void MQTT_v_Init();

extern void MQTT_v_SendUpdates(mqtt_sent_t t_sent_from_module, uint32_t sensor_value);

extern void MQTT_v_SendErrorsFan(char* aub_error_text);
extern void MQTT_v_SendErrorsTemp(char* aub_error_text);
extern void MQTT_v_SendErrorsHumi(char* aub_error_text);
extern void MQTT_v_SendErrorsTime(char* aub_error_text);
extern void MQTT_v_SendErrorsWifi(char* aub_error_text);
#endif  

