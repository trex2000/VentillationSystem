/*============================================================================*/
/*                                 ALTRAN                                     */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*!
 * $Source: RTOS_API.h $
 * $Revision: 1 $
 * $Author: George Ciuclea $
 * $Date: 2021.08.05 $
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file
    Header for RTOS cyclical functions that need to be filled with
    the various needed subroutines of each time
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
 * $Log: RTOS_API.h  $
  ============================================================================*/
#ifndef RTOS_API_H
#define RTOS_API_H

/* Includes */
/*============================================================================*/


/* Constants and types */
/*============================================================================*/


/* Exported Variables */
/*============================================================================*/


/* Exported functions prototypes */
/*============================================================================*/

/**
 * @brief Is called in the startup routine, runs on core 1
 * 
 */
void rtosNetworkInitTask();

/**
 * @brief Is called in the startup routine, runs on core 0, just for MQTT, after all other tasks
 * 
 */
void rtosMQTTInitTask();

/**
 * @brief Is called in the startup routine, runs on core 0
 * 
 */
void rtosInitTask();

/**
 * @brief Is called every 5ms during runtime
 * 
 */
void rtos1000msTaskC1();

/**
 * @brief Is called every 50ms during runtime
 * 
 */
void rtos50msTask();

/**
 * @brief Is called every 500ms during runtime
 * 
 */
void rtos500msTask();

/**
 * @brief Is called every 1000ms during runtime
 * 
 */
void rtos1000msTask();

/**
 * @brief Is called every 1 min during runtime
 * 
 */
void rtos1minTask();

/**
 * @brief Is called before a controlled reboot/shutdown
 * @warning Will not be called if reboot is unexpected
 * 
 */
void rtosShutdownTask();

#endif  

