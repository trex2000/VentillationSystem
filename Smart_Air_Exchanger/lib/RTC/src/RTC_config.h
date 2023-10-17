/**
 * @file RTC_config.h
 * @author maria.irimus@altran.com
 * @brief 
 * @version 0.1
 * @date 2021-08-08
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
 * $Source: RTC_config.h $
 * $Revision: 1 $
 * $Author: Maria Irimus $
 * $Date: 2021.08.08 $
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
 * $Log: RTC_config.h  $
  ============================================================================*/

#ifndef RTC_CONFIG_H
#define RTC_CONFIG_H

/* Includes */
/*============================================================================*/
#include "GPIO_Interface.h"

/* Defines */
/*============================================================================*/

/*! Slave address of the DS3231 RTC module */
#define DS3231_ADDR                           0x68      

/*! GPIO number used for I2C master clock */
#define RTC_CONFIG_I2C_MASTER_SCL             SCL_Humidity       

/*! GPIO number used for I2C master data */
#define RTC_CONFIG_I2C_MASTER_SDA             SDA_Humidity        

/*! The number of seconds in a minute */
#define RTC_SECONDS_IN_A_MINUTE       (int)   60

/*! The number of miliseconds in a second */
#define RTC_MILIS_IN_A_SECOND         (int)   1000

/*! Cyclicity of the task in which the NTP is used (in miliseconds)  */
#define RTC_TASK_DELAY                (int)   500       

/*! Time between synchronisations (in minutes) */
#define RTC_MINUTES_FOR_REFRESH       (int)   10    

/*! The number of calls of the getTime() function until the specific delay time is achieved */
#define RTC_TIME_REFRESH_DELAY_COUNT  (int)   ((RTC_MINUTES_FOR_REFRESH * RTC_SECONDS_IN_A_MINUTE * RTC_MILIS_IN_A_SECOND) / RTC_TASK_DELAY)

/*! Daylight saving offset */
#define RTC_DAYLIGHT_OFFSET           (int)   -1

/*! I2C port number */
#define RTC_I2C_PORT_NUMBER           (int)    0

/*! The number of the DS3231 timekeeping registers used */
#define RTC_REGISTERS_NUMBER          (int)    6 

/*! Flag used to allocate the interrupt */
#define RTC_INTERRUPT_FLAG            (int)    0      

/* Constants and types */
/*============================================================================*/

/* Exported Variables */
/*============================================================================*/

/* Exported functions prototypes */
/*============================================================================*/

#endif