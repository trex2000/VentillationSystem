/**
 * @file ErrorManagement.c
 * @author tudor.coroian@altran.com
 *         maria.irimus@altran.com
 * @brief 
 * @version 0.1
 * @date 2021-08-27
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
 * $Source: ErrorManagement.c $
 * $Revision: 1 $
 * $Author: Tudor Coroian $
 *          Maria Irimus  $
 * $Date: 2021.08.27 $
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
 * $Log: ErrorManagement.c  $
  ============================================================================*/

/* Includes */
/*============================================================================*/
#include "ErrorManagement.h"

/* Defines */
/*============================================================================*/
#define ERM_i_NUMBER_ERRORS     (int)   8       ///< Size of the error vector
#define ERM_i_NUMBER_WARNINGS   (int)   4       ///< Size of the warning vector

/* Constants and types  */
/*============================================================================*/
/**
 * @brief Indexes for every type of error/warning * 
 */
typedef enum
{
    /* Errors */
    ERM_i_TAG_TEMP_ERROR_1 =  0,    ///< Error index for TEMP1
    ERM_i_TAG_HUMI_ERROR_1,         ///< Error index for HUMI1
    ERM_i_TAG_WIFI_ERROR_2,         ///< Error index for WIFI2
    ERM_i_TAG__NTP_ERROR_1,         ///< Error index for _NTP1
    ERM_i_TAG__RTC_ERROR_1,         ///< Error index for _RTC1
    ERM_i_TAG__FAN_ERROR_1,         ///< Error index for _FAN1
    ERM_i_TAG__FAN_ERROR_2,         ///< Error index for _FAN2
    ERM_i_TAG__FAN_ERROR_3,         ///< Error index for _FAN3

    /* Warnings */
    ERM_i_TAG_TEMP_WARNING_2 = 16, ///< Warning index for TEMP2
    ERM_i_TAG_TEMP_WARNING_3,      ///< Warning index for TEMP3
    ERM_i_TAG_HUMI_WARNING_2,      ///< Warning index for HUMI2
    ERM_i_TAG_WIFI_WARNING_1,      ///< Warning index for WIFI1
} erm_tag_t;

/* Variables */
/*============================================================================*/
static uint16_t ERM_i_ERRORS    = 0xFFFF;           ///< Bitfield for errors
static uint16_t ERM_i_WARNINGS  = 0xFFFF;           ///< Bitfield for warnings

/**
 * @brief Vector used to store information about errors
 * 
 */
static erm_errorInfo_t ERM_at_errorLog[16] = {
    {
        .ac_sender = "TEMP",
        .ac_number = "001",
        .i_priority = 7,
        .ac_msg = "Temperature sensors are not connected on the One Wire Bus.",
        .i_timeHold = 10
    },
    {
        .ac_sender = "HUMI",
        .ac_number = "001",
        .i_priority = 8,
        .ac_msg = "The humidity sensor was not initialized successfully.",
        .i_timeHold = 10
    },
    {
        .ac_sender = "WIFI",
        .ac_number = "002",
        .i_priority = 9,
        .ac_msg = "ESP32 failed to create an AP.",
        .i_timeHold = 10
    },
    {
        .ac_sender = "_NTP",
        .ac_number = "001",
        .i_priority = 10,
        .ac_msg = "The NTP server connection failed.",
        .i_timeHold = 20
    },
    {
        .ac_sender = "_RTC",
        .ac_number = "001",
        .i_priority = 11,
        .ac_msg = "The time received is not correct.",
        .i_timeHold = 20
    },
    {
        .ac_sender = "_FAN",
        .ac_number = "001",
        .i_priority = 4,
        .ac_msg = "Fan module has not been initialized.",
        .i_timeHold = 40
    },
    {
        .ac_sender = "_FAN",
        .ac_number = "002",
        .i_priority = 5,
        .ac_msg = "Fan inside->outside speed error.",
        .i_timeHold = 40
    },
    {
        .ac_sender = "_FAN",
        .ac_number = "003",
        .i_priority = 6,
        .ac_msg = "Fan outside->inside speed error.",
        .i_timeHold = 40
    }
    };

/**
 * @brief Vector used to store information about warnings
 * 
 */
static erm_errorInfo_t ERM_at_warningLog[16] = {
    {
        .ac_sender = "TEMP",
        .ac_number = "002",
        .i_priority = 12,
        .ac_msg = "Temperature sensor is malfunctioning.",
        .i_timeHold = 20
    },
    {
        .ac_sender = "TEMP",
        .ac_number = "003",
        .i_priority = 14,
        .ac_msg = "Read temperature is outside the given interval.",
        .i_timeHold = 40
    },
    {
        .ac_sender = "HUMI",
        .ac_number = "002",
        .i_priority = 13,
        .ac_msg = "The humidity could not be read correctly by the sensor.",
        .i_timeHold = 20
    },
    {
        .ac_sender = "WIFI",
        .ac_number = "001",
        .i_priority = 15,
        .ac_msg = "ESP32 is not connected to a network.",
        .i_timeHold = 40
    }
    };

/* Private functions prototypes */
/*============================================================================*/

/* Inline functions */
/*============================================================================*/

/* Private functions */
/*============================================================================*/
/**
 * @brief Logs errors when they appear
 * 
 * @param tag representing the error index for modules
 * @return erm_err_t representing the error type
 */
erm_err_t ERM_t_logError(erm_tag_t tag)
{
    switch (tag)
    {
    case ERM_i_TAG_TEMP_ERROR_1:;
        /* Log TEMP1 error using specific mask */
        ERM_i_ERRORS |= ERM_i_TEMP_ERROR_1_MASK;
        break;
    case ERM_i_TAG_HUMI_ERROR_1:;
        /* Log HUMI1 error using specific mask */
        ERM_i_ERRORS |= ERM_i_HUMI_ERROR_1_MASK;
        break;
    case ERM_i_TAG_WIFI_ERROR_2:;
        /* Log WIFI2 error using specific mask */
        ERM_i_ERRORS |= ERM_i_WIFI_ERROR_2_MASK;
        break;
    case ERM_i_TAG__NTP_ERROR_1:;
        /* Log _NTP1 error using specific mask */
        ERM_i_ERRORS |= ERM_i__NTP_ERROR_1_MASK;
        break;
    case ERM_i_TAG__RTC_ERROR_1:;
        /* Log _RTC1 error using specific mask */
        ERM_i_ERRORS |= ERM_i__RTC_ERROR_1_MASK;
        break;
    case ERM_i_TAG__FAN_ERROR_1:;
        /* Log _FAN1 error using specific mask */
        ERM_i_ERRORS |= ERM_i_TAG__FAN_ERROR_1;
        break;
    case ERM_i_TAG__FAN_ERROR_2:;
        /* Log _FAN2 error using specific mask */
        ERM_i_ERRORS |= ERM_i_TAG__FAN_ERROR_2;
        break;
    case ERM_i_TAG__FAN_ERROR_3:;
        /* Log _FAN3 error using specific mask */
        ERM_i_ERRORS |= ERM_i_TAG__FAN_ERROR_3;
        break;
    default:;
    }
    return ERM_i_OK;
}

/**
 * @brief Delogs errors after being solved
 * 
 * @param tag  representing the error index for modules
 * @return erm_err_t representing the error type
 */
erm_err_t ERM_t_delogError(erm_tag_t tag)
{
    switch (tag)
    {
    case ERM_i_TAG_TEMP_ERROR_1:;
        /* Delog TEMP1 error using specific mask */
        ERM_i_ERRORS &= ~ERM_i_TEMP_ERROR_1_MASK;

        /* Reset the time hold */
        ERM_at_errorLog[ERM_i_TAG_TEMP_ERROR_1].i_timeHold = 5;

        break;
    case ERM_i_TAG_HUMI_ERROR_1:;
        /* Delog HUMI1 error using specific mask */
        ERM_i_ERRORS &= ~ERM_i_HUMI_ERROR_1_MASK;

        /* Reset the time hold */
        ERM_at_errorLog[ERM_i_TAG_HUMI_ERROR_1].i_timeHold = 5;
        
        break;
    case ERM_i_TAG_WIFI_ERROR_2:;
        /* Delog WIFI2 error using specific mask */
        ERM_i_ERRORS &= ~ERM_i_WIFI_ERROR_2_MASK;

        /* Reset the time hold */
        ERM_at_errorLog[ERM_i_TAG_WIFI_ERROR_2].i_timeHold = 5;
        
        break;
    case ERM_i_TAG__NTP_ERROR_1:;
        /* Delog _NTP1 error using specific mask */
        ERM_i_ERRORS &= ~ERM_i__NTP_ERROR_1_MASK;

        /* Reset the time hold */
        ERM_at_errorLog[ERM_i_TAG__NTP_ERROR_1].i_timeHold = 10;
        
        break;
    case ERM_i_TAG__RTC_ERROR_1:;
        /* Delog _RTC1 error using specific mask */
        ERM_i_ERRORS &= ~ERM_i__RTC_ERROR_1_MASK;

        /* Reset the time hold */
        ERM_at_errorLog[ERM_i_TAG__RTC_ERROR_1].i_timeHold = 10;
        
        break;
    case ERM_i_TAG__FAN_ERROR_1:;
        /* Delog _FAN1 error using specific mask */
        ERM_i_ERRORS &= ~ERM_i__FAN_ERROR_1_MASK;

        /* Reset the time hold */
        ERM_at_errorLog[ERM_i_TAG__FAN_ERROR_1].i_timeHold = 20;
        
        break;
    case ERM_i_TAG__FAN_ERROR_2:;
        /* Delog _FAN2 error using specific mask */
        ERM_i_ERRORS &= ~ERM_i__FAN_ERROR_2_MASK;

        /* Reset the time hold */
        ERM_at_errorLog[ERM_i_TAG__FAN_ERROR_2].i_timeHold = 20;
        
        break;
    case ERM_i_TAG__FAN_ERROR_3:;
        /* Delog _FAN3 error using specific mask */
        ERM_i_ERRORS &= ~ERM_i__FAN_ERROR_3_MASK;

        /* Reset the time hold */
        ERM_at_errorLog[ERM_i_TAG__FAN_ERROR_3].i_timeHold = 20;
        
        break;
    default:;
    }
    return ERM_i_OK;
}

/**
 * @brief Log warnings when they appear
 * 
 * @param tag  representing the error index for modules
 * @return erm_err_t representing the error type
 */
erm_err_t ERM_t_logWarning(erm_tag_t tag)
{
    switch (tag)
    {
    case ERM_i_TAG_TEMP_WARNING_2:;
        /* Log TEMP2 warning using specific mask */
        ERM_i_WARNINGS |= ERM_i_TEMP_WARNING_2_MASK;
        break;
    case ERM_i_TAG_TEMP_WARNING_3:;
        /* Log TEMP3 warning using specific mask */
        ERM_i_WARNINGS |= ERM_i_TEMP_WARNING_3_MASK;
        break;
    case ERM_i_TAG_HUMI_WARNING_2:;
        /* Log HUMI2 warning using specific mask */
        ERM_i_WARNINGS |= ERM_i_HUMI_WARNING_2_MASK;
        break;
    case ERM_i_TAG_WIFI_WARNING_1:;
        /* Log WIFI1 warning using specific mask */
        ERM_i_WARNINGS |= ERM_i_WIFI_WARNING_1_MASK;
        break;
    default:;
    }
    return ERM_i_OK;
}

/**
 * @brief Delog warnings after being solved
 * 
 * @param tag  representing the error index for modules
 * @return erm_err_t representing the error type
 */
erm_err_t ERM_t_delogWarning(erm_tag_t tag)
{
    switch (tag)
    {
    case ERM_i_TAG_TEMP_WARNING_2:;
        /* Delog TEMP2 warning using specific mask */
        ERM_i_WARNINGS &= ~ERM_i_TEMP_WARNING_2_MASK;

        /* Reset the time hold */
        ERM_at_warningLog[ERM_i_TAG_TEMP_WARNING_2].i_timeHold = 10;

        break;
    case ERM_i_TAG_TEMP_WARNING_3:;
        /* Delog TEMP3 warning using specific mask */
        ERM_i_WARNINGS &= ~ERM_i_TEMP_WARNING_3_MASK;

        /* Reset the time hold */
        ERM_at_warningLog[ERM_i_TAG_TEMP_WARNING_3].i_timeHold = 20;
        
        break;
    case ERM_i_TAG_HUMI_WARNING_2:;
        /* Delog HUMI2 warning using specific mask */
        ERM_i_WARNINGS &= ~ERM_i_HUMI_WARNING_2_MASK;

        /* Reset the time hold */
        ERM_at_warningLog[ERM_i_TAG_HUMI_WARNING_2].i_timeHold = 10;
        
        break;
    case ERM_i_TAG_WIFI_WARNING_1:;
        /* Delog WIFI1 warning using specific mask */
        ERM_i_WARNINGS &= ~ERM_i_WIFI_WARNING_1_MASK;

        /* Reset the time hold */
        ERM_at_warningLog[ERM_i_TAG_WIFI_WARNING_1].i_timeHold = 20;
        
        break;
    default:;
    }

    return ERM_i_OK;
}

/* Exported functions */
/*============================================================================*/

/**
 * @brief Display errors and warnings on Home Assistant as messages.
 * Returns the list of errors and warnings, 
 * "The system works correctly" otherwise
 * 
 * @return erm_err_t representing the error type
 */
erm_err_t ERM_t_displayErrorsOnHomeAssistant()
{
    /* Hold the message to be displayed on the Home Assistent */
    char ac_buffer[2048];
    strcpy(ac_buffer, "\"");

    /* Check if the time hold for the error expired */
    if (ERM_at_errorLog[ERM_i_TAG__FAN_ERROR_1].i_timeHold == 0) {
        strcat(ac_buffer, ERM_at_errorLog[ERM_i_TAG__FAN_ERROR_1].ac_msg);
        strcat(ac_buffer, "\"\0");
    } else {
        if (ERM_at_errorLog[ERM_i_TAG__FAN_ERROR_2].i_timeHold == 0) {
            strcat(ac_buffer, ERM_at_errorLog[ERM_i_TAG__FAN_ERROR_2].ac_msg);
            strcat(ac_buffer, "\"\0");
        } else {
            if (ERM_at_errorLog[ERM_i_TAG__FAN_ERROR_3].i_timeHold == 0) {
                strcat(ac_buffer, ERM_at_errorLog[ERM_i_TAG__FAN_ERROR_3].ac_msg);
                strcat(ac_buffer, "\"\0");
            } else {
                strcpy(ac_buffer, "\"Everything works good!\"\0");
            }
        }
    }
    #if ERM_DEBUG == TRUE
        ESP_LOGI("ERM", "Payload  %s", ac_buffer);
    #endif
    MQTT_v_SendErrorsFan(ac_buffer);

    /* Reset the buffer */
    strcpy(ac_buffer, "\"");

    /* Check if the time hold for the error expired */
    if (ERM_at_errorLog[ERM_i_TAG_TEMP_ERROR_1].i_timeHold == 0) {
        strcat(ac_buffer, ERM_at_errorLog[ERM_i_TAG_TEMP_ERROR_1].ac_msg);
        strcat(ac_buffer, "\"\0");
    } else {
        if (ERM_at_errorLog[ERM_i_TAG_TEMP_WARNING_2].i_timeHold == 0) {
            strcat(ac_buffer, ERM_at_errorLog[ERM_i_TAG_TEMP_WARNING_2].ac_msg);
            strcat(ac_buffer, "\"\0");
        } else {
            if (ERM_at_errorLog[ERM_i_TAG_TEMP_WARNING_3].i_timeHold == 0) {
                strcat(ac_buffer, ERM_at_errorLog[ERM_i_TAG_TEMP_WARNING_3].ac_msg);
                strcat(ac_buffer, "\"\0");
            } else {
                strcpy(ac_buffer, "\"Everything works good!\"\0");
            }
        }
    }
    #if ERM_DEBUG == TRUE
        ESP_LOGI("ERM", "Payload  %s", ac_buffer);
    #endif
    MQTT_v_SendErrorsTemp(ac_buffer);

    /* Reset the buffer */
    strcpy(ac_buffer, "\"");

    /* Check if the time hold for the error expired */
    if (ERM_at_errorLog[ERM_i_TAG_HUMI_ERROR_1].i_timeHold == 0) {
        strcat(ac_buffer, ERM_at_errorLog[ERM_i_TAG_HUMI_ERROR_1].ac_msg);
        strcat(ac_buffer, "\"\0");
    } else {
        if (ERM_at_errorLog[ERM_i_TAG_HUMI_WARNING_2].i_timeHold == 0) {
            strcat(ac_buffer, ERM_at_errorLog[ERM_i_TAG_HUMI_WARNING_2].ac_msg);
            strcat(ac_buffer, "\"\0");
        } else {
            strcpy(ac_buffer, "\"Everything works good!\"\0");
        }
    }
    #if ERM_DEBUG == TRUE
        ESP_LOGI("ERM", "Payload  %s", ac_buffer);
    #endif
    MQTT_v_SendErrorsHumi(ac_buffer);

    /* Reset the buffer */
    strcpy(ac_buffer, "\"");

    /* Check if the time hold for the error expired */
    if (ERM_at_errorLog[ERM_i_TAG__NTP_ERROR_1].i_timeHold == 0) {
        strcat(ac_buffer, ERM_at_errorLog[ERM_i_TAG__NTP_ERROR_1].ac_msg);
        strcat(ac_buffer, "\"\0");
    } else {
        if (ERM_at_errorLog[ERM_i_TAG__RTC_ERROR_1].i_timeHold == 0) {
            strcat(ac_buffer, ERM_at_errorLog[ERM_i_TAG__RTC_ERROR_1].ac_msg);
            strcat(ac_buffer, "\"\0");
        } else {
            strcpy(ac_buffer, "\"Everything works good!\"\0");
        }
    }
    #if ERM_DEBUG == TRUE
        ESP_LOGI("ERM", "Payload  %s", ac_buffer);
    #endif
    MQTT_v_SendErrorsTime(ac_buffer);

    /* Reset the buffer */
    strcpy(ac_buffer, "\"");

    /* Check if the time hold for the error expired */
    if (ERM_at_errorLog[ERM_i_TAG_WIFI_ERROR_2].i_timeHold == 0) {
        strcat(ac_buffer, ERM_at_errorLog[ERM_i_TAG_WIFI_ERROR_2].ac_msg);
        strcat(ac_buffer, "\"\0");
    } else {
        if (ERM_at_errorLog[ERM_i_TAG_WIFI_WARNING_1].i_timeHold == 0) {
            strcat(ac_buffer, ERM_at_errorLog[ERM_i_TAG_WIFI_WARNING_1].ac_msg);
            strcat(ac_buffer, "\"\0");
        } else {
            strcpy(ac_buffer, "\"Everything works good!\"\0");
        }
    }
    #if ERM_DEBUG == TRUE
        ESP_LOGI("ERM", "Payload  %s", ac_buffer);
    #endif
    MQTT_v_SendErrorsWifi(ac_buffer);

    return ERM_i_OK;
}

/**
 * @brief Display errors and warnings on Webserver as messages
 * 
 * @param puc_buffer representing the message displayed on Webserver: 
 * the list of errors and warnings
 * @return erm_err_t representing the error type
 */
erm_err_t ERM_t_displayErrorsOnWebserver(char *puc_buffer)
{
    if (ERM_i_ERRORS != 0)
    {
        int i;
        for (i = 0; i < ERM_i_NUMBER_ERRORS; i++)
        {
            if (ERM_at_errorLog[i].i_timeHold == 0)
            {
                strcat(puc_buffer, ERM_at_errorLog[i].ac_sender);
                strcat(puc_buffer, ERM_at_errorLog[i].ac_number);
                strcat(puc_buffer, ": ");
                strcat(puc_buffer, ERM_at_errorLog[i].ac_msg);
                strcat(puc_buffer, "<br>");
            } else {
                /* Empty */
            }
        }
    }
    else
    {
        /* Empty */
    }

    strcat(puc_buffer, "<br>Warnings<br>");

    if (ERM_i_WARNINGS != 0)
    {
        int j;
        for (j = 0; j < ERM_i_NUMBER_WARNINGS; j++)
        {
            if (ERM_at_warningLog[j].i_timeHold == 0)
            {
                strcat(puc_buffer, ERM_at_warningLog[j].ac_sender);
                strcat(puc_buffer, ERM_at_warningLog[j].ac_number);
                strcat(puc_buffer, ": ");
                strcat(puc_buffer, ERM_at_warningLog[j].ac_msg);
                strcat(puc_buffer, "<br>");
            } else {
                /* Empty */
            }
        }
    }
    else
    {
        /* Empty */
    }
    return ERM_i_OK;
}

/**
 * @brief Display errors and warnings on led as different colors 
 * specific for each error and warning
 * 
 * @return erm_err_t representing the error type
 */
erm_err_t ERM_t_displayErrorsOnLed()
{

    /* Variable used to store the current priority */
    int i_priority = 100;

    /* Variable used to store the index of the highest priority error*/
    int i_index = -1;

    /* Check if there are any errors */
    if (ERM_i_ERRORS != 0)
    {
        /* Check if the error is active */
        if ((ERM_i_TEMP_ERROR_1_MASK & ERM_i_ERRORS) == ERM_i_TEMP_ERROR_1_MASK)
        {
            /* Check if the time hold expired */
            if (ERM_at_errorLog[ERM_i_TAG_TEMP_ERROR_1].i_timeHold == 0)
            {
                /* Check the current priority level */
                if (i_priority > ERM_at_errorLog[ERM_i_TAG_TEMP_ERROR_1].i_priority)
                {
                    /* Set the current priority level and the error index */
                    i_priority = ERM_at_errorLog[ERM_i_TAG_TEMP_ERROR_1].i_priority;
                    i_index = ERM_i_TAG_TEMP_ERROR_1;
                }
                else
                {
                    /* Empty */
                }
            }
            else
            {
                /* Decrement the time hold */
                ERM_at_errorLog[ERM_i_TAG_TEMP_ERROR_1].i_timeHold--;
            }
        }
        else
        {
            /* Empty */
        }

        if ((ERM_i_HUMI_ERROR_1_MASK & ERM_i_ERRORS) == ERM_i_HUMI_ERROR_1_MASK)
        {
            /* Check if the time hold expired */
            if (ERM_at_errorLog[ERM_i_TAG_HUMI_ERROR_1].i_timeHold == 0)
            {
                /* Check the current priority level */
                if (i_priority > ERM_at_errorLog[ERM_i_TAG_HUMI_ERROR_1].i_priority)
                {
                    /* Set the current priority level and the error index */
                    i_priority = ERM_at_errorLog[ERM_i_TAG_HUMI_ERROR_1].i_priority;
                    i_index = ERM_i_TAG_HUMI_ERROR_1;
                }
                else
                {
                    /* Empty */
                }
            }
            else
            {
                /* Decrement the time hold */
                ERM_at_errorLog[ERM_i_TAG_HUMI_ERROR_1].i_timeHold--;
            }
        }
        else
        {
            /* Empty */
        }

        if ((ERM_i_WIFI_ERROR_2_MASK & ERM_i_ERRORS) == ERM_i_WIFI_ERROR_2_MASK)
        {
            /* Check if the time hold expired */
            if (ERM_at_errorLog[ERM_i_TAG_WIFI_ERROR_2].i_timeHold == 0)
            {
                /* Check the current priority level */
                if (i_priority > ERM_at_errorLog[ERM_i_TAG_WIFI_ERROR_2].i_priority)
                {
                    /* Set the current priority level and the error index */
                    i_priority = ERM_at_errorLog[ERM_i_TAG_WIFI_ERROR_2].i_priority;
                    i_index = ERM_i_TAG_WIFI_ERROR_2;
                }
                else
                {
                    /* Empty */
                }
            }
            else
            {
                /* Decrement the time hold */
                ERM_at_errorLog[ERM_i_TAG_WIFI_ERROR_2].i_timeHold--;
            }
        }
        else
        {
            /* Empty */
        }

        if ((ERM_i__NTP_ERROR_1_MASK & ERM_i_ERRORS) == ERM_i__NTP_ERROR_1_MASK)
        {
            /* Check if the time hold expired */
            if (ERM_at_errorLog[ERM_i_TAG__NTP_ERROR_1].i_timeHold == 0)
            {
                /* Check the current priority level */
                if (i_priority > ERM_at_errorLog[ERM_i_TAG__NTP_ERROR_1].i_priority)
                {
                    /* Set the current priority level and the error index */
                    i_priority = ERM_at_errorLog[ERM_i_TAG__NTP_ERROR_1].i_priority;
                    i_index = ERM_i_TAG__NTP_ERROR_1;
                }
                else
                {
                    /* Empty */
                }
            }
            else
            {
                /* Decrement the time hold */
                ERM_at_errorLog[ERM_i_TAG__NTP_ERROR_1].i_timeHold--;
            }
        }
        else
        {
            /* Empty */
        }

        if ((ERM_i__RTC_ERROR_1_MASK & ERM_i_ERRORS) == ERM_i__RTC_ERROR_1_MASK)
        {
            /* Check if the time hold expired */
            if (ERM_at_errorLog[ERM_i_TAG__RTC_ERROR_1].i_timeHold == 0)
            {
                /* Check the current priority level */
                if (i_priority > ERM_at_errorLog[ERM_i_TAG__RTC_ERROR_1].i_priority)
                {
                    /* Set the current priority level and the error index */
                    i_priority = ERM_at_errorLog[ERM_i_TAG__RTC_ERROR_1].i_priority;
                    i_index = ERM_i_TAG__RTC_ERROR_1;
                }
                else
                {
                    /* Empty */
                }
            }
            else
            {
                /* Decrement the time hold */
                ERM_at_errorLog[ERM_i_TAG__RTC_ERROR_1].i_timeHold--;
            }
        }
        else
        {
            /* Empty */
        }

        if ((ERM_i__FAN_ERROR_1_MASK & ERM_i_ERRORS) == ERM_i__FAN_ERROR_1_MASK)
        {
            /* Check if the time hold expired */
            if (ERM_at_errorLog[ERM_i_TAG__FAN_ERROR_1].i_timeHold == 0)
            {
                /* Check the current priority level */
                if (i_priority > ERM_at_errorLog[ERM_i_TAG__FAN_ERROR_1].i_priority)
                {
                    /* Set the current priority level and the error index */
                    i_priority = ERM_at_errorLog[ERM_i_TAG__FAN_ERROR_1].i_priority;
                    i_index = ERM_i_TAG__FAN_ERROR_1;
                }
                else
                {
                    /* Empty */
                }
            }
            else
            {
                /* Decrement the time hold */
                ERM_at_errorLog[ERM_i_TAG__FAN_ERROR_1].i_timeHold--;
            }
        }
        else
        {
            /* Empty */
        }

        if ((ERM_i__FAN_ERROR_2_MASK & ERM_i_ERRORS) == ERM_i__FAN_ERROR_2_MASK)
        {
            /* Check if the time hold expired */
            if (ERM_at_errorLog[ERM_i_TAG__FAN_ERROR_2].i_timeHold == 0)
            {
                /* Check the current priority level */
                if (i_priority > ERM_at_errorLog[ERM_i_TAG__FAN_ERROR_2].i_priority)
                {
                    /* Set the current priority level and the error index */
                    i_priority = ERM_at_errorLog[ERM_i_TAG__FAN_ERROR_2].i_priority;
                    i_index = ERM_i_TAG__FAN_ERROR_2;
                }
                else
                {
                    /* Empty */
                }
            }
            else
            {
                /* Decrement the time hold */
                ERM_at_errorLog[ERM_i_TAG__FAN_ERROR_2].i_timeHold--;
            }
        }
        else
        {
            /* Empty */
        }

        if ((ERM_i__FAN_ERROR_3_MASK & ERM_i_ERRORS) == ERM_i__FAN_ERROR_3_MASK)
        {
            /* Check if the time hold expired */
            if (ERM_at_errorLog[ERM_i_TAG__FAN_ERROR_3].i_timeHold == 0)
            {
                /* Check the current priority level */
                if (i_priority > ERM_at_errorLog[ERM_i_TAG__FAN_ERROR_3].i_priority)
                {
                    /* Set the current priority level and the error index */
                    i_priority = ERM_at_errorLog[ERM_i_TAG__FAN_ERROR_3].i_priority;
                    i_index = ERM_i_TAG__FAN_ERROR_3;
                }
                else
                {
                    /* Empty */
                }
            }
            else
            {
                /* Decrement the time hold */
                ERM_at_errorLog[ERM_i_TAG__FAN_ERROR_3].i_timeHold--;
            }
        }
        else
        {
            /* Empty */
        }
    }

    if (ERM_i_WARNINGS != 0)
    {
        if ((ERM_i_TEMP_WARNING_2_MASK & ERM_i_WARNINGS) == ERM_i_TEMP_WARNING_2_MASK)
        {
            if ((ERM_i_TEMP_WARNING_2_MASK & ERM_i_WARNINGS) == ERM_i_TEMP_WARNING_2_MASK)
            {
                /* Check if the time hold expired */
                if (ERM_at_warningLog[ERM_i_TAG_TEMP_WARNING_2 % 16].i_timeHold == 0)
                {
                    /* Check the current priority level */
                    if (i_priority > ERM_at_warningLog[ERM_i_TAG_TEMP_WARNING_2 % 16].i_priority)
                    {
                        /* Set the current priority level and the error index */
                        i_priority = ERM_at_warningLog[ERM_i_TAG_TEMP_WARNING_2 % 16].i_priority;
                        i_index = ERM_i_TAG_TEMP_WARNING_2;
                    } 
                    else
                    {
                        /* Empty */
                    }
                }
                else
                {
                    /* Decrement the time hold */
                    ERM_at_warningLog[ERM_i_TAG_TEMP_WARNING_2 % 16].i_timeHold--;
                }
            }
            else
            {
                /* Empty */
            }
        }
        else
        {
            /* Empty */
        }

        if ((ERM_i_TEMP_WARNING_3_MASK & ERM_i_WARNINGS) == ERM_i_TEMP_WARNING_3_MASK)
        {
            if ((ERM_i_TEMP_WARNING_3_MASK & ERM_i_WARNINGS) == ERM_i_TEMP_WARNING_3_MASK)
            {
                /* Check if the time hold expired */
                if (ERM_at_warningLog[ERM_i_TAG_TEMP_WARNING_3 % 16].i_timeHold == 0)
                {
                    /* Check the current priority level */
                    if (i_priority > ERM_at_warningLog[ERM_i_TAG_TEMP_WARNING_3 % 16].i_priority)
                    {
                        /* Set the current priority level and the error index */
                        i_priority = ERM_at_warningLog[ERM_i_TAG_TEMP_WARNING_3 % 16].i_priority;
                        i_index = ERM_i_TAG_TEMP_WARNING_3;
                    } 
                    else
                    {
                        /* Empty */
                    }
                }
                else
                {
                    /* Decrement the time hold */
                    ERM_at_warningLog[ERM_i_TAG_TEMP_WARNING_3 % 16].i_timeHold--;
                }
            }
            else
            {
                /* Empty */
            }
        }
        else
        {
            /* Empty */
        }

        if ((ERM_i_HUMI_WARNING_2_MASK & ERM_i_WARNINGS) == ERM_i_HUMI_WARNING_2_MASK)
        {
            if ((ERM_i_HUMI_WARNING_2_MASK & ERM_i_WARNINGS) == ERM_i_HUMI_WARNING_2_MASK)
            {
                /* Check if the time hold expired */
                if (ERM_at_warningLog[ERM_i_TAG_HUMI_WARNING_2 % 16].i_timeHold == 0)
                {
                    /* Check the current priority level */
                    if (i_priority > ERM_at_warningLog[ERM_i_TAG_HUMI_WARNING_2 % 16].i_priority)
                    {
                        /* Set the current priority level and the error index */
                        i_priority = ERM_at_warningLog[ERM_i_TAG_HUMI_WARNING_2 % 16].i_priority;
                        i_index = ERM_i_TAG_HUMI_WARNING_2;
                    } 
                    else
                    {
                        /* Empty */
                    }
                }
                else
                {
                    /* Decrement the time hold */
                    ERM_at_warningLog[ERM_i_TAG_HUMI_WARNING_2 % 16].i_timeHold--;
                }
            }
            else
            {
                /* Empty */
            }
            
        }
        else
        {
            /* Empty */
        }

        if ((ERM_i_WIFI_WARNING_1_MASK & ERM_i_WARNINGS) == ERM_i_WIFI_WARNING_1_MASK)
        {
            if ((ERM_i_WIFI_WARNING_1_MASK & ERM_i_WARNINGS) == ERM_i_WIFI_WARNING_1_MASK)
            {
                /* Check if the time hold expired */
                if (ERM_at_warningLog[ERM_i_TAG_WIFI_WARNING_1 % 16].i_timeHold == 0)
                {
                    /* Check the current priority level */
                    if (i_priority > ERM_at_warningLog[ERM_i_TAG_WIFI_WARNING_1 % 16].i_priority)
                    {
                        /* Set the current priority level and the error index */
                        i_priority = ERM_at_warningLog[ERM_i_TAG_WIFI_WARNING_1 % 16].i_priority;
                        i_index = ERM_i_TAG_WIFI_WARNING_1;
                    } 
                    else
                    {
                        /* Empty */
                    }
                }
                else
                {
                    /* Decrement the time hold */
                    ERM_at_warningLog[ERM_i_TAG_WIFI_WARNING_1 % 16].i_timeHold--;
                }
            }
            else
            {
                /* Empty */
            }
        }
        else
        {
            /* Empty */
        }
    }

    switch (i_index) {       
        case ERM_i_TAG_TEMP_ERROR_1:;     
            LED_t_setBlue();
            break;
        case ERM_i_TAG_HUMI_ERROR_1:;        
            LED_t_setGreen();
            break;
        case ERM_i_TAG_WIFI_ERROR_2:;       
            LED_t_setCyan();
            break;
        case ERM_i_TAG__NTP_ERROR_1:;     
            LED_t_setMagenta();
            break;
        case ERM_i_TAG__RTC_ERROR_1:;  
            LED_t_setPurple();
            break;
        case ERM_i_TAG__FAN_ERROR_1:;    
            LED_t_setRed();
            break;
        case ERM_i_TAG__FAN_ERROR_2:;
            LED_t_setRed();
            break;
        case ERM_i_TAG__FAN_ERROR_3:;
            LED_t_setRed();
            break;
        case ERM_i_TAG_TEMP_WARNING_2:;
            LED_t_setOrange();
            break;
        case ERM_i_TAG_TEMP_WARNING_3:;  
            LED_t_setOrange();
            break;
        case ERM_i_TAG_HUMI_WARNING_2:;  
            LED_t_setYellow();
            break;
        case ERM_i_TAG_WIFI_WARNING_1:;  
            LED_t_setWhite();
            break;
        default:;
            /* Emmpty */
    }

    return ERM_i_OK;
}

/**
 * @brief Log the "temperature sensors not connected" error
 * 
 * @return erm_err_t representing the error type
 */
erm_err_t ERM_t_logError_tempSensorsNotConnected(){

    return ERM_t_logError(ERM_i_TAG_TEMP_ERROR_1);
}

/**
 * @brief Delog the "temperature sensors not connected" error
 * 
 * @return erm_err_t representing the error type
 */
erm_err_t ERM_t_delogError_tempSensorsNotConnected(){
    
    return ERM_t_delogError(ERM_i_TAG_TEMP_ERROR_1);
}

/**
 * @brief Log the "temperature sensors not initialized" error
 * 
 * @return erm_err_t representing the error type
 */
erm_err_t ERM_t_logError_tempSensorsNotInitialized(){

    return ERM_t_logError(ERM_i_TAG_HUMI_ERROR_1);
}

/**
 * @brief Delog the "temperature sensors not initialized" error
 * 
 * @return erm_err_t representing the error type
 */
erm_err_t ERM_t_delogError_tempSensorsNotInitialized(){
    
    return ERM_t_delogError(ERM_i_TAG_HUMI_ERROR_1);
}

/**
 * @brief Log the "ESP32 failed to create AP" error
 * 
 * @return erm_err_t representing the error type
 */
erm_err_t ERM_t_logError_failedToCreateAP(){

    return ERM_t_logError(ERM_i_TAG_WIFI_ERROR_2);
}

/**
 * @brief Delog the "ESP32 failed to create AP" error
 * 
 * @return erm_err_t representing the error type
 */
erm_err_t ERM_t_delogError_failedToCreateAP(){
    
    return ERM_t_delogError(ERM_i_TAG_WIFI_ERROR_2);
}

/**
 * @brief Log the "NTP server connection failed" error
 * 
 * @return erm_err_t representing the error type
 */
erm_err_t ERM_t_logError_NTPserverConnectionFailed(){

    return ERM_t_logError(ERM_i_TAG__NTP_ERROR_1);
}

/**
 * @brief Delog the "NTP server connection failed" error
 * 
 * @return erm_err_t representing the error type
 */
erm_err_t ERM_t_delogError_NTPserverConnectionFailed(){
    
    return ERM_t_delogError(ERM_i_TAG__NTP_ERROR_1);
}

/**
 * @brief Log the "time received is not correct" error
 * 
 * @return erm_err_t representing the error type
 */
erm_err_t ERM_t_logError_RTCtimeNotCorrect(){

    return ERM_t_logError(ERM_i_TAG__RTC_ERROR_1);
}

/**
 * @brief Delog the "time received is not correct" error
 * 
 * @return erm_err_t representing the error type
 */
erm_err_t ERM_t_delogError_RTCtimeNotCorrect(){
    
    return ERM_t_delogError(ERM_i_TAG__RTC_ERROR_1);
}

/**
 * @brief Log the "fan module has not been initialized" error
 * 
 * @return erm_err_t representing the error type
 */
erm_err_t ERM_t_logError_fanNotInitialized(){

    return ERM_t_logError(ERM_i_TAG__FAN_ERROR_1);
}

/**
 * @brief Delog the "fan module has not been initialized." error
 * 
 * @return erm_err_t representing the error type
 */
erm_err_t ERM_t_delogError_fanNotInitialized(){
    
    return ERM_t_delogError(ERM_i_TAG__FAN_ERROR_1);
}

/**
 * @brief Log the "fan inside->outside speed error" error
 * 
 * @return erm_err_t representing the error type
 */
erm_err_t ERM_t_logError_fanIOSpeedError(){

    return ERM_t_logError(ERM_i_TAG__FAN_ERROR_2);
}

/**
 * @brief Delog the "fan inside->outside speed error" error
 * 
 * @return erm_err_t representing the error type
 */
erm_err_t ERM_t_delogError_fanIOSpeedError(){
    
    return ERM_t_delogError(ERM_i_TAG__FAN_ERROR_2);
}

/**
 * @brief Log the "fan outside->inside speed error" error
 * 
 * @return erm_err_t representing the error type
 */
erm_err_t ERM_t_logError_fanOISpeedError(){

    return ERM_t_logError(ERM_i_TAG__FAN_ERROR_3);
}

/**
 * @brief Delog the "fan outside->inside speed error" error
 * 
 * @return erm_err_t representing the error type
 */
erm_err_t ERM_t_delogError_fanOISpeedError(){
    
    return ERM_t_delogError(ERM_i_TAG__FAN_ERROR_3);
}

/**
 * @brief Log the "temperature sensor is malfunctioning" warning
 * 
 * @return erm_err_t representing the error type
 */
erm_err_t ERM_t_logWarning_tempSensorMalfunctioning(){

    return ERM_t_logWarning(ERM_i_TAG_TEMP_WARNING_2);
}

/**
 * @brief Delog the "temperature sensor is malfunctioning" warning
 * 
 * @return erm_err_t representing the error type
 */
erm_err_t ERM_t_delogWarning_tempSensorMalfunctioning(){

    return ERM_t_delogWarning(ERM_i_TAG_TEMP_WARNING_2);
}

/**
 * @brief Log the "read temperature is outside the given interval" warning
 * 
 * @return erm_err_t representing the error type
 */
erm_err_t ERM_t_logWarning_tempOutsideGivenInterval(){

    return ERM_t_logWarning(ERM_i_TAG_TEMP_WARNING_3);
}

/**
 * @brief Delog the "read temperature is outside the given interval" warning
 * 
 * @return erm_err_t representing the error type
 */
erm_err_t ERM_t_delogWarning_tempOutsideGivenInterval(){

    return ERM_t_delogWarning(ERM_i_TAG_TEMP_WARNING_3);
}

/**
 * @brief Log the "humidity could not be read correctly by the sensor" warning
 * 
 * @return erm_err_t representing the error type
 */
erm_err_t ERM_t_logWarning_humiNotReadCorrectly(){

    return ERM_t_logWarning(ERM_i_TAG_HUMI_WARNING_2);
}

/**
 * @brief Delog the "humidity could not be read correctly by the sensor" warning
 * 
 * @return erm_err_t representing the error type
 */
erm_err_t ERM_t_delogWarning_humiNotReadCorrectly(){

    return ERM_t_delogWarning(ERM_i_TAG_HUMI_WARNING_2);
}

/**
 * @brief Log the "ESP32 is not connected to a network" warning
 * 
 * @return erm_err_t representing the error type
 */
erm_err_t ERM_t_logWarning_notConnectedToNetwork(){

    return ERM_t_logWarning(ERM_i_TAG_WIFI_WARNING_1);
}

/**
 * @brief Delog the "ESP32 is not connected to a network" warning
 * 
 * @return erm_err_t representing the error type
 */
erm_err_t ERM_t_delogWarning_notConnectedToNetwork(){

    return ERM_t_delogWarning(ERM_i_TAG_WIFI_WARNING_1);
}