/**
 * @file WIFI_AP.c
 * @author Baleanu Sorina (sorina.baleanu@altran.com)
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
 * $Source: WIFI_Interface.h $
 * $Revision: 1 $
 * $Author: Baleanu Sorina $
 * $Date: 2021.08.05 $
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
 * $Log: WIFI_Interface.h  $
  ============================================================================*/

#ifndef COMFORT_INTERFACE_H
#define COMFORT_INTERFACE_H

/* Includes */
/*============================================================================*/
#include "Comfort.h"
#include <stdint.h>

/* Constants and types */
/*============================================================================*/

/* Debug flag for Comfort managment module, 0 - debug logs are not available, 1- debug logs are available .*/
#define COMF_DEBUG_FLAG 1


typedef enum
{
  COMF_i_REC = 0, //<Recycling mode
  COMF_i_NONREC, ///<Non-recycling mode
  COMF_i_ITO, ///<Inside fan in manual mode
  COMF_i_OTI ///<Outside fan in manual mode

}comf_mode_t;

typedef enum
{
  COMF_i_OFF_PRESET=0,
  COMF_i_LOW_PRESET=20, ///< LOW speed for FAN
  COMF_i_MED_PRESET=60, ///<Medium speed for fan
  COMF_i_HIGH_PRESET=100 ///< HIGH speed for fan

}comf_speed_preset_t;

typedef enum{
COMF_DIRECTION_IN,
COMF_DIRECTION_OUT,
}COMF_FAN_DIRECTION;



/* Exported Variables */
/*============================================================================*/


/* Exported functions prototypes */
/*============================================================================*/
extern void COMF_v_Init(void);

extern void COMF_v_nonRecyclingMode(void);

extern void COMF_v_recyclingMode(void);

extern void COMF_v_setMode(comf_mode_t t_comf_mode);

extern comf_mode_t COMF_t_getMode(void);

extern comf_speed_preset_t COMF_t_getSpeed(void);

extern void COMF_v_recieveMQTTCommand(char* aub_MQTT_payload);

extern void COMF_v_setSpeed(int t_speed);



#endif 