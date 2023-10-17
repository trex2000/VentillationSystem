/**
 * @file LED_Diagnostic.c
 * @author tudor.coroian@altran.com
 * @brief 
 * @version 0.1
 * @date 2021-08-11
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
 * $Source: LED_Diagnostic.c $
 * $Revision: 1 $
 * $Author: Tudor Coroian $
 * $Date: 2021.08.11 $
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
 * $Log: LED_Diagnostic.c  $
  ============================================================================*/

/* Includes */
/*============================================================================*/
#include "LED_Diagnostic.h"

/* Constants and types  */
/*============================================================================*/
static const char* TAG = "LEDC";    ///< Tag used in printing logs when debuging

typedef enum {
    LED_i_GREEN = 0,            ///< Set the color of the LED to green
    LED_i_RED,                  ///< Set the color of the LED to red
    LED_i_BLUE,                 ///< Set the color of the LED to blue
    LED_i_YELLOW,               ///< Set the color of the LED to yellow
    LED_i_MAGENTA,              ///< Set the color of the LED to magenta
    LED_i_CYAN,                 ///< Set the color of the LED to cyan
    LED_i_ORANGE,               ///< Set the color of the LED to orange
    LED_i_WHITE,                ///< Set the color of the LED to white
    LED_i_PURPLE,               ///< Set the color of the LED to purple
    LED_i_BLACK,                ///< Turn of the LED
} led_color_t;


/* Variables */
/*============================================================================*/
static led_control_status_t LED_t_controller_status;

/* Private functions prototypes */
/*============================================================================*/


/* Inline functions */
/*============================================================================*/


/* Private functions */
/*============================================================================*/
/**
 * @brief 
 * 
 * @param color 
 * @return led_err_t 
 */
static led_err_t LED_t_setColor(led_color_t color) {
    if (LED_t_controller_status != LED_i_CONTROL_INITIALIZED) {
        #if LED_DEBUG == 1
            ESP_LOGI(TAG, "Module was not initialized before setting a color.");
        #endif
        return LED_i_STATUS_NOT_INITIALIZED;
    } else {
        /* Empty */
    }

    /* Store duty cycles for each channel of the LED */
    uint32_t duty_red   = LED_i_INIT_DUTY;
    uint32_t duty_green = LED_i_INIT_DUTY;
    uint32_t duty_blue  = LED_i_INIT_DUTY;

    esp_err_t err;

    /* Duty cycles for each channel are specified in |R|G|B| order (in %) */
    switch (color) { 
        /* Set the duty cycle of each channel */       
        case LED_i_GREEN:;     
            duty_red    = LED_i_INIT_DUTY;  /* RED:     0%      */
            duty_green  = LED_i_FULL_DUTY;  /* GREEN:   100%    */
            duty_blue   = LED_i_INIT_DUTY;  /* BLUE:    0%      */

            break;
        case LED_i_RED:;        
            duty_red    = LED_i_FULL_DUTY;  /* RED:     100%    */
            duty_green  = LED_i_INIT_DUTY;  /* GREEN:   0%      */
            duty_blue   = LED_i_INIT_DUTY;  /* BLUE:    0%      */

            break;
        case LED_i_BLUE:;       
            duty_red    = LED_i_INIT_DUTY;  /* RED:     0%      */
            duty_green  = LED_i_INIT_DUTY;  /* GREEN:   0%      */
            duty_blue   = LED_i_FULL_DUTY;  /* BLUE:    100%    */

            break;
        case LED_i_YELLOW:;     
            duty_red    = LED_i_FULL_DUTY;  /* RED:     100%    */
            duty_green  = LED_i_FULL_DUTY;  /* GREEN:   100%    */
            duty_blue   = LED_i_INIT_DUTY;  /* BLUE:    0%      */

            break;
        case LED_i_MAGENTA:;  
            duty_red    = LED_i_FULL_DUTY;  /* RED:     100%    */
            duty_green  = LED_i_INIT_DUTY;  /* GREEN:   0%      */
            duty_blue   = LED_i_FULL_DUTY;  /* BLUE:    100%    */

            break;
        case LED_i_CYAN:;    
            duty_red    = LED_i_INIT_DUTY;  /* RED:     0%      */
            duty_green  = LED_i_FULL_DUTY;  /* GREEN:   100%    */
            duty_blue   = LED_i_FULL_DUTY;  /* BLUE:    100%    */

            break;
        case LED_i_ORANGE:;
            duty_red    = LED_i_FULL_DUTY;  /* RED:     100%    */    
            duty_green  = LED_i_HALF_DUTY;  /* GREEN:   25%     */
            duty_blue   = LED_i_INIT_DUTY;  /* BLUE:    0%      */

            break;
        case LED_i_WHITE:;
            duty_red    = LED_i_FULL_DUTY;  /* RED:     100%    */    
            duty_green  = LED_i_FULL_DUTY;  /* GREEN:   100%    */
            duty_blue   = LED_i_FULL_DUTY;  /* BLUE:    100%    */

            break;
        case LED_i_BLACK:;
            duty_red    = LED_i_INIT_DUTY;  /* RED:      0%     */    
            duty_green  = LED_i_INIT_DUTY;  /* GREEN:    0%     */
            duty_blue   = LED_i_INIT_DUTY;  /* BLUE:     0%     */

            break;
        case LED_i_PURPLE:;  
            duty_red    = LED_i_QUARTER_DUTY;   /* RED:     25%     */
            duty_green  = LED_i_INIT_DUTY;      /* GREEN:   0%      */
            duty_blue   = LED_i_FULL_DUTY;      /* BLUE:    100%    */

            break;
        default:;
            /* Emmpty */
    }

    /* Change the duty cycle on the red channel */
    err = ledc_set_duty(LED_t_SPEED_MODE, LED_t_CHANNEL_RED, duty_red);
    ledc_update_duty(LED_t_SPEED_MODE, LED_t_CHANNEL_RED);

    /* Check for errors */
    if (err != ESP_OK) {
        #if LED_DEBUG == 1
            ESP_LOGI(TAG, "\e[1;31;40mWrong parameters were passed when updating the RED channel.\e[39;49m");
        #endif
        return LED_i_ERR_INVALID_ARGUMETS;
    } else {
        #if LED_DEBUG == 1
            ESP_LOGI(TAG, "\e[31;40m[RED] Duty cycle: %d / 256\e[39;49m", ledc_get_duty(LED_t_SPEED_MODE, LED_t_CHANNEL_RED));
        #endif
    }

    /* Change the duty cycle on the green channel */
    err = ledc_set_duty(LED_t_SPEED_MODE, LED_t_CHANNEL_GREEN, duty_green);
    ledc_update_duty(LED_t_SPEED_MODE, LED_t_CHANNEL_GREEN);

    /* Check for errors */
    if (err != ESP_OK) {
        #if LED_DEBUG == 1
            ESP_LOGI(TAG, "\e[1;31;40mWrong parameters were passed when updating the GREEN channel.\e[39;49m");
        #endif
        return LED_i_ERR_INVALID_ARGUMETS;
    } else {
        #if LED_DEBUG == 1
            ESP_LOGI(TAG, "\e[32;40m[GREEN] Duty cycle: %d / 256\e[39;49m", ledc_get_duty(LED_t_SPEED_MODE, LED_t_CHANNEL_GREEN));
        #endif
    }

    /* Change the duty cycle on the blue channel */
    err = ledc_set_duty(LED_t_SPEED_MODE, LED_t_CHANNEL_BLUE, duty_blue);
    ledc_update_duty(LED_t_SPEED_MODE, LED_t_CHANNEL_BLUE);

    /* Check for errors */
    if (err != ESP_OK) {
        #if LED_DEBUG == 1
            ESP_LOGI(TAG, "\e[1;31;40mWrong parameters were passed when updating the BLUE channel.\e[39;49m");
        #endif
        return LED_i_ERR_INVALID_ARGUMETS;
    } else {
        #if LED_DEBUG == 1
            ESP_LOGI(TAG, "\e[34;40m[BLUE] Duty cycle: %d / 256\e[39;49m", ledc_get_duty(LED_t_SPEED_MODE, LED_t_CHANNEL_BLUE));
        #endif
    }

    return LED_i_OK;
}


/* Exported functions */
/*============================================================================*/
/**
 * @brief Initialized the LED control module. 
 *        Returns LED_i_OK on success.
 * 
 * @return led_err_t 
 */
led_err_t LED_t_init(void) {
    LED_t_controller_status = LED_i_CONTROL_NOT_INITIALIZED;

    /* Configure the different attributes of the timer */
    ledc_timer_config_t ledc_timer = {
        .speed_mode         = LED_t_SPEED_MODE,
        .timer_num          = LED_t_TIMER_NUMB,
        .duty_resolution    = LED_t_DUTY_RESOLUTION,
        .freq_hz            = LED_i_FREQUENCY,
        .clk_cfg            = LED_t_CLOCK,
    };

    /* ESP error checking macro */
    /* ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer)); */

    esp_err_t err = ledc_timer_config(&ledc_timer);
    if (err == ESP_ERR_INVALID_ARG) {
        return LED_i_ERR_INVALID_ARGUMETS;
    } else {
        /* Empty */
    }
    
    /* Configure the 3 channels for each color */
    ledc_channel_config_t ledc_channel_red = {
        .speed_mode         = LED_t_SPEED_MODE,
        .channel            = LED_t_CHANNEL_RED,
        .timer_sel          = LED_t_TIMER_NUMB,
        .intr_type          = LED_t_INTERRUPT,
        .gpio_num           = LED_t_RED_PIN,
        .duty               = LED_i_INIT_DUTY,
        .hpoint             = LED_i_INIT_HPOINT
    };
    ledc_channel_config_t ledc_channel_blue = {
        .speed_mode         = LED_t_SPEED_MODE,
        .channel            = LED_t_CHANNEL_BLUE,
        .timer_sel          = LED_t_TIMER_NUMB,
        .intr_type          = LED_t_INTERRUPT,
        .gpio_num           = LED_t_BLUE_PIN,
        .duty               = LED_i_INIT_DUTY,
        .hpoint             = LED_i_INIT_HPOINT
    };
    ledc_channel_config_t ledc_channel_green = {
        .speed_mode         = LED_t_SPEED_MODE,
        .channel            = LED_t_CHANNEL_GREEN,
        .timer_sel          = LED_t_TIMER_NUMB,
        .intr_type          = LED_t_INTERRUPT,
        .gpio_num           = LED_t_GREEN_PIN,
        .duty               = LED_i_INIT_DUTY,
        .hpoint             = LED_i_INIT_HPOINT
    };

    /* ESP error checking macro */
    /* ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_red)); */
    /* ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_green)); */
    /* ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_blue)); */

    /* Initialize the red channel */
    esp_err_t err_red = ledc_channel_config(&ledc_channel_red);

    /* Check for error in intialization */
    if(err_red != ESP_OK) {
        #if LED_DEBUG == 1
            ESP_LOGI(TAG, "Wrong parameters were passed when initializing the red channel.");
        #endif
        return LED_i_ERR_INVALID_ARGUMETS;
    } else {
        /* Empty */
    }

    /* Initialize the green channel */
    esp_err_t err_green = ledc_channel_config(&ledc_channel_green);

    /* Check for error in intialization */
    if(err_green != ESP_OK) {
        #if LED_DEBUG == 1
            ESP_LOGI(TAG, "Wrong parameters were passed when initializing the green channel.");
        #endif
        return LED_i_ERR_INVALID_ARGUMETS;
    } else {
        /* Empty */
    }

    /* Initialize the blue channel */
    esp_err_t err_blue = ledc_channel_config(&ledc_channel_blue);

    /* Check for error in intialization */
    if(err_blue != ESP_OK) {
        #if LED_DEBUG == 1
            ESP_LOGI(TAG, "Wrong parameters were passed when initializing the blue channel.");
        #endif
        return LED_i_ERR_INVALID_ARGUMETS;
    } else {
        /* Empty */
    }

    LED_t_controller_status = LED_i_CONTROL_INITIALIZED;
    return LED_i_OK;
}

/**
 * @brief Checks the status of the LED control module.
 *        Returns the status of the LED control module.
 * 
 * @return led_err_t 
 */
led_err_t LED_t_status(void){
    switch(LED_t_controller_status) {
        case LED_i_CONTROL_NOT_INITIALIZED:
            return LED_i_STATUS_NOT_INITIALIZED;
            break;
        case LED_i_CONTROL_IN_USE:
            return LED_i_STATUS_UNDER_USE;
            break;
        case LED_i_CONTROL_NOT_IN_USE:
            return LED_i_STATUS_FREE;
            break;
        default:
            return LED_i_STATUS_INITIALIZED;
    }
}

/**
 * @brief Set the color of the RGB LED to green
 * 
 * @return led_err_t 
 */
led_err_t LED_t_setGreen() {
    return LED_t_setColor(LED_i_GREEN);
}
/**
 * @brief Set the color of the RGB LED to red
 * 
 * @return led_err_t 
 */
led_err_t LED_t_setRed() {
    return LED_t_setColor(LED_i_RED);
}
/**
 * @brief Set the color of the RGB LED to blue
 * 
 * @return led_err_t 
 */
led_err_t LED_t_setBlue() {
    return LED_t_setColor(LED_i_BLUE);
}
/**
 * @brief Set the color of the RGB LED to yellow
 * 
 * @return led_err_t 
 */
led_err_t LED_t_setYellow() {
    return LED_t_setColor(LED_i_YELLOW);
}
/**
 * @brief Set the color of the RGB LED to magenta
 * 
 * @return led_err_t 
 */
led_err_t LED_t_setMagenta() {
    return LED_t_setColor(LED_i_MAGENTA);
}
/**
 * @brief Set the color of the RGB LED to cyan
 * 
 * @return led_err_t 
 */
led_err_t LED_t_setCyan() {
    return LED_t_setColor(LED_i_CYAN);
}
/**
 * @brief Set the color of the RGB LED to orange
 * 
 * @return led_err_t 
 */
led_err_t LED_t_setOrange() {
    return LED_t_setColor(LED_i_ORANGE);
}
/**
 * @brief Set the color of the RGB LED to white
 * 
 * @return led_err_t 
 */
led_err_t LED_t_setWhite() {
    return LED_t_setColor(LED_i_WHITE);
}
/**
 * @brief Set the color of the RGB LED to black
 * 
 * @return led_err_t 
 */
led_err_t LED_t_setBlack() {
    return LED_t_setColor(LED_i_BLACK);
}
/**
 * @brief Set the color of the RGB LED to purple
 * 
 * @return led_err_t 
 */
led_err_t LED_t_setPurple() {
    return LED_t_setColor(LED_i_PURPLE);
}


