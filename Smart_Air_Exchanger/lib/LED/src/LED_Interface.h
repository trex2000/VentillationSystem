/**
 * @file LED_Interface.h
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
 * $Source: LED_Interface.h $
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
 * $Log: LED_Interface.h  $
  ============================================================================*/

#ifndef LED_INTERFACE_H
#define LED_INTERFACE_H

/* Includes */
/*============================================================================*/


/* Defines */
/*============================================================================*/
#define LED_DEBUG               0       ///< Flag for enableing debug mode on the module

/* Constants and types */
/*============================================================================*/
typedef enum {
    LED_i_OK = 0,                       ///< Success
    LED_i_STATUS_NOT_INITIALIZED,       ///< LED control module was not initialized
    LED_i_STATUS_INITIALIZED,           ///< LED control module was initialized
    LED_i_STATUS_UNDER_USE,             ///< LED control module is being used
    LED_i_STATUS_FREE,                  ///< LED control module in not being used
    LED_i_ERR_INVALID_ARGUMETS,         ///< Invalid argumets passed to the fucntion
} led_err_t;

/* Exported Variables */
/*============================================================================*/


/* Exported functions prototypes */
/*============================================================================*/
extern led_err_t LED_t_init(void);
extern led_err_t LED_t_status(void);
extern led_err_t LED_t_setGreen(void);
extern led_err_t LED_t_setRed(void);
extern led_err_t LED_t_setBlue(void);
extern led_err_t LED_t_setYellow(void);
extern led_err_t LED_t_setMagenta(void);
extern led_err_t LED_t_setCyan(void);
extern led_err_t LED_t_setOrange(void);
extern led_err_t LED_t_setPurple(void);
extern led_err_t LED_t_setWhite(void);
extern led_err_t LED_t_setBlack(void);


#endif