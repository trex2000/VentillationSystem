/**
 * @file RTC_TIME.h
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
 * $Source: RTC_TIME.h $
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
 * $Log: NTP_TIME.h  $
  ============================================================================*/

#ifndef RTC_TIME_H
#define RTC_TIME_H

/* Includes */
/*============================================================================*/
#include "WIFI_Interface.h"

#include "driver/i2c.h"
#include "time.h"

#include "esp_sntp.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_attr.h"

/* Defines */
/*============================================================================*/

/*! GPIO number used for I2C master clock */
#define I2C_MASTER_SCL_IO             RTC_CONFIG_I2C_MASTER_SCL     

/*! GPIO number used for I2C master data */
#define I2C_MASTER_SDA_IO             RTC_CONFIG_I2C_MASTER_SDA      

/*! I2C master I2C port number, the number of I2C peripheral interfaces available will depend on the chip */
#define I2C_MASTER_NUM                (int)           0                                                                        

/*! I2C master clock frequency */                                                               
#define I2C_MASTER_FREQ_HZ            (int)           400000    

/*! I2C master doesn't need buffer */
#define I2C_MASTER_TX_BUF_DISABLE     (int)           0         

/*! I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE     (int)           0          

/*! I2C master timeout in ms */
#define I2C_MASTER_TIMEOUT_MS         (int)           1000       

/* Constants and types */
/*============================================================================*/

/* Private functions prototypes*/
/*============================================================================*/

/* Exported Variables */
/*============================================================================*/

/* Exported functions prototypes */
/*============================================================================*/

#endif