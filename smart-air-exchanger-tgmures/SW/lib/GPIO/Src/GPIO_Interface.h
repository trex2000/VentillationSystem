/**
 * @file GPIO_Interface.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-04-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef GPIO_Interface_H
#define GPIO_Interface_H

/* Includes */
/*============================================================================*/
#include "GPIO_Config.h"

/* Constants and types */
/*============================================================================*/
#ifdef LOLIN_32_LITE
    #define LED_BLUE    GPIO_22
    #define EXT_TEMP    GPIO_4
    #define PWM_ItO     GPIO_2
    #define PWM_OtI     GPIO_5
#endif
/* Exported Variables */
/*============================================================================*/


/* Exported functions prototypes */
/*============================================================================*/
extern void GPIO_Init(void);
extern void GPIO_Set(unsigned char);
extern void GPIO_Clear(unsigned char);
#endif  




