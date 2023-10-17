/**
 * @file DS18B20_In_Temp.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-04-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */
/* Includes */
/*============================================================================*/

#include "DS18B20_Config.h"
#include "owb.h"
#include "owb_rmt.h"
#include "ds18b20.h"


/* Constants and types  */
/*============================================================================*/

#define GPIO_DS18B20_0       (EXT_TEMP)
#define MAX_DEVICES          (4)
#define DS18B20_RESOLUTION   (DS18B20_RESOLUTION_12_BIT)
#define SAMPLE_PERIOD        (1000)   // milliseconds

/* Variables */
/*============================================================================*/



/* Private functions prototypes */
/*============================================================================*/



/* Inline functions */
/*============================================================================*/




/* Private functions */
/*============================================================================*/

/**
 * @brief Initialize the External Temperature Sensors
 * 
 */
void Ext_Temp_Init()
{
    /* Create and configure One Wire Bus */

    // Create a 1-Wire bus, using the RMT timeslot driver
    OneWireBus * owb;
    owb_rmt_driver_info rmt_driver_info;
    owb = owb_rmt_initialize(&rmt_driver_info, GPIO_DS18B20_0, RMT_CHANNEL_1, RMT_CHANNEL_0);
    owb_use_crc(owb, true);  // enable CRC check for ROM code

    // Find all connected devices
    printf("Find devices:\n");
    OneWireBus_ROMCode device_rom_codes[MAX_DEVICES] = {0};
    int num_devices = 0;
    OneWireBus_SearchState search_state = {0};
    bool found = false;
    owb_search_first(owb, &search_state, &found);
    /* TODO : Add time frame for sensor search !! */    
    while (found)
    {
        char rom_code_s[17];
        owb_string_from_rom_code(search_state.rom_code, rom_code_s, sizeof(rom_code_s));
        printf("  %d : %s\n", num_devices, rom_code_s);
        device_rom_codes[num_devices] = search_state.rom_code;
        ++num_devices;
        owb_search_next(owb, &search_state, &found);
    }
    /* TODO Only in the development phase use printf */
    printf("Found %d device%s\n", num_devices, num_devices == 1 ? "" : "s");
}


/* Exported functions */
/*============================================================================*/
