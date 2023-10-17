/**
 * @file GPIO_Config.h
 * @author Radu Larisa (larisa.radu@altran.com)
 * @brief 
 * @version 0.1
 * @date 2021-04-09
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
 * $Source: GPIO_Config.h $
 * $Revision: 1 $
 * $Author: Radu Larisa $
 * $Date: 2021.08.05 $
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file
    [to be written]
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
 * $Log: GPIO_Config.h  $
  ============================================================================*/

#ifndef GPIO_Config_H
#define GPIO_Config_H

/* Includes */
/*============================================================================*/

//#include "GPIO_Interface.h"

/* Constants and types */
/*============================================================================*/
/**
 * @brief Board config
 * 
 */
#define LOLIN_32_LITE
/**
 * @brief GPIO List and functionality
 * 
 */
#ifdef LOLIN_32_LITE
    #define GPIO_0  0       // PIN  DTR / RTS
    #define GPIO_1  1       //      TX
    #define GPIO_2  2       // PIN  PWM_ItO Motor_In_to_Out
    #define GPIO_3  3       //      RX
    #define GPIO_4  4       // PIN  TEMP    External temperature Sensors
    #define GPIO_5  5       // PIN  PWM_OtI Motor_Out_to_In
    #define GPIO_6  6       //      FLASH   FLASH_CLK
    #define GPIO_7  7       //      FLASH   SDO / SD0
    #define GPIO_8  8       //      FLASH   SDI / SD1
    #define GPIO_9  9       //      FLASH   SHD / SD2
    #define GPIO_10  10     //      FLASH   SWP / SD3
    #define GPIO_11  11     //      FLASH   SCS / CMD
    #define GPIO_12  12     // PIN
    #define GPIO_13  13     // PIN
    #define GPIO_14  14     // PIN
    #define GPIO_15  15     // PIN
    #define GPIO_16  16     // PIN
    #define GPIO_17  17     // PIN
    #define GPIO_18  18     // PIN
    #define GPIO_19  19     // PIN
    #define GPIO_20  
    #define GPIO_21  21     // PIN  I2C SCL
    #define GPIO_22  22     // PIN  I2C SDA
    #define GPIO_23  23     // PIN
    #define GPIO_24  
    #define GPIO_25  25     // PIN
    #define GPIO_26  26     // PIN
    #define GPIO_27  27     // PIN
    #define GPIO_28  
    #define GPIO_29  
    #define GPIO_30  
    #define GPIO_31  
    #define GPIO_32  32     // PIN
    #define GPIO_33  33     // PIN
    #define GPIO_34  34     // PIN
    #define GPIO_35  35     // PIN
    #define GPIO_36  36     // PIN  VP
    #define GPIO_37  37     // VP
    #define GPIO_38  38     // VN
    #define GPIO_39  39     // PIN  VN
#endif

#define GPIO_TYPE_1_PIN     ((1ULL<<GPIO_22))
#define GPIO_TYPE_PCNT_PIN  ((1ULL<<GPIO_12) | (1ULL<<GPIO_27) | (1ULL<<GPIO_5) | (1ULL<<GPIO_18))
//#define GPIO_OUT_PINS       ((1ULL<<GPIO_2) | (1ULL<<GPIO_4) | (1ULL<<GPIO_15) | (1ULL << GPIO_13) | (1ULL << GPIO_26))
#define GPIO_OUT_PINS       ((1ULL<<GPIO_2) | (1ULL<<GPIO_4) | (1ULL<<GPIO_15))
#define GPIO_MAX_PIN_NUMBER 40
#define GPIO_HIGH           1
#define GPIO_LOW            0


/* Exported Variables */
/*============================================================================*/


/* Exported functions prototypes */
/*============================================================================*/

#endif  

