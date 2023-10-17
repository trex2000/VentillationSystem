/**
 * @file MQTT_Integration.c
 * @author Baleanu Sorina (sorina.baleanu@altran.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-16
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
 * $Source: MQTT_Integration.c $
 * $Revision: 1 $
 * $Author: Baleanu Sorina $
 * $Date: 2021.08.16 $
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file
    short description in one sentence end with dot.
    detailed
    multiline
    description of the file
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
 * $Log: MQTT_Integration.c  $
  ============================================================================*/

/* Includes */
/*============================================================================*/
#include "MQTT_Config.h"
#include "MQTT_Integration.h"
#include "MQTT_Interface.h"
#include <esp_event.h>
#include <esp_log.h>
#include "mqtt_client.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <Comfort_Interface.h>


/* Constants and types  */
/*============================================================================*/
/* Default string size for constructed strings */
#define MQTT_DEFAULT_STRING_SIZE 32

/* String size for error constructed strings */
#define MQTT_ERROR_STRING_SIZE 2048

/* Quality of service for MQTT */
#define MQTT_QOS 1

/*  Retain value for MQTT. */
#define MQTT_RETAIN 1

/* Data lenght for MQTT. 0 data lenght means it will be calculated automaticallty,  */
#define MQTT_DATA_LENGHT 0

/* Value to convert temperature to a float. */
#define MQTT_TEMP_CONVERT 10

/* Tag for ESP debug logs  */
static const char *TAG = "MQTT";

/* Variables */
/*============================================================================*/
static esp_mqtt_client_handle_t MQTT_t_client; ///<MQTT client handle

/* Private functions prototypes */
/*============================================================================*/

/* Inline functions */
/*============================================================================*/

/* Private functions */
/*============================================================================*/

/**
 * @brief The event handler for MQTT events.
 * 
 * @param handler_args Event handler argument
 * @param base The unique pointer to a subsystem that exposes events.
 * @param MQTT_i_event_id Id of the MQTT event.
 * @param MQTT_pt_event_data Data from the MQTT event.
 */
static void MQTT_EventHandler(void *v_handler_args, esp_event_base_t t_base, int32_t i_event_id, void *pt_event_data)
{
  /*Extract the client handle from MQTT event data recieved via event handler */
 esp_mqtt_event_handle_t t_event = pt_event_data;

  switch (i_event_id)
  {
    case MQTT_EVENT_CONNECTED:
    {
      /* In case the client has successfully established a connection to the broker all sensors will sent discovery topics and payloads */
      ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");

      /* Discovery topic and payload for humidity sensor */
      const char caub_humidity_topic[] = "homeassistant/sensor/humidity_status/config";
      const char caub_humidity_payload[] = "{\n"
      "\"name\": \"Humidity\",\n"
      "\"state_topic\": \"sensor/humidity\",\n"
      "\"uniq_id\": \"humidity\",\n"
      "\"unit_of_measurement\": \"%\",\n"
      "\"value_template\": \"{{value_json.humidity}}\"}";

      /* Publish discovery topic and payload for humidity sensor */
      esp_mqtt_client_publish(MQTT_t_client, caub_humidity_topic, caub_humidity_payload, MQTT_DATA_LENGHT, MQTT_QOS , MQTT_RETAIN);

      /* Discovery topic and payload for inside temperature sensor */
      const char caub_temperature1_topic[] = "homeassistant/sensor/temperature_in_status/config";
      const char caub_temperature1_payload[] = "{\n"
      "   \"name\": \"Temperature inside\", \n"
      "   \"state_topic\": \"sensor/temperature_in\",\n"
      "   \"uniq_id\": \"temperature_in\",\n"
      "   \"unit_of_measurement\": \"°C\",\n"
      "   \"value_template\": \"{{value_json.temperature}}\"} ";                                                      
                                                                                                                                                      
      /* Publish discovery topic and payload for inside temperature sensor */
      esp_mqtt_client_publish(MQTT_t_client, caub_temperature1_topic, caub_temperature1_payload, MQTT_DATA_LENGHT, MQTT_QOS , MQTT_RETAIN);
      
      /* Discovery topic and payload for outside temperature sensor */
      const char caub_temperature2_topic[] = "homeassistant/sensor/temperature_out_status/config";
      const char caub_temperature2_payload[] = "{\n"
      "  \"name\": \"Temperature outside\",\n"                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
      "  \"state_topic\": \"sensor/temperature_out\",\n"
      "  \"uniq_id\": \"temperature_out\",\n"
      "  \"unit_of_measurement\": \"°C\",\n"
      "  \"value_template\": \"{{value_json.temperature}}\"}";

      /* Publish discovery topic and payload for outside temperature sensor */
      esp_mqtt_client_publish(MQTT_t_client, caub_temperature2_topic, caub_temperature2_payload, MQTT_DATA_LENGHT, MQTT_QOS , MQTT_RETAIN);

      /* Discovery topic and payload for fans */
      const char caub_fan_topic[] = "homeassistant/fan/fan_modes/config";
      const char caub_fan_payload[] = "{\n"
      "  \"name\": \"Fans\",\n"
      "  \"uniq_id\": \"fans\",\n"
      "  \"command_topic\": \"fan/on/set\",\n"
      "  \"command_template\": \"{{ value }}\",\n"
      "  \"preset_mode_command_topic\": \"fan/preset/command\",\n"
      "  \"preset_mode_command_template\": \"{{ value }}\",\n"
      "  \"preset_modes\": [\n"
      "    \"recycling\",\n"
      "    \"non-recycling\",\n"
      "    \"in\",\n"
      "    \"out\",\n"
      "    \"auto\"\n"
      "  ],\n"
      "  \"percentage_command_topic\": \"fan/percentage/command\",\n"
      "  \"percentage_command_template\": \"{{ value}}\"\n"
      "}";

      /* Publish discovery topic and payload for fans*/
      esp_mqtt_client_publish(MQTT_t_client, caub_fan_topic, caub_fan_payload, MQTT_DATA_LENGHT, MQTT_QOS , MQTT_RETAIN);

      /* Discovery topic and payload for error management module */
      const char caub_error_topic[] = "homeassistant/sensor/message_manager/config";
      const char caub_error_payload[] = "{\n"
      "  \"name\": \"Messages\",\n"
      "  \"state_topic\": \"sensor/messages\",\n"
      "  \"uniq_id\": \"messages\",\n"
      "  \"unit_of_measurement\": \"\",\n"
      "  \"value_template\": \"{{value_json.message}}\"\n"
      "}";

      /* Publish discovery topic and payload for fans*/
      esp_mqtt_client_publish(MQTT_t_client, caub_error_topic, caub_error_payload, MQTT_DATA_LENGHT, MQTT_QOS , MQTT_RETAIN);

      #if MQTT_DEBUG_FLAG == 1
        ESP_LOGD(TAG, "Sent publish was successful");
      #endif
     
      /* Topic for turning on and off the fans  */
      const char caub_fan_powering_topic[] = "fan/on/set";

      /* Suscribe to topic for turning on and off the fans */
      esp_mqtt_client_subscribe(MQTT_t_client, caub_fan_powering_topic , MQTT_DATA_LENGHT);

      /* Topic for setting operating mode the fans  */
      const char caub_fan_set_mode_topic[] = "fan/preset/command";

      /* Suscribe to topic for setting operating mode the fans */
      esp_mqtt_client_subscribe(MQTT_t_client, caub_fan_set_mode_topic , MQTT_DATA_LENGHT);

      /* Topic for setting the speed of the fans */
      const char caub_fan_set_speed_topic[] = "fan/percentage/command";

      /* Subscribe t topic for setting the speed of the fans */
      esp_mqtt_client_subscribe(MQTT_t_client, caub_fan_set_speed_topic, MQTT_DATA_LENGHT);

      #if MQTT_DEBUG_FLAG == 1
        ESP_LOGD(TAG, "Subscribed to fan topics successfully");
      #endif

      break;
    }
    case MQTT_EVENT_DISCONNECTED:
    {
      #if MQTT_DEBUG_FLAG == 1
        ESP_LOGD(TAG, "MQTT_EVENT_DISCONNECTED");
      #endif

      break;
    }
    case MQTT_EVENT_DATA:
    {
      #if MQTT_DEBUG_FLAG == 1
        ESP_LOGD(TAG, "MQTT_EVENT_DATA");
      #endif
   
      char aub_payload_from_mqtt[32];

      /* Copy the data recieved from the event into a new string*/
      strncpy(aub_payload_from_mqtt, t_event->data, t_event->data_len);

      /* String must be ended in the its its specific null character*/
      aub_payload_from_mqtt[t_event->data_len] = '\0';
      
      /*  Send string with the command recieved from mqtt to be processed by the comfort management module*/
      COMF_v_recieveMQTTCommand(aub_payload_from_mqtt);

      break;
    }
          
  }
}

/**
 * @brief Function for configuring the MQTT client and registering a handler callback for the MQTT events  
 */
static void MQTT_v_ConfigClient(void)
{
  /* MQTT client configuration with the MQTT broker URI, username and password */
  esp_mqtt_client_config_t t_cfg =
  {
    .uri = MQTT_CONFIG_BROKER_URL,
    .username = MQTT_CONFIG_USERNAME,
    .password = MQTT_CONFIG_PASS,
  };

  /* Create a mqtt client handle based on the configuration. */
  MQTT_t_client = esp_mqtt_client_init(&t_cfg);

  /* Register mqtt event on client with a handler callback*/
  esp_mqtt_client_register_event(MQTT_t_client, ESP_EVENT_ANY_ID, MQTT_EventHandler, NULL);

  /* Starts mqtt client with already created client handle. */
  esp_mqtt_client_start(MQTT_t_client);

}


/* Exported functions */
/*============================================================================*/
/**
 * @brief Initi function for MQTT to be called in RTOS_API.c * 
 */
void MQTT_v_Init(void)
{
  /*Configure and start MQTT Client */
  MQTT_v_ConfigClient();

}

/**
 * @brief The function sends updates from sensors to Home Assistant via MQTT.
 * 
 * @param t_sent_from_module The module that sends the update usefull for chosing the topic.
 * @param sensor_value The value to be sent from the modules
 */
void MQTT_v_SendUpdates(mqtt_sent_t t_sent_from_module, uint32_t sensor_value)
{
  switch(t_sent_from_module)
  {
    case MQTT_i_SEND_TEMP_IN:
    {
      /* Topic for inside temperature sensor*/
      const char caub_temperature_in_topic[] = "sensor/temperature_in";

      /* Inside temperature payload */
      const char caub_payload_part1[]= "{\n  \"temperature\": ";

      /* Convert the inside temperature data from sensor into a string.*/
      char aub_temperature_in_decimal[MQTT_DEFAULT_STRING_SIZE];
      sprintf(aub_temperature_in_decimal,"%d", sensor_value / MQTT_TEMP_CONVERT);

      /* Convert the inside temperature data from sensor into a string.*/
      char aub_temperature_in_fractional[MQTT_DEFAULT_STRING_SIZE];
      sprintf(aub_temperature_in_fractional,"%d", sensor_value % MQTT_TEMP_CONVERT);
      const char  caub_payload_part2[] = " \n}";
      
      char aub_payload_temperature_in[MQTT_DEFAULT_STRING_SIZE];
      
      /* Concatenate the payload string.*/
      strcpy(aub_payload_temperature_in , caub_payload_part1);
      strcat(aub_payload_temperature_in , aub_temperature_in_decimal);
      strcat(aub_payload_temperature_in , ".");
      strcat(aub_payload_temperature_in , aub_temperature_in_fractional);
      strcat(aub_payload_temperature_in , caub_payload_part2);

      /* Publish topic and payload for inside temperature sensor */
      esp_mqtt_client_publish(MQTT_t_client, caub_temperature_in_topic , aub_payload_temperature_in , MQTT_DATA_LENGHT, MQTT_QOS , MQTT_RETAIN);

      #if MQTT_DEBUG_FLAG == 1
        ESP_LOGD(TAG, "Sent in temperature update");
      #endif

      break;
    }
    case MQTT_i_SEND_TEMP_OUT:
    {
      /* Topic for outside temperature sensor*/
      const char caub_temperature_out_topic[] = "sensor/temperature_out";

      /* Outside temperature payload */
      const char caub_payload_part1[]= "{\n  \"temperature\": ";

      /* Convert the outside temperature data from sensor into a string.*/
      char aub_temperature_out_decimal[MQTT_DEFAULT_STRING_SIZE];
      sprintf(aub_temperature_out_decimal,"%d", sensor_value / MQTT_TEMP_CONVERT);

      /* Convert the outside temperature data from sensor into a string.*/
      char aub_temperature_out_fractional[MQTT_DEFAULT_STRING_SIZE];
      sprintf(aub_temperature_out_fractional,"%d", sensor_value % MQTT_TEMP_CONVERT);
      const char  caub_payload_part2[] = " \n}";
      
      char aub_payload_temperature_out[32];
      
      /* Concatenate the payload string.*/
      strcpy(aub_payload_temperature_out , caub_payload_part1);
      strcat(aub_payload_temperature_out , aub_temperature_out_decimal);
      strcat(aub_payload_temperature_out , ".");
      strcat(aub_payload_temperature_out , aub_temperature_out_fractional);
      strcat(aub_payload_temperature_out ,caub_payload_part2);

      /* Publish topic and payload for outside temperature sensor */
      esp_mqtt_client_publish(MQTT_t_client, caub_temperature_out_topic , aub_payload_temperature_out , MQTT_DATA_LENGHT, MQTT_QOS, MQTT_RETAIN);

      #if MQTT_DEBUG_FLAG == 1
        ESP_LOGD(TAG, "Sent out temperature update");
      #endif

      break;
    }
    case MQTT_i_SEND_HUMIDITY:
    {
      /* Topic for humidity sensor*/
      const char caub_humidity_topic[] = "sensor/humidity";

      /* Humidity payload */
      const char caub_payload_part1[]= "{\n  \"humidity\": ";

      /* Convert the humidity data from sensor into a string.*/
      char aub_humidity[MQTT_DEFAULT_STRING_SIZE];
      sprintf(aub_humidity,"%d", sensor_value);
      const char  caub_payload_part2[] = " \n}";
      
      char aub_payload_humidity[MQTT_DEFAULT_STRING_SIZE];
      
      /* Concatenate the payload string.*/
      strcpy(aub_payload_humidity, caub_payload_part1);
      strcat(aub_payload_humidity , aub_humidity);
      strcat(aub_payload_humidity,caub_payload_part2);

      /* Publish topic and payload for humidity sensor */
      esp_mqtt_client_publish(MQTT_t_client, caub_humidity_topic , aub_payload_humidity , MQTT_DATA_LENGHT, MQTT_QOS, MQTT_RETAIN);

      
      #if MQTT_DEBUG_FLAG == 1
        ESP_LOGD(TAG, "Sent humidity update");
      #endif

      break;
    }
  }
}
 /**
  * @brief Function for sending error or warning from the error management error module to Home assistant via MQTT.
  * 
  * @param aub_error_text Error or warning text
  */
void MQTT_v_SendErrorsFan(char* aub_error_text)
{
    /* Topic for error management module */
    const char caub_error_topic[] = "sensor/errfan1";

    /* Error message payload */
    const char caub_payload_part1[]= "{\n  \"message\": ";
    const char  caub_payload_part2[] = " \n}";
      
    char aub_payload_error[MQTT_ERROR_STRING_SIZE];
      
    /* Concatenate the payload string.*/
    strcpy(aub_payload_error, caub_payload_part1);
    strcat(aub_payload_error , aub_error_text);
    strcat(aub_payload_error,caub_payload_part2);

    /* Publish topic and payload for fan error */
    esp_mqtt_client_publish(MQTT_t_client, caub_error_topic , aub_payload_error , MQTT_DATA_LENGHT, MQTT_QOS, MQTT_RETAIN);

    #if MQTT_DEBUG_FLAG == 1
      ESP_LOGD(TAG, "Sent error update");
    #endif
}

/**
  * @brief Function for sending error or warning from the error management error module to Hopme assistant via MQTT.
  * 
  * @param aub_error_text Error or warning text
  */
void MQTT_v_SendErrorsTemp(char* aub_error_text)
{
    /* Topic for error management module */
    const char caub_error_topic[] = "sensor/errtemp1";

    /* Error message payload */
    const char caub_payload_part1[]= "{\n  \"message\": ";
    const char  caub_payload_part2[] = " \n}";
      
    char aub_payload_error[MQTT_ERROR_STRING_SIZE];
      
    /* Concatenate the payload string.*/
    strcpy(aub_payload_error, caub_payload_part1);
    strcat(aub_payload_error , aub_error_text);
    strcat(aub_payload_error,caub_payload_part2);

    /* Publish topic and payload for temperature error*/
    esp_mqtt_client_publish(MQTT_t_client, caub_error_topic , aub_payload_error , MQTT_DATA_LENGHT, MQTT_QOS, MQTT_RETAIN);

    #if MQTT_DEBUG_FLAG == 1
      ESP_LOGD(TAG, "Sent error update");
    #endif
}

/**
  * @brief Function for sending error or warning from the error management error module to Hopme assistant via MQTT.
  * 
  * @param aub_error_text Error or warning text
  */
void MQTT_v_SendErrorsHumi(char* aub_error_text)
{
    /* Topic for error management module */
    const char caub_error_topic[] = "sensor/errhumi1";

    /* Error message payload */
    const char caub_payload_part1[]= "{\n  \"message\": ";
    const char  caub_payload_part2[] = " \n}";
      
    char aub_payload_error[MQTT_ERROR_STRING_SIZE];
      
    /* Concatenate the payload string.*/
    strcpy(aub_payload_error, caub_payload_part1);
    strcat(aub_payload_error , aub_error_text);
    strcat(aub_payload_error,caub_payload_part2);

    /* Publish topic and payload for humidity error */
    esp_mqtt_client_publish(MQTT_t_client, caub_error_topic , aub_payload_error , MQTT_DATA_LENGHT, MQTT_QOS, MQTT_RETAIN);

    #if MQTT_DEBUG_FLAG == 1
      ESP_LOGD(TAG, "Sent error update");
    #endif
}

/**
  * @brief Function for sending error or warning from the error management error module to Hopme assistant via MQTT.
  * 
  * @param aub_error_text Error or warning text
  */
void MQTT_v_SendErrorsTime(char* aub_error_text)
{
    /* Topic for error management module */
    const char caub_error_topic[] = "sensor/errtime1";

    /* Error message payload */
    const char caub_payload_part1[]= "{\n  \"message\": ";
    const char  caub_payload_part2[] = " \n}";
      
    char aub_payload_error[MQTT_ERROR_STRING_SIZE];
      
    /* Concatenate the payload string.*/
    strcpy(aub_payload_error, caub_payload_part1);
    strcat(aub_payload_error , aub_error_text);
    strcat(aub_payload_error,caub_payload_part2);

    /* Publish topic and payload for time management error */
    esp_mqtt_client_publish(MQTT_t_client, caub_error_topic , aub_payload_error , MQTT_DATA_LENGHT, MQTT_QOS, MQTT_RETAIN);

    #if MQTT_DEBUG_FLAG == 1
      ESP_LOGD(TAG, "Sent error update");
    #endif
}

/**
  * @brief Function for sending error or warning from the error management error module to Hopme assistant via MQTT.
  * 
  * @param aub_error_text Error or warning text
  */
void MQTT_v_SendErrorsWifi(char* aub_error_text)
{
    /* Topic for error management module */
    const char caub_error_topic[] = "sensor/errwifi1";

    /* Error message payload */
    const char caub_payload_part1[]= "{\n  \"message\": ";
    const char  caub_payload_part2[] = " \n}";
      
    char aub_payload_error[MQTT_ERROR_STRING_SIZE];
      
    /* Concatenate the payload string.*/
    strcpy(aub_payload_error, caub_payload_part1);
    strcat(aub_payload_error , aub_error_text);
    strcat(aub_payload_error,caub_payload_part2);

    /* Publish topic and payload for wifi error */
    esp_mqtt_client_publish(MQTT_t_client, caub_error_topic , aub_payload_error , MQTT_DATA_LENGHT, MQTT_QOS, MQTT_RETAIN);

    #if MQTT_DEBUG_FLAG == 1
      ESP_LOGD(TAG, "Sent error update");
    #endif
}
