/**
 * @file RTC_TIME.c
 * @author maria.irimus@altran.com
 * @brief 
 * @version 0.1
 * @date 2021-08-08.
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
 * $Source: RTC_TIME.c $
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
 * $Log: RTC_TIME.c  $
  ============================================================================*/

#ifndef RTC_TIME_C
#define RTC_TIME_C

/* Includes */
/*============================================================================*/
#include "RTC_TIME.h"
#include "RTC_interface.h"
#include "RTC_config.h"

/* Defines */
/*============================================================================*/

/*! Number of time frame bytes */
#define RTC_i_NR_BYTES_TIME_FRAME   (int)       8        

/*! I2C master write */
#define RTC_WRITE_BIT               (int)       0        

/*! I2C master read */
#define RTC_READ_BIT                (int)       1       

/*! I2C master will check ack from slave */
#define RTC_ACK_CHECK_EN            (int)       1        

/*! Get last digit in base10 */
#define RTC_LAST_DIGIT_B10          (int)       10      

/*! Cut the last digit in base10 */
#define RTC_FIRST_DIGITS_B10        (int)       10      

/*! Get last digit in base16 */
#define RTC_LAST_DIGIT_B16          (int)       16       

/*! Cut the last digit in base16 */
#define RTC_FIRST_DIGITS_B16        (int)       16      

/*! Left shift 4 bits */
#define RTC_LEFT_SHIFT_4_BITS       (int)       4

/*! Left shift 1 bit */
#define RTC_LEFT_SHIFT_1_BIT        (int)       1

#define RTC_ZERO                    (int)       0

/*! Resets to zero */
#define RTC_RESET_ZERO              (int)       0

/* Constants and types */
/*============================================================================*/

/* Variables */
/*============================================================================*/

/*! Auxiliary variable for computing a 10 min delay for time refresh */
static int NTP_i_refreshTime = RTC_ZERO; 

/* Private functions prototypes */
/*============================================================================*/

/**
 * @brief Convert normal decimal numbers to binary coded decimal
 * 
 * @param  sb_val representing the decimal to be converted
 * @return char representing the binary converted number
 */
char RTC_sb_decToBcd(char sb_val);

/**
 * @brief Convert binary coded decimal to normal decimal numbers
 * 
 * @param  sb_val representing the binary to be converted
 * @return char representing the decimal converted number
 */
char RTC_sb_bcdToDec(char sb_val);

/**
 * @brief Set time and date data on the DS3231
 * 
 * @param  tm_timeInfo representing the time structure 
 * @return esp_err_t representing error type
 */

/**
 * @brief Set time and date data on the DS3231
 * 
 * @param NTP_t_timeInfo representing the time structure
 * @return rtc_err_t representing the error type
 */
rtc_err_t RTC_t_setDS3231Time(struct tm NTP_t_timeInfo);

/**
 * @brief Get the time from the DS3231
 * 
 * @param ub_data representing the data transmitted by the DS3231
 * @return esp_err_t representing the error type
 */
rtc_err_t RTC_t_getDS3231Time(unsigned char *ub_data);

/* Inline functions */
/*============================================================================*/

/* Private functions */
/*============================================================================*/

char RTC_sb_decToBcd(char sb_val)
{
    char sb_retVal;

    sb_retVal = ((sb_val / RTC_FIRST_DIGITS_B10 << RTC_LEFT_SHIFT_4_BITS) + (sb_val % RTC_LAST_DIGIT_B10));

    return sb_retVal;
}

char RTC_sb_bcdToDec(char sb_val)
{
    char sb_retVal;

    sb_retVal = ((sb_val / RTC_FIRST_DIGITS_B16 * 10) + (sb_val % RTC_LAST_DIGIT_B16));

    return sb_retVal;
}

rtc_err_t RTC_t_setDS3231Time(struct tm NTP_t_timeInfo)
{
    // Status of the task
    rtc_err_t t_err_status = RTC_i_OK;
    esp_err_t t_err = ESP_OK;

    char sb_writeBuf[RTC_i_NR_BYTES_TIME_FRAME] = {
        RTC_ZERO,
        RTC_sb_decToBcd(NTP_t_timeInfo.tm_sec),
        RTC_sb_decToBcd(NTP_t_timeInfo.tm_min),
        RTC_sb_decToBcd(NTP_t_timeInfo.tm_hour),
        RTC_sb_decToBcd(NTP_t_timeInfo.tm_wday),
        RTC_sb_decToBcd(NTP_t_timeInfo.tm_mday),
        RTC_sb_decToBcd(NTP_t_timeInfo.tm_mon),
        RTC_sb_decToBcd(NTP_t_timeInfo.tm_year)};

    // Create and initialize an I2C commands list
    i2c_cmd_handle_t t_cmd = i2c_cmd_link_create();

    if(t_cmd == NULL)
    {
        t_err_status = RTC_i_ERR_CREATE_LINK;
    }
    else
    {
        /* Empty */
    }

    // Populate the command link with the series of data to be sent to the slave:

    // Start bit; queue a “START signal” to the given commands list
    t_err = i2c_master_start(t_cmd);
    if (t_err != ESP_OK) 
    {
        #if RTC_DEBUG == 1
            ESP_LOGI("RTC", "i2c_master_start error in RTC_t_setDS3231Time().");
        #endif
        t_err_status = RTC_i_ERR_INVALID_ARG;
    } 
    else
    {
        /* Empty */
    }

    // A single byte will be sent on the I2C port containing the slave address
    t_err = i2c_master_write_byte(t_cmd, (DS3231_ADDR << RTC_LEFT_SHIFT_1_BIT) | RTC_WRITE_BIT, RTC_ACK_CHECK_EN);
    if (t_err != ESP_OK) 
    {
        #if RTC_DEBUG == 1
            ESP_LOGI("RTC", "i2_master_write_byte error in RTC_t_setDS3231Time().");
        #endif
        t_err_status = RTC_i_ERR_INVALID_ARG;
    } 
    else
    {
        /* Empty */
    }

    // Write data
    int i_index;
    for (i_index = RTC_ZERO; i_index < RTC_i_NR_BYTES_TIME_FRAME; i_index++)
    {
        t_err = i2c_master_write_byte(t_cmd, sb_writeBuf[i_index], RTC_ACK_CHECK_EN);
        if (t_err != ESP_OK) 
        {
            #if RTC_DEBUG == 1
                ESP_LOGI("RTC", "i2_master_write_byte error in RTC_t_setDS3231Time() while writing all the data.");
            #endif
            t_err_status = RTC_i_ERR_INVALID_ARG;
        } 
        else
        {
        /* Empty */
        }
    }

    // Stop bit; queue a “STOP signal” to the given commands list
    t_err = i2c_master_stop(t_cmd);
    if (t_err != ESP_OK) 
    {
        #if RTC_DEBUG == 1
            ESP_LOGI("RTC", "i2_master_stop error in RTC_t_setDS3231Time().");
        #endif
        t_err_status = RTC_i_ERR_INVALID_STATE;
    } 
    else
    {
        /* Empty */
    }

    // Send all queued commands
    // Trigger the execution of the command link by I2C controller
    // Once the execution is triggered, the command link cannot be modified
    t_err = i2c_master_cmd_begin(RTC_ZERO, t_cmd, I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);
    if (t_err != ESP_OK) 
    {
        #if RTC_DEBUG == 1
            ESP_LOGI("RTC", "i2_master_cmd_begin error in RTC_t_setDS3231Time().");
        #endif
        t_err_status = RTC_i_ERR_INVALID_STATE;
    } 
    else
    {
        /* Empty */
    }

    // Free the I2C commands list
    i2c_cmd_link_delete(t_cmd);

    return t_err_status;
}

rtc_err_t RTC_t_getDS3231Time(unsigned char *ub_data)
{
    // Status of the task
    rtc_err_t t_err_status = RTC_i_OK;
    esp_err_t t_err = ESP_OK;

    // Create and initialize an I2C commands list
    i2c_cmd_handle_t t_cmd = i2c_cmd_link_create();

    if(t_cmd == NULL)
    {
        t_err_status = RTC_i_ERR_CREATE_LINK;
    }
    else
    {
        /* Empty */
    }

    // Populate the command link with the series of data to be sent to the slave:

    // Start bit; queue a “START signal” to the given commands list
    t_err = i2c_master_start(t_cmd);
    if (t_err != ESP_OK) 
    {
        #if RTC_DEBUG == 1
            ESP_LOGI("RTC", "i2c_master_start error in RTC_t_getDS3231Time().");
        #endif
        t_err_status = RTC_i_ERR_INVALID_ARG;
    } 
    else
    {
        /* Empty */
    }

    // A single byte will be sent on the I2C port containing the slave address
    t_err = i2c_master_write_byte(t_cmd, (DS3231_ADDR << RTC_LEFT_SHIFT_1_BIT) | RTC_WRITE_BIT, RTC_ACK_CHECK_EN);
    if (t_err != ESP_OK) 
    {
        #if RTC_DEBUG == 1
            ESP_LOGI("RTC", "i2_master_write_byte error in RTC_t_getDS3231Time().");
        #endif
        t_err_status = RTC_i_ERR_INVALID_ARG;
    } 
    else
    {
        /* Empty */
    }
    
    t_err = i2c_master_write_byte(t_cmd, RTC_ZERO, RTC_ACK_CHECK_EN);
    if (t_err != ESP_OK) 
    {
        #if RTC_DEBUG == 1
            ESP_LOGI("RTC", "i2_master_write_byte error in RTC_t_getDS3231Time() when ACK.");
        #endif
        t_err_status = RTC_i_ERR_INVALID_ARG;
    } 
    else
    {
        /* Empty */
    }

    // Stop bit; queue a “STOP signal” to the given commands list
    t_err = i2c_master_stop(t_cmd);
    if (t_err != ESP_OK) 
    {
        #if RTC_DEBUG == 1
            ESP_LOGI("RTC", "i2_master_stop error in RTC_t_getDS3231Time().");
        #endif
        t_err_status = RTC_i_ERR_INVALID_STATE;
    } 
    else
    {
        /* Empty */
    }

    // Send all queued commands
    // Trigger the execution of the command link by I2C controller
    // Once the execution is triggered, the command link cannot be modified
    t_err = i2c_master_cmd_begin(RTC_I2C_PORT_NUMBER, t_cmd, I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);
    if (t_err != ESP_OK) 
    {
        #if RTC_DEBUG == 1
            ESP_LOGI("RTC", "i2_master_cmd_begin error in RTC_t_getDS3231Time().");
        #endif
        t_err_status = RTC_i_ERR_INVALID_STATE;
    } 
    else
    {
        /* Empty */
    }

    // Free the I2C commands list
    i2c_cmd_link_delete(t_cmd);

    // Create a new link
    t_cmd = i2c_cmd_link_create();
    if(t_cmd == NULL)
    {
        t_err_status = RTC_i_ERR_CREATE_LINK;
    }
    else
    {
        /* Empty */
    }

    // Start bit
    t_err = i2c_master_start(t_cmd);
    if (t_err != ESP_OK) 
    {
        #if RTC_DEBUG == 1
            ESP_LOGI("RTC", "i2c_master_start error in RTC_t_getDS3231Time() before reading the data.");
        #endif
        t_err_status = RTC_i_ERR_INVALID_ARG;
    } 
    else
    {
        /* Empty */
    }

    // A single byte will be sent on the I2C port containing the slave address followed by the READ bit
    t_err = i2c_master_write_byte(t_cmd, (DS3231_ADDR << RTC_LEFT_SHIFT_1_BIT) | RTC_READ_BIT, RTC_ACK_CHECK_EN);
    if (t_err != ESP_OK) 
    {
        #if RTC_DEBUG == 1
            ESP_LOGI("RTC", "i2_master_write_byte error in RTC_t_getDS3231Time() when READ_BIT.");
        #endif
        t_err_status = RTC_i_ERR_INVALID_ARG;
    } 
    else
    {
        /* Empty */
    }

    // Read the data
    char ub_dataIndex;
    for (ub_dataIndex = RTC_ZERO; ub_dataIndex < RTC_REGISTERS_NUMBER; ub_dataIndex++)
    {
        t_err = i2c_master_read_byte(t_cmd, ub_data + ub_dataIndex, I2C_MASTER_ACK);
        if (t_err != ESP_OK) 
        {
            #if RTC_DEBUG == 1
                ESP_LOGI("RTC", "i2_master_read_byte error in RTC_t_getDS3231Time() while reading all the data.");
            #endif
            t_err_status = RTC_i_ERR_INVALID_ARG;
        } 
        else
        {
        /* Empty */
        }
    }

    // Set NACK bit after reading all the data
    t_err = i2c_master_read_byte(t_cmd, ub_data + RTC_REGISTERS_NUMBER, I2C_MASTER_NACK);
    if (t_err != ESP_OK) 
    {
        #if RTC_DEBUG == 1
            ESP_LOGI("RTC", "i2_master_read_byte error in RTC_t_getDS3231Time() when NACK.");
        #endif
        t_err_status = RTC_i_ERR_INVALID_ARG;
    } 
    else
    {
        /* Empty */
    }

    // Stop bit
    t_err = i2c_master_stop(t_cmd);
    if (t_err != ESP_OK) 
    {
        #if RTC_DEBUG == 1
            ESP_LOGI("RTC", "i2_master_stop error in RTC_t_getDS3231Time().");
        #endif
        t_err_status = RTC_i_ERR_INVALID_ARG;
    } 
    else
    {
        /* Empty */
    }

    // Send all queued commands
    t_err = i2c_master_cmd_begin(RTC_I2C_PORT_NUMBER, t_cmd, I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);
    if (t_err != ESP_OK) 
    {
        #if RTC_DEBUG == 1
            ESP_LOGI("RTC", "i2_master_cmd_begin error in RTC_t_getDS3231Time().");
        #endif
        t_err_status = RTC_i_ERR_INVALID_STATE;
    } 
    else
    {
        /* Empty */
    }

    // Free the I2C commands list
    i2c_cmd_link_delete(t_cmd);

    return t_err_status;
}

/* Exported Variables */
/*============================================================================*/

/* Exported functions prototypes */
/*============================================================================*/

/**
 * @brief Initialize the I2C master 
 * 
 * @return rtc_err_t representing the error type
 */
rtc_err_t RTC_t_I2C_masterInit(void);

/**
 * @brief Retrieve the time from the DS3231 RTC module and set the internal clock of the microcontroller accordingly
 * 
 * @return rtc_err_t representing the error type
 */
rtc_err_t RTC_t_getTime(void);

/**
 * @brief Sync the current system time with RTC time
 * 
 * @return rtc_err_t representing the error type
 */
rtc_err_t RTC_t_updateTime(void);

/* Exported functions */
/*============================================================================*/

rtc_err_t RTC_t_I2C_masterInit(void)
{
    // Status of the task
    rtc_err_t t_err_status = RTC_i_OK;
    esp_err_t t_err = ESP_OK;

    int i_i2c_master_port;
    i_i2c_master_port = I2C_MASTER_NUM;

    // I2C configuration
    i2c_config_t t_I2C_config = {
        .mode = I2C_MODE_MASTER,                // Set mode
        .sda_io_num = I2C_MASTER_SDA_IO,        // Select GPIO specific to your project
        .scl_io_num = I2C_MASTER_SCL_IO,        // Select GPIO specific to your project
        .sda_pullup_en = GPIO_PULLUP_ENABLE,    // Enable pullup for data
        .scl_pullup_en = GPIO_PULLUP_ENABLE,    // Enable pullup for clock
        .master.clk_speed = I2C_MASTER_FREQ_HZ, // Select frequency specific to your project
    };

    // Set the configuration for the i2c driver
    t_err = i2c_param_config(i_i2c_master_port, &t_I2C_config);
    if (t_err != ESP_OK) 
    {
        #if RTC_DEBUG == 1
            ESP_LOGI("RTC", "i2c_param_config error.");
        #endif
        t_err_status = RTC_i_ERR_INVALID_ARG;
    } 
    else
    {
        /* Empty */
    }

    // Install the i2c driver with the previously set configuration
    t_err = i2c_driver_install(i_i2c_master_port, t_I2C_config.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, RTC_INTERRUPT_FLAG);
    if (t_err != ESP_OK) 
    {
        #if RTC_DEBUG == 1
            ESP_LOGI("RTC", "i2c_driver_install error.");
        #endif
        t_err_status = RTC_i_ERR_INVALID_ARG;
    } 
    else
    {
        /* Empty */
    }

    return t_err_status;
}

rtc_err_t RTC_t_getTime(void)
{
    // Status of the task
    rtc_err_t t_err_status = RTC_i_OK;

    // Count the number of calls of this function to compute the time delay
    NTP_i_refreshTime++;

    // Verify if the counter exceeds the expected value (the given time delay has elapsed)
    if (NTP_i_refreshTime > RTC_TIME_REFRESH_DELAY_COUNT)
    {
        // Reset the delay counter
        NTP_i_refreshTime = RTC_RESET_ZERO;

        // Structure for storing the time information received from the DS3231
        struct tm NTP_t_timeInfo;

        // Initialize the time data buffer
        unsigned char ub_timeDataBuffer[RTC_i_NR_BYTES_TIME_FRAME - 1] = {
            RTC_ZERO,
            RTC_ZERO,
            RTC_ZERO,
            RTC_ZERO,
            RTC_ZERO,
            RTC_ZERO,
            RTC_ZERO};

        // Get the time from the DS3231
        t_err_status = RTC_t_getDS3231Time(ub_timeDataBuffer);

        #if RTC_DEBUG == 1
            ESP_LOGI("RTC","Err: %d",t_err_status);
        #endif

        // Set the timeInfo struct with the data from the time buffer
        NTP_t_timeInfo.tm_sec  = RTC_sb_bcdToDec(ub_timeDataBuffer[0]);
        NTP_t_timeInfo.tm_min  = RTC_sb_bcdToDec(ub_timeDataBuffer[1]);
        NTP_t_timeInfo.tm_hour = RTC_sb_bcdToDec(ub_timeDataBuffer[2]);
        NTP_t_timeInfo.tm_wday = RTC_sb_bcdToDec(ub_timeDataBuffer[3]);
        NTP_t_timeInfo.tm_mday = RTC_sb_bcdToDec(ub_timeDataBuffer[4]);
        NTP_t_timeInfo.tm_mon  = RTC_sb_bcdToDec(ub_timeDataBuffer[5]);
        NTP_t_timeInfo.tm_year = RTC_sb_bcdToDec(ub_timeDataBuffer[6]);

        // Set the daylight saving
        NTP_t_timeInfo.tm_isdst = RTC_DAYLIGHT_OFFSET;

        // Convert the time from the time info structure to a time_t variable
        time_t t_time_to_seconds = mktime(&NTP_t_timeInfo);

        if (t_time_to_seconds == -1)
        {
            t_err_status = RTC_i_ERR_CONVERT_TIME;
        }
        else
        {
            /* Empty */
        }

        // Convert the time_t variable to unix time
        struct timeval t_now = {.tv_sec = t_time_to_seconds};

        // Set the internal system time
        settimeofday(&t_now, NULL);

        // Buffer for storing the formated time string
        char sb_timeBuf[64];

        // Format the string buffer for displaying the date and time
        strftime(sb_timeBuf, sizeof(sb_timeBuf), "%c", &NTP_t_timeInfo);

#if RTC_DEBUG == 1
        // Print the current system time on the serial monitor
        ESP_LOGI("RTC", "The current date/time in Cluj is: %s", sb_timeBuf);
#endif
    }

    return t_err_status;
}

rtc_err_t RTC_t_updateTime(void)
{
    // Local variables for storing the time
    struct timeval NTP_t_now;
    struct tm NTP_t_timeInfo;

    // Status of the task
    rtc_err_t t_err_status = RTC_i_OK;

    // Get the current system time
    gettimeofday(&NTP_t_now, NULL);

    // Format the current system time
    localtime_r(&NTP_t_now.tv_sec, &NTP_t_timeInfo);

    // Set the time in the DS3231 RTC module
    t_err_status = RTC_t_setDS3231Time(NTP_t_timeInfo);

    return t_err_status;
}

#endif