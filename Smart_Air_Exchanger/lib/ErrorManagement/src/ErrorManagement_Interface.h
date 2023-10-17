/**
 * @file ErrorManagement_Interface.h
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
 * $Source: ErrorManagement_Interface.h $
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
 * $Log: ErrorManagement_Interface.h  $
  ============================================================================*/

#ifndef ERRORMANAGEMENT_INTERFACE_H
#define ERRORMANAGEMENT_INTERFACE_H

/* Includes */
/*============================================================================*/
#include "ErrorManagement.h"

/* Defines */
/*============================================================================*/
#define ERM_DEBUG       FALSE       ///< Debug flag for the error management module


/* Constants and types */
/*============================================================================*/
typedef enum {
    ERM_i_OK = 0,                   ///< Success
} erm_err_t;


/* Exported Variables */
/*============================================================================*/


/* Exported functions prototypes */
/*============================================================================*/
extern erm_err_t ERM_t_displayErrorsOnWebserver(char* puc_buffer);
extern erm_err_t ERM_t_displayErrorsOnHomeAssistant(void);
extern erm_err_t ERM_t_displayErrorsOnLed(void);

extern erm_err_t ERM_t_logError_tempSensorsNotConnected(void);
extern erm_err_t ERM_t_logError_tempSensorsNotInitialized(void);
extern erm_err_t ERM_t_logError_failedToCreateAP(void);
extern erm_err_t ERM_t_logError_NTPserverConnectionFailed(void);
extern erm_err_t ERM_t_logError_RTCtimeNotCorrect(void);
extern erm_err_t ERM_t_logError_fanNotInitialized(void);
extern erm_err_t ERM_t_logError_fanIOSpeedError(void);
extern erm_err_t ERM_t_logError_fanOISpeedError(void);

extern erm_err_t ERM_t_logWarning_tempSensorMalfunctioning(void);
extern erm_err_t ERM_t_logWarning_tempOutsideGivenInterval(void);
extern erm_err_t ERM_t_logWarning_humiNotReadCorrectly(void);
extern erm_err_t ERM_t_logWarning_notConnectedToNetwork(void);

extern erm_err_t ERM_t_delogError_tempSensorsNotConnected(void);
extern erm_err_t ERM_t_delogError_tempSensorsNotInitialized(void);
extern erm_err_t ERM_t_delogError_failedToCreateAP(void);
extern erm_err_t ERM_t_delogError_NTPserverConnectionFailed(void);
extern erm_err_t ERM_t_delogError_RTCtimeNotCorrect(void);
extern erm_err_t ERM_t_delogError_fanNotInitialized(void);
extern erm_err_t ERM_t_delogError_fanIOSpeedError(void);
extern erm_err_t ERM_t_delogError_fanOISpeedError(void);

extern erm_err_t ERM_t_delogWarning_tempSensorMalfunctioning(void);
extern erm_err_t ERM_t_delogWarning_tempOutsideGivenInterval(void);
extern erm_err_t ERM_t_delogWarning_humiNotReadCorrectly(void);
extern erm_err_t ERM_t_delogWarning_notConnectedToNetwork(void);

#endif