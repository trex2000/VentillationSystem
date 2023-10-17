/*============================================================================*/
/*                                 ALTRAN                                     */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*!
 * $Source: Webserver_Interface.h $
 * $Revision: 1 $
 * $Author: George Ciuclea $
 * $Date: 2021.08.16 $
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file
    Defines infrastructure for timer handling.
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
 * $Log: Webserver_Interface.h  $
  ============================================================================*/
#ifndef WEBSERVER_INTERFACE_H
#define WEBSERVER_INTERFACE_H
/* Includes */
/*============================================================================*/
#include "Webserver.h"
#include <stdio.h>
#include "Comfort_Interface.h"
/* Constants and types */
/*============================================================================*/
typedef enum
{
  FAN_SPEED_LOW = 0,
  FAN_SPEED_MED = 1,
  FAN_SPEED_HIGH = 2,
  FAN_MODE_REC = 3,
  FAN_MODE_NONREC = 4,
  FAN_MODE_IN_OUT = 5,
  FAN_MODE_OUT_IN = 6,
  DATA_REQUEST = 7,
  FAN_SPEED_OFF = 8
}WEBSOCKET_REQUEST_FLAG;


/* Exported Variables */
/*============================================================================*/

/* Exported functions prototypes */
/*============================================================================*/
/**
 * @brief Get the number of needed buffer reads when Webserver file is split into chunks of given size
 * 
 * @param bufferSize The size of one buffer
 * @return The number of buffers
 */
extern int getWebserverNumberBuffers(int bufferSize);
/**
 * @brief Get a pointer to the nth chunk of the HTML file
 * 
 * @param index The index of the chunk (starts at 0)
 * @param bufferSize The maximum size of the chunks
 * @param pointer The address where the pointer to the chunk memory location will be written
 * @return The size of the selected buffer (will be smaller than bufferSize if at end of file and not enough data left)
 */
extern int getWebserverBufferIndex(int index, int bufferSize, const uint8_t* *pointer);
/**
 * @brief Compiles the JSON content to be sent as a reply to the webserver data request
 * 
 * @param dataBuffer The buffer where the JSON will be written
 * @return The size of the JSON
 */
extern int compileDataPacket(uint8_t* dataBuffer);
#endif

