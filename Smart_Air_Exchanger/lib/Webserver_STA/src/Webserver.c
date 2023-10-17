/*============================================================================*/
/*                                 ALTRAN                                     */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*!
 * $Source: Webserver.c $
 * $Revision: 1 $
 * $Author: George Ciuclea $
 * $Date: 2021.08.16 $
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file
    Holds RTOS runtime functions that will hold calls for various submodule
    API functions, in order to run them.
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
 * $Log: Webserver.c  $
  ============================================================================*/

/* Includes */
/*============================================================================*/
#include "Webserver_Config.h"
#include <stdio.h>
#include <string.h>
#include <esp_log.h>
#include "Humidity_Interface.h"
#include "Temperature_Interface.h"
#include "FAN_Interface.h"
#include "RTC_interface.h"
#include "NTP_interface.h"
#include "Comfort_Interface.h"

/* Constants and types  */
/*============================================================================*/
/**
 * @brief Pointer to memory start for the embedded HTML file
 * @var uint8_t* $aws_root_ca_pem_start
 */
extern const uint8_t aws_root_ca_pem_start[] 
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    asm("_binary_webserver_sta_html_start")
#endif
;

extern const uint8_t aws_root_ca_pem_end[] 
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    asm("_binary_webserver_sta_html_end")
#endif
;

/* Variables */
/*============================================================================*/
int filesize = -1;
bool InOutDirection = false; //In - false, Out - true


/* Private functions prototypes */
/*============================================================================*/


/* Inline functions */
/*============================================================================*/

/* Private functions */
/*============================================================================*/


/* Exported functions */
/*============================================================================*/
/**
 * @brief Get the number of needed buffer reads when Webserver file is split into chunks of given size
 * 
 * @param bufferSize The size of one buffer
 * @return The number of buffers
 */
int getWebserverNumberBuffers(int bufferSize)
{
    return ((aws_root_ca_pem_end - aws_root_ca_pem_start) / bufferSize) + 1;
}
/**
 * @brief Get a pointer to the nth chunk of the HTML file
 * 
 * @param index The index of the chunk (starts at 0)
 * @param bufferSize The maximum size of the chunks
 * @param pointer The address where the pointer to the chunk memory location will be written
 * @return The size of the selected buffer (will be smaller than bufferSize if at end of file and not enough data left)
 */
int getWebserverBufferIndex(int index, int bufferSize, const uint8_t **pointer)
{
    *pointer = (aws_root_ca_pem_start + index * bufferSize);
    if (*pointer + bufferSize > aws_root_ca_pem_end)
    {
        return aws_root_ca_pem_end - *pointer;
    }
    return bufferSize;
}
/**
 * @brief Unused
 * 
 */
int htmlSize()
{
    int size=aws_root_ca_pem_end-aws_root_ca_pem_start;

    #if WEBSERVER_STA_DEBUG == 1
        ESP_LOGD("Webserver","HTML file size: %d",size);
    #endif

    return size;
}
/**
 * @brief Compiles the JSON content to be sent as a reply to the webserver data request
 * 
 * @param dataBuffer The buffer where the JSON will be written
 * @return The size of the JSON
 */
int compileDataPacket(uint8_t* dataBuffer)
{
    comf_mode_t comfMode=COMF_t_getMode();
    comf_speed_preset_t speed=COMF_t_getSpeed();

    char modeChar[32]={0};
    char speedChar[32]={0};

    switch (comfMode)
    {
    case COMF_i_REC:
        strcpy(modeChar,"Recycling");
        break;
    case COMF_i_NONREC:
        strcpy(modeChar,"Non-recycling");
        break;
    case COMF_i_ITO:
        strcpy(modeChar,"In to Out");
        break;
    case COMF_i_OTI:
        strcpy(modeChar,"Out to In");
        break;
    }

    switch (speed)
    {
      case COMF_i_OFF_PRESET:
        strcpy(speedChar,"OFF");
        break;
    case COMF_i_LOW_PRESET:
        strcpy(speedChar,"Low");
        break;
    case COMF_i_MED_PRESET:
        strcpy(speedChar,"Medium");
        break;
    case COMF_i_HIGH_PRESET:
        strcpy(speedChar,"High");
        break;
    }

    return sprintf((char *)dataBuffer,
    "{\n"
    "  \"humidityValue\" : %d,\n"
    "  \"tempValueIn\" : %d,\n"
    "  \"tempValueOut\" : %d,\n"
    "  \"fanSpeedIn\" : %d,\n"
    "  \"fanSpeedOut\" : %d,\n"
    "  \"hourMinuteValue\" : \"%02d:%02d\",\n"
    "  \"dateValue\" : \"%02d/%02d/%04d\",\n"
    "  \"syncTimeElapsedValue\" : %d,\n"
    "  \"fanSpeedValue\" : \"%s\",\n"
    "  \"fanDirectionValue\" : \"%s\"\n"
    "}",
    HUMID_i_readHumidity(),
    Temperature_i_getInterior(),
    Temperature_i_getExterior(),
    FAN_uw_calibratedRPM_ItO(),
    FAN_uw_calibratedRPM_OtI(),
    NTP_i_getHour(),
    NTP_i_getMin(),
    NTP_i_getDay(),
    NTP_i_getMonth(),
    NTP_i_getYear(),
    NTP_i_lastSync(),
    speedChar,
    modeChar
    );
}