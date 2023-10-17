/**
 * @file PWM_Interface.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-04-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef PWM_INTERFACE_H
#define PWM_INTERFACE_H

/* Includes */
/*============================================================================*/


/* Constants and types */
/*============================================================================*/


/* Exported Variables */
/*============================================================================*/


/* Exported functions prototypes */
/*============================================================================*/

extern void PWM_OtI_Init();
extern void PWM_OtI_Set_Speed();
extern void PWM_OtI_Get_Speed();

extern void PWM_ItO_Init();
extern void PWM_ItO_Set_Speed();
extern void PWM_ItO_Get_Speed();
#endif 