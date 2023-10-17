/**
 * @file Humidity_Control.h
 * @author Tudor Coroian (tudor.coroian@altran.com)
 *         Dragos Craciun (dragos.craciun@altran.com)
 * @brief 
 * @version 1.2
 * @date 2021-08-19
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
 * $Source: Humidity_Control.h $
 * $Revision: 1 $
 * $Author: Tudor Coroian, Dragos Craciun $
 * $Date: 2021.08.19 $
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file
 *      
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
 * $Log: Humidity_Control.h  $
  ============================================================================*/
  

#ifndef HUMIDITY_CONTROL_H
#define HUMIDITY_CONTROL_H

/* Includes */
/*============================================================================*/
#include "Humidity_Config.h"
#include "Humidity_Interface.h"


/* Defines */
/*============================================================================*/
#define HUMID_t_MASTER_SDA              SDA_Humidity    ///< GPIO number used for I2C master clock
#define HUMID_t_MASTER_SCL              SCL_Humidity    ///< GPIO number used for I2C master data
#define HUMID_i_MASTER_NUM              (int) 0         ///< I2C master port number
#define HUMID_i_MASTER_TX_BUF_DISABLE   (int) 0         ///< I2C master doesn't need buffer
#define HUMID_i_MASTER_RX_BUF_DISABLE   (int) 0         ///< I2C master doesn't need buffer
#define HUMID_i_MASTER_FREQ             (int) 100000    ///< I2C master clock frequency in Hz (400KHz); alternative 100KHz or 40KHz
#define HUMID_i_MASTER_TIMEOUT          (int) 1000      ///< I2C master timeout in ms (1s)

#define HUMID_i_SENSOR_ADDRESS          (int) 0x40      ///< Slave address of the humidity sensor
#define HUMID_i_HUMIDTY_HOLD            (int) 0xe5      ///< Trigger humidity measurement with hold on master
#define HUMID_i_HUMIDTY_NO_HOLD         (int) 0xf5      ///< Trigger humidity measurement with no hold on master
#define HUMID_i_SOFT_RESET              (int) 0xfe      ///< Soft reset command


/* Constants and types */
/*============================================================================*/


/* Exported Variables */
/*============================================================================*/


/* Exported functions prototypes */
/*============================================================================*/


#endif 