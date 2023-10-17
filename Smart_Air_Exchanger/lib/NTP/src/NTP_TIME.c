/**
 * @file NTP_TIME.c
 * @author maria.irimus@altran.com
 * @brief 
 * @version 0.1
 * @date 2021-08-05
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
 * $Source: NTP_TIME.c $
 * $Revision: 1 $
 * $Author: Maria Irimus $
 * $Date: 2021.08.05 $
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
 * $Log: NTP_TIME.c  $
  ============================================================================*/

#ifndef NTP_TIME_C
#define NTP_TIME_C

/* Includes */
/*============================================================================*/
#include "NTP_TIME.h"
#include "NTP_interface.h"
#include "NTP_config.h"

/* Constants and types */
/*============================================================================*/

/* Defines */
/*============================================================================*/

/*! There is a connection error */
#define NTP_CON_ERROR       (int)   1

/*! The connnection was made succesfully */
#define NTP_CON_OK          (int)   0

/*! Resets to zero */
#define NTP_RESET_ZERO      (int)   0

#define NTP_ONE             (int)   1
#define NTP_ZERO            (int)   0
#define NTP_DEFAULT_YEAR    (int)   1900

/* Variables */
/*============================================================================*/

/*! Auxiliary variable for one time only functions */
static char NTP_ub_oneTimePrint = NTP_ONE;        

/*! Connection error flag */
static char NTP_ub_connectionError = NTP_ONE; 

/*! Variable to retrive the time */
static time_t NTP_t_now = NTP_RESET_ZERO;             

/*! Time info structure */
static struct tm NTP_t_timeInfo = {NTP_RESET_ZERO};     

/*! Number of retries to connect to the server */
static int NTP_i_retry = NTP_RESET_ZERO;                 

/*! Auxiliary variable for computing a 10 min delay for time refresh */
static int NTP_i_refreshTime = NTP_RESET_ZERO;            

/* Private functions prototypes */
/*============================================================================*/

/**
 * @brief Configure the NTP server connection
 * 
 */
static void NTP_v_config(void);

/**
 * @brief Verify if the syncronization with the NTP server was done
 * 
 */
static void NTP_v_obtainTime(void);

/**
 * @brief Function that prints the system time on the serial monitor
 * 
 */
void NTP_v_printTime(void);

/**
 * @brief Retrieve the internal time and set the time info variable accordingly
 * 
 */
void NTP_v_getTimeInfo(void);

/* Inline functions */
/*============================================================================*/

/* Private functions */
/*============================================================================*/

void NTP_v_config(void)
{  
#if DEBUG_NTP == 1
    ESP_LOGI("NTP", "Initializing SNTP");
#endif

    // Set the NTP operating mode to poll
    sntp_setoperatingmode(SNTP_OPMODE_POLL);

    // Choose a server for NTP
    sntp_setservername(NTP_SNTP_SERVER_NUMBER, NTP_SNTP_SERVER_NAME);

    // Intialize the NTP configuration
    sntp_init();

#if DEBUG_NTP == 1
    ESP_LOGI("NTP", "SNTP connected!");
#endif
}

void NTP_v_obtainTime(void)
{
    // Wait for time to be set
    if ( (sntp_get_sync_status() != SNTP_SYNC_STATUS_COMPLETED) && (++NTP_i_retry < NTP_CONNECTION_RETRY_COUNT) && NTP_ub_connectionError)
    {

#if DEBUG_NTP == 1
        ESP_LOGI("NTP", "Waiting for system time to be set... (%d/%d)", NTP_i_retry, NTP_CONNECTION_RETRY_COUNT);
#endif

        // Notify a connection error
        NTP_ub_connectionError = NTP_CON_ERROR;
    }
    else
    {

#if DEBUG_NTP == 1
        ESP_LOGI("NTP", "Time synchronised with the NTP server!");
#endif

        // Notify that the connection was succesful
        NTP_ub_connectionError = NTP_CON_OK;

        // Reset the retry counter
        NTP_i_retry = NTP_RESET_ZERO;
    }
}

void NTP_v_printTime(void)
{
    time_t NTP_t_now;
    struct tm t_timeinfo;

    // Get the system time
    time(&NTP_t_now);

    // Buffer for storing the formated time string
    char ub_timeBuf[64];

    // Adjust for time zone and daylight saving for Romania
    setenv("TZ", NTP_TIME_ZONE_NAME, NTP_OVERWRITE_ENV_NAME);
    // Setting the enviroment variable for time zone
    tzset();

    // Format the system time acording to the previous settings
    localtime_r(&NTP_t_now, &t_timeinfo);

    // Format the string buffer for displayting the date and time
    strftime(ub_timeBuf, sizeof(ub_timeBuf), "%c", &t_timeinfo);

#if DEBUG_NTP == 1
    // Print the current system time on the serial monitor
    ESP_LOGI("NTP", "The current date/time in Cluj is: %s", ub_timeBuf);
#endif
}

void NTP_v_getTimeInfo(void)
{
    // Get the system time
    time(&NTP_t_now);

    // Adjust for time zone and daylight saving for Romania
    setenv("TZ", NTP_TIME_ZONE_NAME, NTP_OVERWRITE_ENV_NAME);
    // Setting the enviroment variable for time zone
    tzset();

    // Format the system time acording to the previous settings
    localtime_r(&NTP_t_now, &NTP_t_timeInfo);
}

/* Exported Variables */
/*============================================================================*/

/* Exported functions prototypes */
/*============================================================================*/
/**
 * @brief Getter for the connection error flag
 * 
 * @return char representing the connection error flag
 */
char NTP_ub_getConnectionError(void);

/**
 * @brief Retrieve the connection status of the NTP
 * 
 * @return ntp_err_t representing the error type
 */
ntp_err_t NTP_t_connectionStatus(void);

/**
 * @brief Initialize the NTP server connection
 * 
 * @return ntp_err_t representing the error type
 */
ntp_err_t NTP_t_init(void);

/**
 * @brief Try to retrive data from the NTP server until the time is set correctly
 * 
 */
void NTP_v_tryConnect(void);

/**
 * @brief Set the internal time to the time received from the NTP server 
 * 
 * @return ntp_err_t representing the error type
 */
ntp_err_t NTP_t_getTime(void);

/**
 * @brief Set the seconds since the last synchronization 
 * 
 * @return int representing the seconds since the last sync
 */
int NTP_i_lastSync(void);

/**
 * @brief Get the minutes from the internal system time
 * 
 * @return int representing the minutes 
 */
int NTP_i_getMin(void);

/**
 * @brief Get the hours from the internal system time
 * 
 * @return int representing the hour
 */
int NTP_i_getHour(void);

/**
 * @brief Get the day from the internal system time
 * 
 * @return int representing the day
 */
int NTP_i_getDay(void);

/**
 * @brief Get the month from the internal system time
 * 
 * @return int representing the month
 */
int NTP_i_getMonth(void);

/**
 * @brief Get the year from the internal system time
 * 
 * @return int representing the year
 */
int NTP_i_getYear(void);

/**
 * @brief Check if current time is between 21 and 9
 * 
 * @return int 1 if true, 0 otherwise
 */
int NTP_i_checkNightTime(void);

/* Exported functions */
/*============================================================================*/

char NTP_ub_getConnectionError(void)
{
    return NTP_ub_connectionError;
}

ntp_err_t NTP_t_connectionStatus(void)
{
    if(!NTP_ub_connectionError)
    {
        return NTP_i_OK;
    }
    else
    {
        /* Empty */
    }

    return NTP_i_ERR_CONNECTION;
}

ntp_err_t NTP_t_init(void)
{
    // Initialize error status
    ntp_err_t err_status = NTP_i_OK;
   
    // Local variables for storing the time
    int NTP_i_time;
    struct tm *NTP_t_localtime;
    struct tm NTP_t_timeInfo;
    time_t NTP_t_now;

    // Read the current system time
    NTP_i_time = time(&NTP_t_now);
    if(NTP_i_time < NTP_ZERO)
    {
        err_status = NTP_i_ERR_SYSTEM_TIME;
    }
    else
    {
        /* Empty */
    }

    // Format the current system time
    NTP_t_localtime = localtime_r(&NTP_t_now, &NTP_t_timeInfo);
    if(NTP_t_localtime == NULL)
    {
        err_status = NTP_i_ERR_FORMAT_TIME;
    }
    else
    {
        /* Empty */
    }

    // Verify if the time is correctly set. If not, tm_year will be (1970 - 1900) by default.
    if (NTP_t_timeInfo.tm_year < (NTP_CURRENT_YEAR - NTP_DEFAULT_YEAR))
    {

#if DEBUG_NTP == 1
        ESP_LOGI("NTP", "Time is not set yet. Connecting to WiFi and getting time over NTP.");
#endif

        // Configure the NTP server connection
        NTP_v_config();
    }
    else
    {
        /* Empty */
    }

    return err_status;
}

void NTP_v_tryConnect(void)
{
    // If the connection was succesful and the time was retrieved correctly, increment refresh time count for 
    // the next time synchronisation, else reset the refresh counter
    if (!NTP_ub_connectionError)
    {
        NTP_i_refreshTime++;
    }
    else
    {
        NTP_i_refreshTime = NTP_RESET_ZERO;
    }

    if (NTP_i_refreshTime > NTP_TIME_REFRESH_DELAY_COUNT)
    {
        // Set a connection error until a sucesful connection to the NTP server was established
        NTP_ub_connectionError = NTP_CON_ERROR;

        // Set a flag to print the time again after the connection was succesful
        NTP_ub_oneTimePrint = NTP_ONE;

        // Restart the NTP for a new synchronisation
        sntp_restart();
    }
    else
    {
        /* Empty */
    }

    // Try to retrive the time from the NTP server if a connection error was set
    if (NTP_ub_connectionError)
    {
        NTP_v_obtainTime();
    }
    else
    {
        /* Empty */
    }
}

ntp_err_t NTP_t_getTime(void)
{
    // Initialize error status
    ntp_err_t err_status = NTP_i_OK;

    // Local variables for storing the time
    int NTP_i_time;
    struct tm *NTP_t_localtime;

    // Verify if the connection to the NTP server was succesful. Verify if this was previously called
    if (NTP_ub_oneTimePrint && !NTP_ub_getConnectionError())
    {
        // Set the time
        NTP_i_time = time(&NTP_t_now);

        if(NTP_i_time < NTP_ZERO)
        {
            err_status = NTP_i_ERR_SYSTEM_TIME;
        }
        else
        {
            /* Empty */
        }
    
        // Format the time
        NTP_t_localtime = localtime_r(&NTP_t_now, &NTP_t_timeInfo);

        if(NTP_t_localtime == NULL)
        {
            err_status = NTP_i_ERR_FORMAT_TIME;
        }
        else
        {
            /* Empty */
        }

        // Print the current system time on the serial monitor
        NTP_v_printTime();

        // Execute this task only one time after the NTP synchronisation
        NTP_ub_oneTimePrint = NTP_RESET_ZERO;
    }
    else
    {
        /* Empty */
    }
    
    return err_status;
}

int NTP_i_lastSync(void)
{
    int i_lastSync;

    // Compute the time since last sync (in seconds)
    i_lastSync = NTP_i_refreshTime * (NTP_MILIS_IN_A_SECOND / NTP_TASK_DELAY);

    // Convert the time from seconds to minutes
    i_lastSync = i_lastSync / (float)NTP_SECONDS_IN_A_MINUTE;

    return i_lastSync;
}

int NTP_i_getMin(void)
{
    // Updates the local timeInfo structure with the current system time
    NTP_v_getTimeInfo();

    int i_minutes;

    i_minutes = NTP_t_timeInfo.tm_min;

    return i_minutes;
}

int NTP_i_getHour(void)
{
    // Updates the local timeInfo structure with the current system time
    NTP_v_getTimeInfo();

    int i_hour;

    i_hour = NTP_t_timeInfo.tm_hour;

    return i_hour;
}

int NTP_i_getDay(void)
{
    // Updates the local timeInfo structure with the current system time
    NTP_v_getTimeInfo();

    int i_day;

    i_day = NTP_t_timeInfo.tm_mday;

    return i_day;
}

int NTP_i_getMonth(void)
{
    // Updates the local timeInfo structure with the current system time
    NTP_v_getTimeInfo();

    int i_month;
    // Adjust the month so that it matches the real life month number
    // The tm_mon field of the timeInfo structure starts counting the months from 0
    i_month = NTP_t_timeInfo.tm_mon + NTP_ONE;

    return i_month;
}

int NTP_i_getYear(void)
{
    // Updates the local timeInfo structure with the current system time
    NTP_v_getTimeInfo();

    int i_year;

    // Adjust the year to match the current year as an integer
    // The tm_year field of the timeInfo structure contains the number of years elapsed since 1900
    i_year = NTP_t_timeInfo.tm_year + NTP_DEFAULT_YEAR;

    return i_year;
}

int NTP_i_checkNightTime(void)
{
    // Updates the local timeInfo structure with the current system time
    NTP_v_getTimeInfo();

    int i_hour;
    i_hour = NTP_i_getHour();

    if( ( 0 <= i_hour  &&  i_hour < 9 ) || ( 21 <= i_hour  &&  i_hour <= 23 ))
    {
        return 1;
    }
    else
    {
        /* Empty */
    }

    return 0;
}
#endif