/**
 * @file NTP_config.h
 * @author maria.irimus@altran.com
 * @brief 
 * @version 0.1
 * @date 2021-08-05
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
 * $Source: NTP_config.h $
 * $Revision: 1 $
 * $Author: Maria Irimus $
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
 * $Log: NTP_config.h  $
  ============================================================================*/

#ifndef NTP_CONFIG_H
#define NTP_CONFIG_H

/* Includes */
/*============================================================================*/

/*! The name of the time zone */
#define NTP_TIME_ZONE_NAME                  "EET-2EEST,M3.5.0/3,M10.5.0/4"

/*! Overwrite the name in the environment for setenv() function */
#define NTP_OVERWRITE_ENV_NAME        (int)   1

/*! SNTP server number; 0 is the main server */
#define NTP_SNTP_SERVER_NUMBER        (int)   0

/*! Domain name; must be a legitimate SNTP server */
#define NTP_SNTP_SERVER_NAME                 "pool.ntp.org"       

/*! Maximum number of attempts to retrive data from the NTP */
#define NTP_CONNECTION_RETRY_COUNT    (int)   50  

/*! The number of seconds in a minute */
#define NTP_SECONDS_IN_A_MINUTE       (int)   60

/*! The number of miliseconds in a second */
#define NTP_MILIS_IN_A_SECOND         (int)   1000

/*! Cyclicity of the task in which the NTP is used (in miliseconds) */
#define NTP_TASK_DELAY                (int)   500         

/*! Time between synchronisations (in minutes) */
#define NTP_MINUTES_FOR_REFRESH       (int)   10 

/*! The number of calls of the getTime() function until the specific delay time is achieved */
#define NTP_TIME_REFRESH_DELAY_COUNT  (int)   ((NTP_MINUTES_FOR_REFRESH * NTP_SECONDS_IN_A_MINUTE * NTP_MILIS_IN_A_SECOND) / NTP_TASK_DELAY)

#define NTP_CURRENT_YEAR              (int)   2021

/* Constants and types */
/*============================================================================*/

/* Exported Variables */
/*============================================================================*/

/* Exported functions prototypes */
/*============================================================================*/

#endif