/**
 * @file NVM_Storage.c
 * @author tudor.coroian@altran.com
 * @brief This file contains the implementations of the functions found in the
 * interface. The private functions are used as the main executing functions, while
 * the exported ones provide a safe access to them.
 * @version 1.2
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
 * $Source: NVM_Storage.c $
 * $Revision: 1 $
 * $Author: Tudor Coroian $
 * $Date: 2021.08.05 $
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file
 *      This file contains the implementations of the functions found in the
 * interface. The private functions are used as the main executing functions, while
 * the exported ones provide a safe access to them.
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
 * $Log: NVM_Storage.c  $
  ============================================================================*/

/* Includes */
/*============================================================================*/
#include "NVM_Config.h"
#include "NVM_Interface.h"

/* Constants and types  */
/*============================================================================*/
const char* CONST_NVM_str_namespace = "storage";            ///< Store the name of the single namespace used in the application
const char* CONST_NVM_str_ssid = "SSID";                    ///< Store the key for the SSID
const char* CONST_NVM_str_pwd = "password";                 ///< Store the key for the password for connecting to a network
const char* CONST_NVM_str_temperature_offset = "offtemp";   ///< Store the key for the temperature offset
const char* CONST_NVM_str_humidity_offset = "offhumid";     ///< Store the key for the humidity offset
const char* CONST_NVM_str_comfort_speed = "comfspeed";      ///< Store the key for the speed parameter of the comfort module
const char* CONST_NVM_str_comfort_mode = "comfmode";        ///< Store the key for the mode parameter of the comfort module
const char* CONST_NVM_str_comfort_direction = "comfdir";    ///< Store the key for the direction parameter of the comfort module

/**
 * @brief Hold indexes for all the possible integer sizes available when
 * working with the non-volatile storage.
 * 
 */
typedef enum {
    NVM_i_SIZE_U8 = 0,      ///< Flag for usigned values of 8 bits
    NVM_i_SIZE_S8,          ///< Flag for signed values of 8 bits
    NVM_i_SIZE_U16,         ///< Flag for unsigned values of 16 bits
    NVM_i_SIZE_S16,         ///< Flag for signed values of 16 bits
    NVM_i_SIZE_U32,         ///< Flag for unsigned values of 32 bits
    NVM_i_SIZE_S32,         ///< Flag for signed values of 32 bits
    NVM_i_SIZE_NOT_SET,     ///< The size of the value was not set yet
}nvm_size_t;

/* Defines */
/*============================================================================*/
#define NVM_i_NMB_OF_SIZES              (int) 6             ///< Number of size types

#define NVM_i_NVM_INITIALIZED           (int) 0             ///< Status macro for signaling that the NVM module was initialized
#define NVM_i_NVM_NOT_INITIALIZED       (int) 1             ///< Status macro for signaling that the NVM module was not initialized

#define U_BIT8_LOWER                    (int) 0             ///< Lower bound for unsigned value on 8 bits
#define U_BIT8_UPPER                    (int) 255           ///< Upper bound for unsigned value on 8 bits
#define S_BIT8_LOWER                    (int) -128          ///< Lower bound for signed value on 8 bits
#define S_BIT8_UPPER                    (int) 127           ///< Upper bound for signed value on 8 bits
#define U_BIT16_LOWER                   (int) 0             ///< Lower bound for unsigned value on 16 bits
#define U_BIT16_UPPER                   (int) 65535         ///< Upper bound for unsigned value on 16 bits
#define S_BIT16_LOWER                   (int) -32768        ///< Lower bound for signed value on 16 bits
#define S_BIT16_UPPER                   (int) 32767         ///< Upper bound for signed value on 16 bits
#define U_BIT32_LOWER                   (int) 0             ///< Lower bound for unsigned value on 32 bits
#define U_BIT32_UPPER                   (int) 4294967295    ///< Upper bound for unsigned value on 32 bits
#define S_BIT32_LOWER                   (int) -2147483648   ///< Lower bound for signed value on 32 bits
#define S_BIT32_UPPER                   (int) 2147483647    ///< Upper bound for signed value on 32 bits


/* Variables */
/*============================================================================*/
static int NVM_i_status;                        ///< Hold the current status of the non-volatile memory

static nvm_size_t NVM_t_size_temp_offset;       ///< Hold the size written for the temperature offset
static nvm_size_t NVM_t_size_humid_offset;      ///< Hold the size written for the humidity offset
static nvm_size_t NVM_t_size_comfort_param;     ///< Hold the size written for the comfort parameters

/**
 * @brief Lower bounds stored in vectors for dynamic access
 * 
 */
static nvm_size_t NVM_t_lowerBounds[NVM_i_NMB_OF_SIZES] = {
    U_BIT8_LOWER,
    S_BIT8_LOWER,
    U_BIT16_LOWER,
    S_BIT16_LOWER,
    U_BIT32_LOWER,
    S_BIT32_LOWER
};
/**
 * @brief Upper bounds stored in vectors for dynamic access
 * 
 */
static nvm_size_t NVM_t_upperBounds[NVM_i_NMB_OF_SIZES] = {
    U_BIT8_UPPER,
    S_BIT8_UPPER,
    U_BIT16_UPPER,
    S_BIT16_UPPER,
    U_BIT32_UPPER,
    S_BIT32_UPPER
};

/* Private functions prototypes */
/*============================================================================*/


/* Inline functions */
/*============================================================================*/


/* Private functions */
/*============================================================================*/
/**
 * @brief Read a string value stored at the specified key.
 *        Returns NVM_OK in case of success.
 * 
 * @param key The name of the stored value
 * @param value The actual value written in memory
 * @return nvm_err_t  
 */
nvm_err_t NVM_read_str(const char* key, char* value) {
    //Open non-volatile storage
    nvs_handle_t NVM_handle;
    esp_err_t err = nvs_open(CONST_NVM_str_namespace, NVS_READONLY, &NVM_handle);

    //Check if the storage was opened correctly
    if (err != ESP_OK) {
        return NVM_i_ERR_OPEN;
    } else {
        size_t NVM_requiredSize;    //Store the size of the value to be extracted
        err = nvs_get_str(NVM_handle, key, NULL, &NVM_requiredSize);
        err = nvs_get_str(NVM_handle, key, value, &NVM_requiredSize);

        //Close the non-volatile storage
        nvs_close(NVM_handle);
    }
    switch(err) {
        case ESP_OK:
            return NVM_i_OK;
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            return NVM_i_ERR_VALUE_NOT_FOUND;
            break;
        default:
            return NVM_i_ERR_READ;
    }
}
/**
 * @brief Write a string value at the specified key.
 *        Return NVM_OK in case of success.
 * 
 * @param key The name of the stored value
 * @param value The actual value written in memory
 * @return nvm_err_t 
 */
nvm_err_t NVM_write_str(const char* key, char* value) {
    //Open non-volatile storage
    nvs_handle_t NVM_handle;
    esp_err_t err = nvs_open(CONST_NVM_str_namespace, NVS_READWRITE, &NVM_handle);

    //Check if the storage was opened correctly
    if (err != ESP_OK) {
        return NVM_i_ERR_OPEN;
    } else {
        err = nvs_set_str(NVM_handle, key, value);

        //Commit the updates
        nvs_commit(NVM_handle);
        //Close the non-volatile storage
        nvs_close(NVM_handle);
    }
    switch(err) {
        case ESP_OK:
            return NVM_i_OK;
            break;
        default:
            return NVM_i_ERR_WRITE;
    }
}

/**
 * @brief Read an integer value from the memory. Return
 *        NVM_i_OK in case of success.
 * 
 * @param key   The name of the stored value
 * @param value The value read from the memory
 * @param size  The size of the value to be read from memory
 * @return nvm_err_t 
 */
nvm_err_t NVM_read_integer(const char* key, int* value, nvm_size_t size) {
    /* Open non-volatile storage */
    nvs_handle_t NVM_handle;
    esp_err_t err = nvs_open(CONST_NVM_str_namespace, NVS_READWRITE, &NVM_handle);

    /* Check if the storage was opened correctly */
    if (err != ESP_OK) {
        return NVM_i_ERR_OPEN;
    } else {
        /* Empty */
    }

    /* Branch based on the size of the value to be read */
    switch (size) {
        case NVM_i_SIZE_U8:;     /* Unsigned 8 bits */
            /* Variable to store the value read from the memory */
            uint8_t placeholder_u8;
            err = nvs_get_u8(NVM_handle, key, &placeholder_u8);
            *value = placeholder_u8;
            break;
        case NVM_i_SIZE_S8:;     /* Signed 8 bits */
            /* Variable to store the value read from the memory */
            int8_t placeholder_s8;
            err = nvs_get_i8(NVM_handle, key, &placeholder_s8);
            *value = placeholder_s8;
            break;
        case NVM_i_SIZE_U16:;    /* Unsigned 16 bits */
            /* Variable to store the value read from the memory */
            uint16_t placeholder_u16;
            err = nvs_get_u16(NVM_handle, key, &placeholder_u16);
            *value = placeholder_u16;
            break;
        case NVM_i_SIZE_S16:;    /* Signed 16 bits */
            /* Variable to store the value read from the memory */
            int16_t placeholder_s16;
            err = nvs_get_i16(NVM_handle, key, &placeholder_s16);
            *value = placeholder_s16;
            break;
        case NVM_i_SIZE_U32:;    /* Unsigned 32 bits */
            /* Variable to store the value read from the memory */
            uint32_t placeholder_u32;
            err = nvs_get_u32(NVM_handle, key, &placeholder_u32);
            *value = placeholder_u32;
            break;
        case NVM_i_SIZE_S32:;    /* Signed 32 bits */
            /* Variable to store the value read from the memory */
            int32_t placeholder_s32;
            err = nvs_get_i32(NVM_handle, key, &placeholder_s32);
            *value = placeholder_s32;
            break;
        default:
            return NVM_i_ERR_UNSPECIFIED_SIZE;
    }

    /* Check for errors */
    switch(err) {
        case ESP_OK:
            return NVM_i_OK;
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            return NVM_i_ERR_VALUE_NOT_FOUND;
            break;
        default:
            return NVM_i_ERR_READ;
    }
}

nvm_err_t NVM_write_integer (const char* key, int value, nvm_size_t size) {
    /* Open non-volatile storage */
    nvs_handle_t NVM_handle;
    esp_err_t err = nvs_open(CONST_NVM_str_namespace, NVS_READWRITE, &NVM_handle);

    /* Check if the storage was opened correctly */
    if (err != ESP_OK) {
        return NVM_i_ERR_OPEN;
    } else {
        /* Empty */
    }

    /* Branch based on size of the value to be written*/
    switch (size) {
        case NVM_i_SIZE_U8:;     /* Unsigned 8 bits */
            /* Variable to be written in memory */
            uint8_t placeholder_u8;
            placeholder_u8 = value;
            err = nvs_set_u8(NVM_handle, key, placeholder_u8);
            break;
        case NVM_i_SIZE_S8:;     /* Signed 8 bits */
            /* Variable to be written in memory */
            int8_t placeholder_s8;
            placeholder_s8 = value;
            err = nvs_set_i8(NVM_handle, key, placeholder_s8);
            break;
        case NVM_i_SIZE_U16:;    /* Unsigned 16 bits */
            /* Variable to be written in memory */
            uint16_t placeholder_u16;
            placeholder_u16 = value;
            err = nvs_set_u16(NVM_handle, key, placeholder_u16);
            break;
        case NVM_i_SIZE_S16:;    /* Signed 16 bits */
            /* Variable to be written in memory */
            int16_t placeholder_s16;
            placeholder_s16 = value;
            err = nvs_set_i16(NVM_handle, key, placeholder_s16);
            break;
        case NVM_i_SIZE_U32:;    /* Unsigned 32 bits */
            /* Variable to be written in memory */
            uint32_t placeholder_u32;
            placeholder_u32 = value;
            err = nvs_set_u32(NVM_handle, key, placeholder_u32);
            break;
        case NVM_i_SIZE_S32:;    /* Signed 32 bits */
            /* Variable to be written in memory */
            int32_t placeholder_s32;
            placeholder_s32 = value;
            err = nvs_set_i32(NVM_handle, key, placeholder_s32);
            break;
        default:                /* Undefined size */
            return NVM_i_ERR_UNSPECIFIED_SIZE;
    }

    /* Check for errors */
    switch(err) {
        case ESP_OK:
            return NVM_i_OK;
            break;
        default:
            return NVM_i_ERR_WRITE;
    }
}

/* Exported functions */
/*============================================================================*/
/**
 * @brief Initializer function for the Non-volatile memory module
 * 
 */
nvm_err_t NVM_t_init(void) 
{
    esp_err_t err = nvs_flash_init();
    NVM_i_status = NVM_i_NVM_NOT_INITIALIZED ;
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        /* NVS partition was truncated and needs to be erased */
        #if NVM_DEBUG == TRUE
            ESP_ERROR_CHECK(nvs_flash_erase());
        #else
            nvs_flash_erase();
        #endif
        /* Retry nvs_flash_init */
        err = nvs_flash_init();
    }

    /* Initialize the size of the stored values */
    NVM_t_size_temp_offset      = NVM_i_SIZE_U8;
    NVM_t_size_humid_offset     = NVM_i_SIZE_U8;
    NVM_t_size_comfort_param    = NVM_i_SIZE_U8;

    #if NVM_DEBUG == TRUE
        ESP_ERROR_CHECK(err);
    #endif

    if (err != ESP_OK) {
        return NVM_i_ERR_INIT;
    } else {
        /* Empty */
    }

    NVM_i_status = NVM_i_NVM_INITIALIZED;
    return NVM_i_OK;
}

/**
 * @brief Check the status of the non-volatile storage
 * 
 * @return nvm_err_t 
 */
nvm_err_t NVM_t_status(void) {
    switch(NVM_i_status) {
        case NVM_i_NVM_INITIALIZED:
            return NVM_i_STORAGE_INITIALIZED;
            break;
        default:
            return NVM_i_STORAGE_NOT_INITIALIZED;
    }
}

/**
 * @brief Get the WiFi credentials stored in the non-volatile memory.
 *        Return NVM_ERR_NO_CREDENTIALS_FOUND if one of the credentials 
 *        was not found in the memory.
 * 
 * @param ssid Used to store the actual value for the SSID
 * @param pwd Used to store the actual value for the password
 * @return nvm_err_t 
 */
nvm_err_t NVM_t_getWifiCredentials(char* ssid, char* pwd) {

    if(strcmp(ssid, "") != 0 || strcmp(pwd, "") != 0) {
        /* Check if the parameters are not empty */
        return NVM_i_ERR_NON_EMPTY_PARAM;
    } else if (NVM_i_status == NVM_i_NVM_NOT_INITIALIZED) {
        /* Check if the NVM module was initialized */
        return NVM_i_STORAGE_NOT_INITIALIZED;
    } else {
        /* Empty */
    }

    /* Read the requiered values from the memory */
    nvm_err_t err_ssid = NVM_read_str(CONST_NVM_str_ssid, ssid);
    nvm_err_t err_pwd  = NVM_read_str(CONST_NVM_str_pwd, pwd);

    if (err_ssid == NVM_i_ERR_VALUE_NOT_FOUND || err_pwd == NVM_i_ERR_VALUE_NOT_FOUND) {
        /* Check if the values were found */
        return NVM_i_ERR_NO_CREDENTIALS_FOUND;
    } else if (err_ssid != NVM_i_OK || err_pwd != NVM_i_OK){
        /* Check if something went wrong */
        return NVM_i_ERR_READ;
    } else {
        /* Empty */
    }

    return NVM_i_OK;
}

/**
 * @brief Store the WiFi credentials stored in the non-volatile memory.
 *        Return NVM_ERR_WRITE if the values were not written in the
 *        memory.
 * 
 * @param ssid Used to store the actual value for the SSID
 * @param pwd Used to store the actual value for the password 
 * @return nvm_err_t 
 */
nvm_err_t NVM_t_setWifiCredentials(char* ssid, char* pwd){
    if(strcmp(ssid, "") == 0 || strcmp(pwd, "") == 0) {
        /* Check if the parameters are empty */
        return NVM_i_ERR_EMPTY_PARAM;
    } else if (NVM_i_status == NVM_i_NVM_NOT_INITIALIZED) {
        /* Check if the NVM module was initialized */
        return NVM_i_STORAGE_NOT_INITIALIZED;
    } else {
        /* Empty */
    }

    /* Write the requiered values in the memory */
    nvm_err_t err_ssid = NVM_write_str(CONST_NVM_str_ssid, ssid);
    nvm_err_t err_pwd  = NVM_write_str(CONST_NVM_str_pwd, pwd);

    if (err_ssid != NVM_i_OK || err_pwd != NVM_i_OK) {
        /* Check if the values have been written successfully */
        return NVM_i_ERR_WRITE;
    } else {
        /* Empty */
    }

    return NVM_i_OK;
}

/**
 * @brief Read the temperature offset from the memory.
 *        Return NVM_i_OK in case of success.
 * 
 * @param offset Store the read value
 * @return nvm_err_t 
 */
nvm_err_t NVM_t_getTempOffset(int* offset) {
    if (NVM_i_status != NVM_i_NVM_INITIALIZED) {
        /* Check if the non-volatile memory was initialized */
        return NVM_i_STORAGE_NOT_INITIALIZED;
    } else {
        /* Empty */
    }

    /* Read the temperature offset from the memory */
    nvm_err_t err = NVM_read_integer(CONST_NVM_str_temperature_offset, offset, NVM_t_size_temp_offset);
    return err;
}
/**
 * @brief Write the temperature offset in the memory.
 *        Return NVM_i_OK in case of success.
 * 
 * @param offset Value to be written
 * @return nvm_err_t 
 */
nvm_err_t NVM_t_setTempOffset(int offset) {
    if (NVM_i_status != NVM_i_NVM_INITIALIZED) {
        /* Check if the non-volatile memory was initialized */
        return NVM_i_STORAGE_NOT_INITIALIZED;
    } else if (offset < NVM_t_lowerBounds[NVM_t_size_temp_offset] || offset > NVM_t_upperBounds[NVM_t_size_temp_offset]) {
        /* Check if the value is within the boundaries */
        return NVM_i_ERR_VALUE_OUTSIDE_BOUNDARIES;
    } else {
        /* Empty */
    }

    /* Write the temperature offset in the memory */
    nvm_err_t err = NVM_write_integer(CONST_NVM_str_temperature_offset, offset, NVM_t_size_temp_offset);
    return err;
}
/**
 * @brief Read the humidity offset from the memory.
 *        Return NVM_i_OK in case of success.
 * 
 * @param offset Store the read value
 * @return nvm_err_t 
 */
nvm_err_t NVM_t_getHumidityOffset(int* offset) {
    if (NVM_i_status != NVM_i_NVM_INITIALIZED) {
        /* Check if the non-volatile memory was initialized */
        return NVM_i_STORAGE_NOT_INITIALIZED;
    } else {
        /* Empty */
    }

    /* Read the humidity offset from the memory */
    nvm_err_t err = NVM_read_integer(CONST_NVM_str_humidity_offset, offset, NVM_t_size_humid_offset);
    return err;
}
/**
 * @brief Write the humidity offset in the memory.
 *        Return NVM_i_OK in case of success.
 * 
 * @param offset Value to be written
 * @return nvm_err_t 
 */
nvm_err_t NVM_t_setHumidityOffset(int offset) {
    if (NVM_i_status != NVM_i_NVM_INITIALIZED) {
        /* Check if the non-volatile memory was initialized */
        return NVM_i_STORAGE_NOT_INITIALIZED;
    } else if (offset < NVM_t_lowerBounds[NVM_t_size_humid_offset] || offset > NVM_t_upperBounds[NVM_t_size_humid_offset]) {
        /* Check if the value is within the boundaries */
        return NVM_i_ERR_VALUE_OUTSIDE_BOUNDARIES;
    } else {
        /* Empty */
    }

    /* Write the humidity offset in the memory */
    nvm_err_t err = NVM_write_integer(CONST_NVM_str_humidity_offset, offset, NVM_t_size_humid_offset);
    return err;
}
/**
 * @brief Read the fan speed from the memory.
 *        Return NVM_i_OK in case of success.
 * 
 * @param offset Store the read value
 * @return nvm_err_t 
 */
nvm_err_t NVM_t_getFanSpeed(int* speed) {
    if (NVM_i_status != NVM_i_NVM_INITIALIZED) {
        /* Check if the non-volatile memory was initialized */
        return NVM_i_STORAGE_NOT_INITIALIZED;
    } else {
        /* Empty */
    }

    /* Read the fan speed from the memory */
    nvm_err_t err = NVM_read_integer(CONST_NVM_str_comfort_speed, speed, NVM_t_size_comfort_param);    
    return err;
}
/**
 * @brief Write the fan speed in the memory.
 *        Return NVM_i_OK in case of success.
 * 
 * @param offset Value to be written
 * @return nvm_err_t 
 */
nvm_err_t NVM_t_setFanSpeed(int speed) {
    if (NVM_i_status != NVM_i_NVM_INITIALIZED) {
        /* Check if the non-volatile memory was initialized */
        return NVM_i_STORAGE_NOT_INITIALIZED;
    } else if (speed < NVM_t_lowerBounds[NVM_t_size_comfort_param] || speed > NVM_t_upperBounds[NVM_t_size_comfort_param]) {
        /* Check if the value is within the boundaries */
        return NVM_i_ERR_VALUE_OUTSIDE_BOUNDARIES;
    } else {
        /* Empty */
    }

    /* Write the fan speed in the memory */
    nvm_err_t err = NVM_write_integer(CONST_NVM_str_comfort_speed, speed, NVM_t_size_comfort_param);    
    return err;
}
/**
 * @brief Read the fan functioning mode from the memory.
 *        Return NVM_i_OK in case of success.
 * 
 * @param slope Store the read value 
 * @return nvm_err_t 
 */
nvm_err_t NVM_t_getFanMode(int* mode) {
    if (NVM_i_status != NVM_i_NVM_INITIALIZED) {
        /* Check if the non-volatile memory was initialized */
        return NVM_i_STORAGE_NOT_INITIALIZED;
    } else {
        /* Empty */
    }

    /* Read the fan functioning mode from the memory */
    nvm_err_t err = NVM_read_integer(CONST_NVM_str_comfort_mode, mode, NVM_t_size_comfort_param);    
    return err;
}
/**
 * @brief Write the fan functioning mode in the memory.
 *        Return NVM_i_OK in case of success.
 * 
 * @param slope Value to be written
 * @return nvm_err_t 
 */
nvm_err_t NVM_t_setFanMode(int mode) {
    if (NVM_i_status != NVM_i_NVM_INITIALIZED) {
        /* Check if the non-volatile memory was initialized */
        return NVM_i_STORAGE_NOT_INITIALIZED;
    } else if (mode < NVM_t_lowerBounds[NVM_t_size_comfort_param] || mode > NVM_t_upperBounds[NVM_t_size_comfort_param]) {
        /* Check if the value is within the boundaries */
        return NVM_i_ERR_VALUE_OUTSIDE_BOUNDARIES;
    } else {
        /* Empty */
    }

    /* Write the fan functioning mode in the memory */
    nvm_err_t err = NVM_write_integer(CONST_NVM_str_comfort_mode, mode, NVM_t_size_comfort_param);    
    return err;
}
/**
 * @brief Read the fan spinning direction from the memory.
 *        Return NVM_i_OK in case of success.
 * 
 * @param errorCoeff Store the read value
 * @return nvm_err_t 
 */
nvm_err_t NVM_t_getFanDirection(int *direction) {
    if (NVM_i_status != NVM_i_NVM_INITIALIZED) {
        /* Check if the non-volatile memory was initialized */
        return NVM_i_STORAGE_NOT_INITIALIZED;
    } else {
        /* Empty */
    }

    /* Read the fan spinning direction from the memory */
    nvm_err_t err = NVM_read_integer(CONST_NVM_str_comfort_direction, direction, NVM_t_size_comfort_param);    
    return err;
}
/**
 * @brief Write the fan spinning direction in the memory.
 *        Return NVM_i_OK in case of success.
 * 
 * @param errorCoeff Value to be written
 * @return nvm_err_t 
 */
nvm_err_t NVM_t_setFanDirection(int direction) {
    if (NVM_i_status != NVM_i_NVM_INITIALIZED) {
        /* Check if the non-volatile memory was initialized */
        return NVM_i_STORAGE_NOT_INITIALIZED;
    } else if (direction < NVM_t_lowerBounds[NVM_t_size_comfort_param] || direction > NVM_t_upperBounds[NVM_t_size_comfort_param]) {
        /* Check if the value is within the boundaries */
        return NVM_i_ERR_VALUE_OUTSIDE_BOUNDARIES;
    } else {
        /* Empty */
    }

    /* Write the fan spinning direction in the memory */
    nvm_err_t err = NVM_write_integer(CONST_NVM_str_comfort_direction, direction, NVM_t_size_comfort_param);    
    return err;
}
