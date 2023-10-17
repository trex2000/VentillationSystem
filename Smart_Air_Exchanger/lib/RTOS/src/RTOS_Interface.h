/*============================================================================*/
/*                                 ALTRAN                                     */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*!
 * $Source: RTOS_Control.h $
 * $Revision: 1 $
 * $Author: George Ciuclea $
 * $Date: 2021.08.05 $
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file
    Control infrastructure for starting the RTOS
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
 * $Log: RTOS_Interface.h  $
  ============================================================================*/
#ifndef RTOS_INTERFACE_H
#define RTOS_INTERFACE_H

/**
 * @brief Flag for debugging
 * 
 */
#define RTOS_DEBUG 0

/* Includes */
/*============================================================================*/
#include "RTOS_Utils.h"

/* Constants and types */
/*============================================================================*/
/**
 * @brief Enum for the task types, based on loop duration
 * 
 */
typedef enum{
  /**
   * @brief Every 5ms
   * 
   */
  CYCLE_1000MSC1,
  /**
   * @brief Every 50ms
   * 
   */
  CYCLE_50MS,
  /**
   * @brief Every 500ms
   * 
   */
  CYCLE_500MS,
  /**
   * @brief Every 1000ms
   * 
   */
  CYCLE_1000MS,
  /**
   * @brief Every 1min
   * 
   */
  CYCLE_1MIN
}TASK_TYPE_LIST;

/**
 * @brief Enum for how to extract the value for the task performance
 * 
 */
typedef enum
{
  /**
   * @brief Extract the smallest value
   * 
   */
  MIN_VALUE,
  /**
   * @brief Extract the average of the values (rounded down)
   * 
   */
  AVG_VALUE, 
  /**
   * @brief Extract the biggest value
   * 
   */
  MAX_VALUE 
}STAT_METRIC_TYPE;

/* Exported Variables */
/*============================================================================*/


/* Exported functions prototypes */
/*============================================================================*/
/**
 * @brief Used to start the whole RTOS, including subsequent tasks
 * 
 */
extern void rtosStart();

/**
 * @brief Used to trigger a controlled reboot of the system
 * 
 */
extern void rtosReboot();

/**
 * @brief Get the complete performance stats for all tasks, in a buffer, ready to be printed
 * 
 * @param stats Should be 256 bytes long
 */
extern void getPerformanceStats(char* stats);

/**
 * @brief Get the performance stats for a single task type, with a single type of value extracted, as a number of ticks elapsed during the execution of the task (1ms ticks)
 * 
 * @param selectedTask Specify the type of task (by loop duration)
 * @param metricType Specify required metric (by min/avg/max duration of multiple logged loops)
 * @return The number of ticks elapsed during runtime of requested task 
 */
extern unsigned int getPerformanceStatsSelective(TASK_TYPE_LIST selectedTask, STAT_METRIC_TYPE metricType);

#endif  

