/*============================================================================*/
/*                                 ALTRAN                                     */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*!
 * $Source: RTOS_Performance.h $
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
#ifndef RTOS_PERFORMANCE_H
#define RTOS_PERFORMANCE_H
/* Includes */
/*============================================================================*/
#include "RTOS_Config.h"
#include "RTOS_Interface.h"
/* Constants and types */
/*============================================================================*/

/* Exported Variables */
/*============================================================================*/

/* Exported functions prototypes */
/*============================================================================*/

/**
 * @brief Get the average elapsed ticks from logged values of the specified task type
 * 
 * @param typeOfTask The type of task (by loop duration)
 * @return The unrounded average value
 */
unsigned int getAverageTicks(TASK_TYPE_LIST typeOfTask);

/**
 * @brief Get the smallest number elapsed ticks from logged values of the specified task type
 * 
 * @param typeOfTask The type of task (by loop duration)
 * @return The exact number of ticks
 */
unsigned int getMinTicks(TASK_TYPE_LIST typeOfTask);

/**
 * @brief Get the smallest number elapsed ticks from logged values of the specified task type
 * 
 * @param typeOfTask The type of task (by loop duration)
 * @return The exact number of ticks
 */
unsigned int getMaxTicks(TASK_TYPE_LIST typeOfTask);

/**
 * @brief Get the duration of the startup function calls (in ticks)
 * 
 * @return The exact number of ticks
 */
unsigned int getTimeStartup();

/**
 * @brief Log a number of elapsed ticks for a certain task
 * 
 * @param typeOfTask The type of task (by loop duration)
 * @param numTicks The exact number of ticks
 */
void registerTicks(TASK_TYPE_LIST typeOfTask, int numTicks);

/**
 * @brief Log the number of elapsed ticks for the startup task
 * 
 * @param numTicks The exact number of ticks
 */
void registerTimeStartup(int numTicks);

#endif

