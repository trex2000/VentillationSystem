/**
 * @file GPIO_GPIO.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-04-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */
/* Includes */
/*============================================================================*/
#include "driver/gpio.h"
#include "GPIO_Config.h"
#include "GPIO_GPIO.h"

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
void GPIO_Init(void)
{
    /**
     * @brief   GPIO_Type_1
     *          - disable interrupt
     *          - set as output mode           
     *          - disable pull-down mode
     *          - disable pull-up mode
     */
    gpio_config_t GPIO_Type_1;
    //disable interrupt
    GPIO_Type_1.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    GPIO_Type_1.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set
    GPIO_Type_1.pin_bit_mask = GPIO_TYPE_1_PIN;
    //disable pull-down mode
    GPIO_Type_1.pull_down_en = 0;
    //disable pull-up mode
    GPIO_Type_1.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&GPIO_Type_1);
}

void GPIO_Set(unsigned char gpio_num)
{
    gpio_set_level(gpio_num,1);
}

void GPIO_Clear(unsigned char gpio_num)
{
    gpio_set_level(gpio_num,0);
}
/* Exported functions */
/*============================================================================*/