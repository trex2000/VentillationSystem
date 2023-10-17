
/**
 * @file WIFI_AP.c
 * @author Baleanu Sorina (sorina.baleanu@altran.com)
 * @brief  Interface for Wi-fi module. It contains a function for 
 * initializing and connecting to the Wi-fi, as well as a function for checking the Wi-fi connection.
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

#ifndef WIFI_INTERFACE_H
#define WIFI_INTERFACE_H

/* Includes */
/*============================================================================*/
#include "WIFI_Control.h"

/* Constants and types */
/*============================================================================*/

/* Debug flag for WIFI module. */
#define WIFI_DEBUG_FLAG                0 

/* Debug flag for websocket. */
#define WEBSOCKET_DEBUG                0




/* Exported Variables */
/*============================================================================*/


/* Exported functions prototypes */
/*============================================================================*/
extern void WIFI_v_Init();
extern int8_t WIFI_i_IsConnected();


#endif 