/**
 * @file NVM_Example.c
 * @author Tudor Coroian (tudor.coroian@altran.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-25
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
 * $Source: NVM_Example.c $
 * $Revision: 1 $
 * $Author: Tudor Coroian $
 * $Date: 2021.08.25 $
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
 * $Log: NVM_Example.c  $
  ============================================================================*/

/* Includes */
/*============================================================================*/
#include "NVM_Interface.h"

void NVM_v_example() {
    /* Initialize the module */
    /* ====================================================================== */
    nvm_err_t err = NVM_t_init();

    if (err != NVM_i_OK) {
        /* Pass error to error handleing module */
        return;
    } else {
        /* Empty */
    }

    /* Check the status of the module */
    /* ====================================================================== */
    err = NVM_t_status();

    if (err != NVM_i_STORAGE_INITIALIZED) {
        /* Call initializing function */
    } else {
        /* Empty */
    }

    /* Read wifi credentials from memory */
    /* ====================================================================== */
    /* Variables used to return the values read from the memory */
    char* SSID;
    char* pwd;

    /* Parameters must be empty (have null values) */
    err = NVM_t_getWifiCredentials(SSID, pwd); 

    if (err != NVM_i_OK) {
        /* Pass error to error handleing module */
    } else {
        /* Empty */
    }
}