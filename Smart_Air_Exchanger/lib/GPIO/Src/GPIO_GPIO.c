/**
 * @file GPIO_GPIO.c
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
 * $Source: GPIO_GPIO.c $
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
 * $Log: GPIO_GPIO.c  $
  ============================================================================*/

/* Includes */
/*============================================================================*/
#include "GPIO_Config.h"
#include "GPIO_GPIO.h"
#include "GPIO_Interface.h"

/* Constants and types  */
/*============================================================================*/

/* Variables */
/*============================================================================*/

/* Private functions prototypes */
/*============================================================================*/

/* Inline functions */
/*============================================================================*/

/* Private functions */
/*============================================================================*/
/**
 * @brief Initialize GPIO Pin for the selectet porpuse.
 *        Do not edit the existing GPIOTypes configuration!
 *        Create new configuration if it does not exist.
 */
void GPIO_v_Init(void)
{
    /**
     * t_type_1
     * - disable interrupt
     * - set as output mode           
     * - disable pull-down mode
     * - disable pull-up mode
     *  t_type_2
     * - disable interrupt
     * - set as input mode           
     * - disable pull-down mode
     * - enable pull-up mode
     *  t_type_3
     * - disable interrupt
     * - set as output mode           
     * - disable pull-down mode
     * - disable pull-up mode
     */

    gpio_config_t t_type_1;
    //disable interrupt
    t_type_1.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    t_type_1.mode = GPIO_MODE_INPUT_OUTPUT;
    //bit mask of the pins that you want to set
    t_type_1.pin_bit_mask = GPIO_TYPE_1_PIN;
    //disable pull-down mode
    t_type_1.pull_down_en = 0;
    //disable pull-up mode
    t_type_1.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&t_type_1);


    // pins used for PCNT
    gpio_config_t t_type_2;
    //disable interrupt
    t_type_2.intr_type = GPIO_INTR_DISABLE;
    //set as input mode
    t_type_2.mode = GPIO_MODE_INPUT;
    //bit mask of the pins that you want to set
    t_type_2.pin_bit_mask = GPIO_TYPE_PCNT_PIN;
    //disable pull-down mode
    t_type_2.pull_down_en = 0;
    //enable pull-up mode
    t_type_2.pull_up_en = 1;
    //configure GPIO with the given settings
    gpio_config(&t_type_2);

    // pins used for PCNT
    gpio_config_t t_type_3;
    //disable interrupt
    t_type_3.intr_type = GPIO_INTR_DISABLE;
    //set as input mode
    t_type_3.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set
    t_type_3.pin_bit_mask = GPIO_OUT_PINS;
    //disable pull-down mode
    t_type_3.pull_down_en = 0;
    //enable pull-up mode
    t_type_3.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&t_type_3);

    //set pins levels to HIGH to initialize the RGB led
    gpio_set_level(LED_RGB_R,GPIO_HIGH);
    gpio_set_level(LED_RGB_G,GPIO_HIGH);
    gpio_set_level(LED_RGB_B,GPIO_HIGH);
}

/**
 * @brief Set the value of the GPIO Pin to HIGH
 * 
 * @param gpio_num GPIO Pin number
 */
void GPIO_v_Set(unsigned char num)
{
    // checking the pinout range
    if (num < GPIO_MAX_PIN_NUMBER) 
    {
        gpio_set_level(num,GPIO_HIGH);
    } 
    else 
    {
        /* Empty */
    }
}

/**
 * @brief Get the value of the GPIO Pin
 * 
 * @param gpio_num GPIO Pin number
 */
int GPIO_v_Get(unsigned char num)
{
    int gpio_level = 0;
    // checking the pinout range
    if (num < GPIO_MAX_PIN_NUMBER) 
    {
        // getting pin level
        gpio_level = gpio_get_level(num);
    } 
    else 
    {
        /* Empty */
    }
    return gpio_level;
}

/**
 * @brief Set the value of the GPIO Pin to LOW
 * 
 * @param gpio_num GPIO Pin number
 */
void GPIO_v_Clear(unsigned char num)
{
    // checking the pinout range
    if (num < GPIO_MAX_PIN_NUMBER) 
    {
       gpio_set_level(num,GPIO_LOW);
    } 
    else 
    {
        /* Empty */
    }
}

/* Exported functions */
/*============================================================================*/