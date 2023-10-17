/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-04-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Includes */
/*============================================================================*/
#include "GPIO_Interface.h"
#include "DS18B20_Interface.h"
#include "PWM_Interface.h"
#include "WIFI_Interface.h"
#include "TASK_Interface.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include <string.h>

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
  
void app_main(void)
{
uint8_t i = 0;
char Buffer[1000];
    #ifdef LED_BLUE
        GPIO_Init(); //initialization
        for(i=0;i<10;i++)
        {
            GPIO_Set(LED_BLUE);
            ESP_LOGI("LED_BLUE", "ON");
            vTaskDelay(1000 / portTICK_RATE_MS);
            GPIO_Clear(LED_BLUE);
            ESP_LOGI("LED_BLUE", "OFF");
            vTaskDelay(1000 / portTICK_RATE_MS);
        }
    #endif
    #ifdef EXT_TEMP
        Ext_Temp_Init();
    #endif
    #ifdef PWM_OtI
        void PWM_OtI_Init();
    #endif
    #ifdef PWM_ItO
        void PWM_ItO_Init();
    #endif
//    WIFI_Init();
//    WIFI_Connect();
}

/* Exported functions */
/*============================================================================*/


