/*============================================================================*/
/*                                 ALTRAN                                     */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*!
 * $Source: RTOS_Config.h $
 * $Revision: 1 $
 * $Author: George Ciuclea $
 * $Date: 2021.08.10 $
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** @file
    
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
 * $Log: RTOS_Performance.h  $
  ============================================================================*/
#ifndef RTOS_CONFIG_H
#define RTOS_CONFIG_H
/* Includes */
/*============================================================================*/

/* Constants and types */
/*============================================================================*/
/**
 * @brief Number of different task times
 * 
 */
#define TIME_CATEGORIES 5
/**
 * @brief Number of values to be stored for the average performance
 * 
 */
#define MAX_TICK_VALUES_AVERAGE 16

/**
 * @brief The durations of the task types
 * 
 */
typedef enum{
  TIME_5MS=5,
  TIME_50MS=50,
  TIME_500MS=500,
  TIME_1000MS=1000,
  TIME_1MIN=60000
}TASK_TIMES_LIST;
/* Exported Variables */
/*============================================================================*/

/* Exported functions prototypes */
/*============================================================================*/

#endif