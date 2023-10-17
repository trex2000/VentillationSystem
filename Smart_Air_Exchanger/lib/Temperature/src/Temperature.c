/**
 * @file Temperature.c
 * @author Ecaterina Lutac (ecaterina.lutac@altran.com)
 * @brief 
 * @version 1.2
 * @date 2021-09-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */
/* Includes */
/*============================================================================*/
#include "Temperature_Config.h"
#include "Temperature_Interface.h"
#include "Temperature.h"

/* Constants and types  */
/*============================================================================*/
#define GPIO_DS18B20_0 (TEMP_PIN) ///< One-Wire Bus pin
#define MAX_DEVICES (2)           ///< Maximum number of temperature sensors that can be connected to the bus
#define DS18B20_RESOLUTION (DS18B20_RESOLUTION_12_BIT)
#define SAMPLE_PERIOD (1000) // milliseconds    ///< used at first, when VtaskDelay() was integrated in the code   
#define TAG ("Temperature")  ///< name for module logs
#define WINDOW_SIZE 4                       ///< number of values to be memorized for the average filter
#define HYST_TRIGGER_LEVEL 5                ///< hysteresis trigger level
#define MINIMUM_ADMISSIBLE_TEMPERATURE -300 ///<lowest admitted temperature * 10
#define MAXIMUM_ADMISSIBLE_TEMPERATURE 500  ///< highest admitted temperature * 10
#define TEMPERATURE_OFFSET 2                ///< offset which is the result of experiments
#define TEMPERATURE_DEFAULT_OFFSET 0        ///< offset in case no other offset is given
#define TEMPERATURE_DEFAULT 850             ///< value in case of any error or warning
#define EXTERIOR ("e53c01f096340428") ///< exterior temperature sensor rom-code
#define INTERIOR ("4d3c01f096466328") ///< interior temperature sensor rom-code

/* Variables */
/*============================================================================*/
static char Temperature_sac_InteriorSensor[17];                               ///< store the interior temperature sensor's rom-code
static char Temperature_sac_ExteriorsensoS[17];                               ///< store the exterior temperature sensor's rom-code
int32_t Temperature_i_Interior;                                               ///< variable that can be seen by other modules
int32_t Temperature_i_Exterior;                                               ///< variable that can be seen by other modules
volatile int Temperature_vi_NumDevices;                                       ///< store the number of connected devices on the One-Wire Bus
static int32_t Temperature_si_INT = INT32_MIN;                                ///< store the interior temperature
static int32_t Temperature_si_OUT = INT32_MIN;                                ///< store the exterior temperature
static int Temperature_sai_SumTemperature[MAX_DEVICES];                       ///< Sum used for the temperature average filter
static int Temperature_si_IndexTemperature[MAX_DEVICES];                      ///< Position in the readings of the temperature values array
static int32_t Temperature_sai_ReadingsTemperature[MAX_DEVICES][WINDOW_SIZE]; ///< Array of size: WINDOW_SIZE used to store past values of the sensor's temperatures
static int32_t Temperature_si_AverageTempValue[MAX_DEVICES];                  ///< Variables used to store the filtered values
static int32_t Temperature_si_PrevReadTemperatureInt;                         ///< variable in which the previously read interior temperature is stored
static int32_t Temperature_si_PrevReadTemperatureExt;                         ///< variable in which the previously read exterior temperature is stored
static int32_t Temperature_si_Offset;                                         ///< variable which will store the value of the offset
static OneWireBus *Temperature_spt_OWB;                                       ///< store the One-Wire Bus instance
static DS18B20_Info *Temperature_sapt_Devices[MAX_DEVICES] = {0};             ///< array that stores the info of the devices connected to the One Wire Bus
static owb_rmt_driver_info Temperature_st_RMTDriverInfo;                      ///< variable that stores the RMT driver's info
static owb_gpio_driver_info Temperature_st_GPIODriverInfo;                    ///< variable that stores the GPIO driver's info

/* Private functions prototypes */
/*============================================================================*/
temperature_err_t Temperature_t_findDevice(char *rom_code, int *device_number);
temperature_err_t Temperature_t_Read_Interior();
temperature_err_t Temperature_t_Read_Exterior();
void Temperature_v_FilterDeviceTemperature(bool Temperature_b_ExteriorVSInteriorDevice);
temperature_err_t Temperature_t_CheckValidity(int32_t Temperature_i_Value);
int32_t Temperature_i_ConvertToMult5(int32_t Temperature_i_Value);
int32_t Temperature_i_Hysteresis(int32_t Temperature_i_PreviousTemperature, int32_t Temperature_i_CurrentTemperature);

/* Inline functions */
/*============================================================================*/

/* Private functions */
/*============================================================================*/
/**
 * @brief Initialise the One-Wire Bus and connect the temperature sensors.
 *  
 */
void Temperature_v_Init()
{
    bool Temperature_b_FoundDevices = false;
    char Temperature_c_ROMCodeString[17];
    int Temperature_i_DeviceIndex = 0;
    // Find all connected devices
    OneWireBus_ROMCode Temperature_t_DeviceROMCodes[MAX_DEVICES] = {0};
    OneWireBus_SearchState Temperature_t_SearchState = {0};
    DS18B20_Info *Temperature_pt_DS18B20Info;
    nvm_err_t Temperature_t_Err;

    strcpy(Temperature_sac_InteriorSensor, INTERIOR);
    strcpy(Temperature_sac_ExteriorsensoS, EXTERIOR);
    Temperature_vi_NumDevices = 0;
    Temperature_si_INT = INT32_MIN;
    Temperature_si_OUT = INT32_MIN;
    Temperature_sai_SumTemperature[0] = 0;
    Temperature_sai_SumTemperature[1] = 0;
    Temperature_si_IndexTemperature[0] = 0;
    Temperature_si_IndexTemperature[1] = 0;
    Temperature_si_AverageTempValue[0] = 0;
    Temperature_si_AverageTempValue[1] = 0;
    Temperature_si_PrevReadTemperatureInt = 0;
    Temperature_si_PrevReadTemperatureExt = 0;

    owb_gpio_initialize(&Temperature_st_GPIODriverInfo, GPIO_DS18B20_0);
    Temperature_spt_OWB = owb_rmt_initialize(&Temperature_st_RMTDriverInfo, GPIO_DS18B20_0, RMT_CHANNEL_1, RMT_CHANNEL_0);
    owb_use_crc(Temperature_spt_OWB, true); // enable CRC check for ROM code
    owb_search_first(Temperature_spt_OWB, &Temperature_t_SearchState, &Temperature_b_FoundDevices);
    while (Temperature_b_FoundDevices == true && Temperature_vi_NumDevices < MAX_DEVICES)
    {
        owb_string_from_rom_code(Temperature_t_SearchState.rom_code, Temperature_c_ROMCodeString, sizeof(Temperature_c_ROMCodeString));
#ifdef DEBUG_TEMPERATURE
        if (DEBUG_TEMPERATURE == 1)
        {
            ESP_LOGI(TAG, "owb:  %d : %s\n", Temperature_vi_NumDevices, Temperature_c_ROMCodeString);
        }
        else
        {
            /*empty*/
        }
#endif
        Temperature_t_DeviceROMCodes[Temperature_vi_NumDevices] = Temperature_t_SearchState.rom_code;
        Temperature_vi_NumDevices++;
        owb_search_next(Temperature_spt_OWB, &Temperature_t_SearchState, &Temperature_b_FoundDevices);
    }
#ifdef DEBUG_TEMPERATURE
    if (DEBUG_TEMPERATURE == 1)
    {
        ESP_LOGI(TAG, "    Found %d device%s\n", Temperature_vi_NumDevices, Temperature_vi_NumDevices == 1 ? "" : "s");
    }
    else
    { /*empty*/
    }
#endif
    if (Temperature_vi_NumDevices == 0)
    {
        ESP_LOGW(TAG, "No devices connected to the bus!");
    }
    else
    {
        // Create DS18B20 devices on the 1-Wire bus
        for (Temperature_i_DeviceIndex = 0; Temperature_i_DeviceIndex < Temperature_vi_NumDevices; Temperature_i_DeviceIndex++)
        {
            Temperature_pt_DS18B20Info = ds18b20_malloc(); // heap allocation
            Temperature_sapt_Devices[Temperature_i_DeviceIndex] = Temperature_pt_DS18B20Info;
            Temperature_sapt_Devices[Temperature_i_DeviceIndex]->bus = Temperature_spt_OWB;
            ds18b20_init(Temperature_pt_DS18B20Info, Temperature_spt_OWB, Temperature_t_DeviceROMCodes[Temperature_i_DeviceIndex]); // associate with bus and device
            ds18b20_use_crc(Temperature_pt_DS18B20Info, true);                                                                      // enable CRC check on all reads
            ds18b20_set_resolution(Temperature_pt_DS18B20Info, DS18B20_RESOLUTION);
        }

        Temperature_t_Err = NVM_t_getTempOffset(&Temperature_si_Offset);
        if (Temperature_t_Err != NVM_i_OK)
        {
            ESP_LOGW(TAG, "Error reading temperature offset: error code = %d", Temperature_t_Err);
            Temperature_si_Offset = TEMPERATURE_DEFAULT_OFFSET;
        }
        else
        {
            /*empty*/
        }
    }
}

/**
 * @brief Search for a device rom-code in the array of devices.
 *        Returns TEMPERATURE_i_SUCCESS in case it is found.
 * 
 * @param Temperature_pc_ROMCode The rom_code to be searched
 * @param Temperature_pi_DeviceNumber The position of the searched device in the array devices[]
 * @return temperature_err_t  
 */
temperature_err_t Temperature_t_findDevice(char *Temperature_pc_ROMCode, int *Temperature_pi_DeviceNumber)
{
    bool Temperature_b_FoundDevice = false;
    char Temperature_ac_CurrentROMCodeString[17];
    int Temperature_i_DeviceIndex;
    for (Temperature_i_DeviceIndex = 0; Temperature_i_DeviceIndex < Temperature_vi_NumDevices; Temperature_i_DeviceIndex++)
    {
        owb_string_from_rom_code(Temperature_sapt_Devices[Temperature_i_DeviceIndex]->rom_code, Temperature_ac_CurrentROMCodeString, sizeof(Temperature_ac_CurrentROMCodeString));
        if (strcmp(Temperature_ac_CurrentROMCodeString, Temperature_pc_ROMCode) == 0)
        {
            *Temperature_pi_DeviceNumber = Temperature_i_DeviceIndex;
            Temperature_b_FoundDevice = true;
            Temperature_i_DeviceIndex = Temperature_vi_NumDevices;
        }
    }
    if (Temperature_b_FoundDevice == false)
    {
        Temperature_si_OUT = TEMPERATURE_DEFAULT;
        Temperature_si_INT = TEMPERATURE_DEFAULT;
        return TEMPERATURE_i_DEVICE_NOT_CONNECTED;
    }
    else
    {
        return TEMPERATURE_i_SUCCESS;
    }
}

/**
 * @brief Read the interior sensor's temperature if connected to the one-wire bus.
 *        Returns TEMPERATURE_i_SUCCESS in case of success.
 * 
 * @return temperature_err_t  
 */
temperature_err_t Temperature_t_Read_Interior()
{
    int Temperature_i_InteriorDeviceIndex;
    int32_t Temperature_i_TemperatureValue;
    float Temperature_f_ReadTemperature = 12.0;
    DS18B20_ERROR Temperature_t_TemperatureReadingError;
    temperature_err_t Temperature_t_DeviceError;
    Temperature_t_DeviceError = Temperature_t_findDevice(Temperature_sac_InteriorSensor, &Temperature_i_InteriorDeviceIndex);
    if (Temperature_t_DeviceError == TEMPERATURE_i_SUCCESS)
    {
        Temperature_t_TemperatureReadingError = ds18b20_convert_and_read_temp(Temperature_sapt_Devices[Temperature_i_InteriorDeviceIndex], &Temperature_f_ReadTemperature);
        if (Temperature_t_TemperatureReadingError == DS18B20_OK)
        {
            Temperature_i_TemperatureValue = (int32_t)(Temperature_f_ReadTemperature * 10);
            Temperature_si_INT = Temperature_i_TemperatureValue;
            return TEMPERATURE_i_SUCCESS;
        }
        else
        {
            Temperature_si_INT = TEMPERATURE_DEFAULT;
            return TEMPERATURE_i_CAN_NOT_READ_TEMPERATURE;
        }
    }
    else
    {
        Temperature_si_INT = TEMPERATURE_DEFAULT;
        return TEMPERATURE_i_UNKNOWN_DEVICE;
    }
}

/**
 * @brief Read the exterior sensor's temperature if connected to the one-wire bus.
 *        Returns TEMPERATURE_i_SUCCESS in case of success.
 * 
 * @return temperature_err_t  
 */
temperature_err_t Temperature_t_Read_Exterior()
{
    int Temperature_i_ExteriorDeviceIndex;
    int32_t Temperature_i_TemperatureValue;
    float Temperature_f_ReadTemperature = 12.0;
    DS18B20_ERROR Temperature_t_TemperatureReadingError;
    temperature_err_t Temperature_t_DeviceError;

    Temperature_t_DeviceError = Temperature_t_findDevice(Temperature_sac_ExteriorsensoS, &Temperature_i_ExteriorDeviceIndex);
    if (Temperature_t_DeviceError == TEMPERATURE_i_SUCCESS)
    {
        Temperature_t_TemperatureReadingError = ds18b20_convert_and_read_temp(Temperature_sapt_Devices[Temperature_i_ExteriorDeviceIndex], &Temperature_f_ReadTemperature);
        if (Temperature_t_TemperatureReadingError == DS18B20_OK)
        {
            Temperature_i_TemperatureValue = (int32_t)(Temperature_f_ReadTemperature * 10);
            Temperature_si_OUT = Temperature_i_TemperatureValue;
            return TEMPERATURE_i_SUCCESS;
        }
        else
        {
            Temperature_si_OUT = TEMPERATURE_DEFAULT;
            return TEMPERATURE_i_CAN_NOT_READ_TEMPERATURE;
        }
    }
    else
    {
        Temperature_si_OUT = TEMPERATURE_DEFAULT;
        return TEMPERATURE_i_UNKNOWN_DEVICE;
    }
}

/**
 * @brief Filter the chosen devices' temperature.
 * 
 * @param Temperature_b_ExteriorVSInteriorDevice Variable which tells the function if the filtering is for the interior temperature or the exterior temperature
 */
void Temperature_v_FilterDeviceTemperature(bool Temperature_b_ExteriorVSInteriorDevice)
{
    int Temperature_i_DeviceIndex;
    int32_t Temperature_i_Value;
    if (Temperature_b_ExteriorVSInteriorDevice == true)
    {
        Temperature_i_DeviceIndex = 1;
        Temperature_i_Value = Temperature_si_OUT;
    }
    else
    {
        Temperature_i_DeviceIndex = 0;
        Temperature_i_Value = Temperature_si_INT;
    }
    Temperature_sai_SumTemperature[Temperature_i_DeviceIndex] = Temperature_sai_SumTemperature[Temperature_i_DeviceIndex] - Temperature_sai_ReadingsTemperature[Temperature_i_DeviceIndex][Temperature_si_IndexTemperature[Temperature_i_DeviceIndex]]; // Remove the oldest entry from the sum
    Temperature_sai_ReadingsTemperature[Temperature_i_DeviceIndex][Temperature_si_IndexTemperature[Temperature_i_DeviceIndex]] = Temperature_i_Value;                                                                                                   // Add the newest reading to the window
    Temperature_sai_SumTemperature[Temperature_i_DeviceIndex] = Temperature_sai_SumTemperature[Temperature_i_DeviceIndex] + Temperature_i_Value;                                                                                                        // Add the newest reading to the sum
    Temperature_si_IndexTemperature[Temperature_i_DeviceIndex] = (Temperature_si_IndexTemperature[Temperature_i_DeviceIndex] + 1) % WINDOW_SIZE;                                                                                                        // Increment the index, and wrap to 0 if it exceeds the window size
    Temperature_si_AverageTempValue[Temperature_i_DeviceIndex] = Temperature_sai_SumTemperature[Temperature_i_DeviceIndex] / WINDOW_SIZE;                                                                                                               // Divide the sum of the window by the window size for the result
}

/**
 * @brief Filter the given value
 *        Returns TEMPERATURE_i_SUCCESS in case the value is inside the chosen interval.
 *        Returns TEMPERATURE_i_OUTSIDE_INTERVAL in case the value is outside the interval.
 * 
 * @param Temperature_i_Value The value to be filtered
 * @return temperature_err_t  
 */
temperature_err_t Temperature_t_CheckValidity(int32_t Temperature_i_Value)
{
    if (Temperature_i_Value < MINIMUM_ADMISSIBLE_TEMPERATURE || Temperature_i_Value > MAXIMUM_ADMISSIBLE_TEMPERATURE)
    {
        return TEMPERATURE_i_OUTSIDE_INTERVAL;
    }
    else
    {
        return TEMPERATURE_i_SUCCESS;
    }
}

/**
 * @brief Convert a given value to a multiple of 5.
 *        Returns the value as an integer.
 * 
 * @param Temperature_i_Value The value to be converted.
 * @return int32_t
 */
int32_t Temperature_i_ConvertToMult5(int32_t Temperature_i_Value)
{
    int32_t Temperature_i_NewValue;
    int32_t div5;
    int32_t mod = 0;
    int32_t mod5;

    div5 = (int32_t)(Temperature_i_Value / 5);
    mod5 = Temperature_i_Value % 5;
    if (mod5 == 3 || mod5 == 4)
    {
        mod = 1;
    }
    else
    {
        /*empty*/
    }
    Temperature_i_NewValue = div5 * 5 + mod * 5;

    return Temperature_i_NewValue;
}

/**
 * @brief Verifies if the new temperature has a difference of at least HYST_TRIGGER_LEVEL celsius degrees from the old temperature.
 *          Returns the current temperature value when the difference of values is valid.
 * 
 * @param Temperature_i_PreviousTemperature The old temperature
 * @param Temperature_i_CurrentTemperature The current temperature
 * @return int32_t
 */
int32_t Temperature_i_Hysteresis(int32_t Temperature_i_PreviousTemperature, int32_t Temperature_i_CurrentTemperature)
{
    int32_t Temperature_i_ReturnedTemperature;
    if (abs(Temperature_i_CurrentTemperature - Temperature_i_PreviousTemperature) >= HYST_TRIGGER_LEVEL)
    // verifying if the newly read temperature is 0.5 C lower or higher than the previously read temperature
    {
        Temperature_i_ReturnedTemperature = Temperature_i_CurrentTemperature;
        // if the newly read temperature is higher or lower with 0.5 C, it is considered as the new temperature
    }
    else
    {
        Temperature_i_ReturnedTemperature = Temperature_i_PreviousTemperature;
        // if the newly read temperature is not higher or lower with 0.5 C, the previously measured temperature is kept as current temperature
    }

    return Temperature_i_ReturnedTemperature;
}

/**
 * @brief Read the sensor's temperatures.
 *        In case debug mode os one, it showcases the read values of the temperature.
 * 
 */
void Temperature_v_Read_Temperature()
{
    if (Temperature_vi_NumDevices == 0)
    {
        ESP_LOGW(TAG, "NO DEVICES CONNECTED!");
        int Temperature_i_Value = TEMPERATURE_DEFAULT;
        Temperature_si_INT = TEMPERATURE_DEFAULT;
        Temperature_si_OUT = TEMPERATURE_DEFAULT;
        MQTT_v_SendUpdates(MQTT_i_SEND_TEMP_IN, Temperature_i_Value);
        MQTT_v_SendUpdates(MQTT_i_SEND_TEMP_OUT, Temperature_i_Value);
    }
    else
    {
#ifdef DEBUG_TEMPERATURE
        if (DEBUG_TEMPERATURE == 1)
        {
            int32_t Temperature_i_Value, Temperature_i_RawTemperature;
            temperature_err_t Temperature_t_DeviceReadingErrorExt, Temperature_t_DeviceReadingErrorInt;
            temperature_err_t Temperature_t_FilterError;
            Temperature_t_DeviceReadingErrorInt = Temperature_t_Read_Interior(); ///< read interior temperature and check if errors
            Temperature_t_DeviceReadingErrorExt = Temperature_t_Read_Exterior(); ///< read exterior temperature and check if errors
            if (Temperature_t_DeviceReadingErrorInt == TEMPERATURE_i_SUCCESS)
            {
                Temperature_v_FilterDeviceTemperature(false);
                Temperature_i_RawTemperature = Temperature_i_Hysteresis(Temperature_si_PrevReadTemperatureInt, Temperature_i_getInterior());
                Temperature_i_Value = Temperature_i_ConvertToMult5(Temperature_i_RawTemperature);
                Temperature_t_FilterError = Temperature_t_CheckValidity(Temperature_i_Value);
                if (Temperature_t_FilterError == TEMPERATURE_i_SUCCESS)
                {
                    if (Temperature_si_PrevReadTemperatureInt != Temperature_i_Value - Temperature_si_Offset)
                    {
                        Temperature_si_PrevReadTemperatureInt = Temperature_i_Value - Temperature_si_Offset;
                        MQTT_v_SendUpdates(MQTT_i_SEND_TEMP_IN, Temperature_si_PrevReadTemperatureInt);
                        ESP_LOGI(TAG, "      SENT TO MQTT INTERIOR!!!");
                    }

                    Temperature_i_Interior = Temperature_i_Value - Temperature_si_Offset;
                    Temperature_si_INT = Temperature_i_Interior;
                    ESP_LOGI(TAG, "Interior temperature: %d", Temperature_i_Interior);
                }
                else
                {
                    Temperature_i_Interior = TEMPERATURE_DEFAULT;
                    Temperature_si_INT = TEMPERATURE_DEFAULT;
                    ESP_LOGI(TAG, "Interior temperature outside the interval!");
                }
            }
            else
            {
                Temperature_i_Interior = TEMPERATURE_DEFAULT;
                Temperature_si_INT = TEMPERATURE_DEFAULT;
                ESP_LOGI(TAG, "Can not read interior temperature");
            }

            if (Temperature_t_DeviceReadingErrorExt == TEMPERATURE_i_SUCCESS)
            {
                Temperature_v_FilterDeviceTemperature(true);
                Temperature_i_RawTemperature = Temperature_i_Hysteresis(Temperature_si_PrevReadTemperatureExt, Temperature_i_getExterior());
                Temperature_i_Value = Temperature_i_ConvertToMult5(Temperature_i_RawTemperature);
                Temperature_t_FilterError = Temperature_t_CheckValidity(Temperature_i_Value);
                // the currently read temperature is stored in a variable in order to keep it as the previous temperature for the next reading process
                if (Temperature_t_FilterError == TEMPERATURE_i_OUTSIDE_INTERVAL)
                {
                    Temperature_i_Exterior = TEMPERATURE_DEFAULT;
                    Temperature_si_OUT = TEMPERATURE_DEFAULT;
                    ESP_LOGI(TAG, "Exterior temperature is outside the interval");
                }
                else
                {
                    if (Temperature_si_PrevReadTemperatureExt != Temperature_i_Value - Temperature_si_Offset)
                    {
                        Temperature_si_PrevReadTemperatureExt = Temperature_i_Value - Temperature_si_Offset;
                        MQTT_v_SendUpdates(MQTT_i_SEND_TEMP_OUT, Temperature_si_PrevReadTemperatureExt);
                        ESP_LOGI(TAG, "      SENT TO MQTT EXTERIOR!!!");
                    }

                    Temperature_i_Exterior = Temperature_i_Value - Temperature_si_Offset;
                    Temperature_si_OUT = Temperature_i_Exterior;
                    ESP_LOGI(TAG, "Exterior temperature: %d", Temperature_i_Exterior);
                }
            }
            else
            {
                Temperature_i_Exterior = TEMPERATURE_DEFAULT;
                Temperature_si_OUT = TEMPERATURE_DEFAULT;
                ESP_LOGI(TAG, "Can not read exterior temperature");
            }
        }
        else
        {
            int32_t Temperature_i_Value, Temperature_i_RawTemperature;
            temperature_err_t Temperature_t_DeviceReadingErrorExt, Temperature_t_DeviceReadingErrorInt;
            temperature_err_t Temperature_t_FilterError;
            Temperature_t_DeviceReadingErrorInt = Temperature_t_Read_Interior(); ///< read interior temperature and check if errors
            Temperature_t_DeviceReadingErrorExt = Temperature_t_Read_Exterior(); ///< read exterior temperature and check if errors
            if (Temperature_t_DeviceReadingErrorInt == TEMPERATURE_i_SUCCESS)
            {
                Temperature_v_FilterDeviceTemperature(false);
                Temperature_i_RawTemperature = Temperature_i_Hysteresis(Temperature_si_PrevReadTemperatureInt, Temperature_i_getInterior());
                Temperature_i_Value = Temperature_i_ConvertToMult5(Temperature_i_RawTemperature);
                Temperature_t_FilterError = Temperature_t_CheckValidity(Temperature_i_Value);
                if (Temperature_t_FilterError == TEMPERATURE_i_SUCCESS)
                {
                    if (Temperature_si_PrevReadTemperatureInt != Temperature_i_Value - Temperature_si_Offset)
                    {
                        Temperature_si_PrevReadTemperatureInt = Temperature_i_Value - Temperature_si_Offset;
                        MQTT_v_SendUpdates(MQTT_i_SEND_TEMP_IN, Temperature_si_PrevReadTemperatureInt);
                    }

                    Temperature_i_Interior = Temperature_i_Value - Temperature_si_Offset;
                    Temperature_si_INT = Temperature_i_Interior;
                }
                else
                {
                    Temperature_i_Interior = TEMPERATURE_DEFAULT;
                    Temperature_si_INT = TEMPERATURE_DEFAULT;
                }
            }
            else
            {
                Temperature_i_Interior = TEMPERATURE_DEFAULT;
                Temperature_si_INT = TEMPERATURE_DEFAULT;
            }

            if (Temperature_t_DeviceReadingErrorExt == TEMPERATURE_i_SUCCESS)
            {
                Temperature_v_FilterDeviceTemperature(true);
                Temperature_i_RawTemperature = Temperature_i_Hysteresis(Temperature_si_PrevReadTemperatureExt, Temperature_i_getExterior());
                Temperature_i_Value = Temperature_i_ConvertToMult5(Temperature_i_RawTemperature);
                Temperature_t_FilterError = Temperature_t_CheckValidity(Temperature_i_Value);
                // the currently read temperature is stored in a variable in order to keep it as the previous temperature for the next reading process
                if (Temperature_t_FilterError == TEMPERATURE_i_OUTSIDE_INTERVAL)
                {
                    Temperature_i_Exterior = TEMPERATURE_DEFAULT;
                    Temperature_si_OUT = TEMPERATURE_DEFAULT;
                }
                else
                {
                    if (Temperature_si_PrevReadTemperatureExt != Temperature_i_Value - Temperature_si_Offset)
                    {
                        Temperature_si_PrevReadTemperatureExt = Temperature_i_Value - Temperature_si_Offset;
                        MQTT_v_SendUpdates(MQTT_i_SEND_TEMP_OUT, Temperature_si_PrevReadTemperatureExt);
                    }

                    Temperature_i_Exterior = Temperature_i_Value - Temperature_si_Offset;
                    Temperature_si_OUT = Temperature_i_Exterior;
                }
            }
            else
            {
                Temperature_i_Exterior = TEMPERATURE_DEFAULT;
                Temperature_si_OUT = TEMPERATURE_DEFAULT;
            }
        }
#endif
    }
}

/* Exported functions */
/*============================================================================*/
/**
 * @brief Returns the interior temperature value.
 * 
 * @return int32_t  
 */
int32_t Temperature_i_getInterior()
{
    return Temperature_si_AverageTempValue[0];
}

/**
 * @brief Returns the exterior temperature value.
 * 
 * @return int32_t  
 */
int32_t Temperature_i_getExterior()
{
    return Temperature_si_AverageTempValue[1];
}