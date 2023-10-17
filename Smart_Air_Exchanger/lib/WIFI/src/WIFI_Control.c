/**
 * @file WIFI_Control.c
 * @author Baleanu Sorina (sorina.baleanu@altran.com)
 * @brief This file contains the implementations of the functions found in the
 * interface. The private functions are used as the main executing functions, while
 * the exported ones provide a safe access to them.
 * @version 1.2
 * @date 2021-08-12
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
 * $Source: WIFI_Control.c $
 * $Revision: 1 $
 * $Author: Baleanu Sorina $
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
 * $Log: WIFI_Station.c  $
  ============================================================================*/

/* Includes */
/*============================================================================*/
#include "WIFI_Config.h"
#include "WIFI_Control.h"
#include "NVM_Interface.h"
#include "Webserver_Interface.h"
#include "ErrorManagement_Interface.h"

#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>

#include <lwip/err.h>
#include <lwip/sys.h>

#include <esp_http_server.h>

/* Constants and types  */
/*============================================================================*/

/*! Wi-fi connected successfully to the AP with an IP. */
#define WIFI_CONNECTED_BIT            BIT0 

/*! Connection failed after the maximum amount of retries. */
#define WIFI_FAIL_BIT                 BIT1     

/*! Configure Wi-fi station with credentials saved in NVS. */
#define WIFI_CONNECT_NVS_CREDENTIALS   0  

/*! Configure Wi-fi station with credentials sent via webserver. */
#define WIFI_CONNECT_FROM_AP           1 

/*! TAG for ESP logs. */
static const char *TAG = "WIFI"; 

/* Variables */
/*============================================================================*/

/*! FreeRTOS group to signal when Wi-fi is connected. */
static EventGroupHandle_t WIFI_t_event_group; 

/*! Pointer to the esp_netif object for the wifi interface in station mode. */
static esp_netif_t *WIFI_t_station; 

/*! Pointer to the esp_netif object for the wifi interface in AP mode. */
static esp_netif_t *WIFI_t_ap;

/*! Hold the current status of number of attempts to connect to the Wi-fi network. */
static int8_t WIFI_i_retry_num; 

/*! Flag for Wi-fi connection, 0 - ESP32 not connected to Wi-fi, 1 - ESP32 connected to Wi-fi. */
static int8_t WIFI_i_connected_flag;

/*! Hold the SSID for network when trying to connect via webserver. */
static char WIFI_aub_ssid[32]; 

/*! Hold the password for network when trying to connect via webserver. */
static char WIFI_aub_pass[64];  


/* Private functions prototypes */
/*============================================================================*/

static void WIFI_v_event_handler(void *v_arg, esp_event_base_t t_event_base, int32_t i_event_id, void *v_event_data);

static void WIFI_v_start_webserver(void);

static void WIFI_v_ConfigSoftAP(void);

static void WIFI_v_ConifgStation(int8_t i_credentials_from);

static esp_err_t  WIFI_v_getHandlerCredentials(httpd_req_t *req);




/* Inline functions */
/*============================================================================*/

/* Private functions */
/*============================================================================*/
/**
 * @brief The URI handler function to be called during GET /connect request.
 * Returns ESP_OK in case of success.
 * 
 * @param req The http request
 * @return esp_err_t  
 */
static esp_err_t WIFI_v_getHandlerCredentials(httpd_req_t *req)
{
    char buf[512];
    size_t buf_len;

    /* Get Query string length from the request URL . */
    buf_len = httpd_req_get_url_query_len(req) + 1;

    /* Check if there is a request */
    if (buf_len > 1)
    {
        /* Check if Query is found in the request URL and it is copied to buffer */

        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK)
        {

            ESP_LOGI(TAG, "Found URL query => %s", buf);

            /* Helper function to get a URL query tag from a query string of the type */
            if (httpd_query_key_value(buf, "ssid", WIFI_aub_ssid, sizeof(WIFI_aub_ssid)) == ESP_OK)
            {
                ESP_LOGI(TAG, "Found URL query parameter => network name = %s", WIFI_aub_ssid);
            }
            else
            {
                //empty
            }
            if (httpd_query_key_value(buf, "pass", WIFI_aub_pass, sizeof(WIFI_aub_pass)) == ESP_OK)
            {
                ESP_LOGI(TAG, "Found URL query parameter => password = %s", WIFI_aub_pass);
            }
            else
            {
                //empty
            }

            ESP_LOGI(TAG, "Switching Wifi station mode.");

            /* Destroy the esp_netif object for the Wi-fi interface in station mode before creating a new one.*/
            esp_netif_destroy(WIFI_t_station);

            /* Reinitialize number of retries with 0*/
            WIFI_i_retry_num = 0;

            /* Wi-fi will now be configured back in station operating mode with credentials sent via webserve. */
            WIFI_v_ConifgStation(WIFI_CONNECT_FROM_AP);
        }
        else
        {
            //empty
        }
    }
    else
    {
        //empty
    }

    /*HTML code for the webserver is sent as an HTTP response to the HTTP get request*/
    char data1[] = "<!DOCTYPE html><html>";
    char data2[] = "<body><h1>Provide connection credentials</h1>";
    char data3[] = "<form action=\"/connect\">";
    char data4[] = "<label for=\"ssid\">Network name:</label><input type=\"text\" id=\"ssid\" name=\"ssid\"><br><br>";
    char data5[] = "<label for=\"pass\">Password:</label><input type=\"text\" id=\"pas\" name=\"pass\"><br><br>";
    char data6[] = "<input type=\"submit\" value=\"Connect\"></form></body></html>";
    char end_data[] = "";

    httpd_resp_send_chunk(req, data1, strlen(data1));
    httpd_resp_send_chunk(req, data2, strlen(data2));
    httpd_resp_send_chunk(req, data3, strlen(data3));
    httpd_resp_send_chunk(req, data4, strlen(data4));
    httpd_resp_send_chunk(req, data5, strlen(data5));
    httpd_resp_send_chunk(req, data6, strlen(data6));
    httpd_resp_send_chunk(req, end_data, strlen(end_data));

    return ESP_OK;
}
/**
 * @brief The URI handler function to be called during GET /status request.
 * Returns ESP_OK in case of success.
 * 
 * @param req The http request
 * @return esp_err_t  
 */
esp_err_t  WIFI_v_getHandlerWebserver(httpd_req_t *req)
{
    const uint8_t *wsBuffer;
    int sizeBuffer = getWebserverBufferIndex(0, 20480, &wsBuffer);

    //ESP_LOGI("HTTP","File size is %d bytes",sizeBuffer);

    httpd_resp_send(req,(const char*)wsBuffer,sizeBuffer);
    return 0;
}

esp_err_t  WIFI_v_getHandlerErrors(httpd_req_t *req)
{
    char errorBuffer[1024] = "Errors<br>";
    unsigned int sizeOfBuffer = 0;

    /* Write to the error buffer */
    ERM_t_displayErrorsOnWebserver(errorBuffer);
    
    /* Set the size of the buffer */
    sizeOfBuffer = strlen(errorBuffer);

    httpd_resp_send(req,(const char*)errorBuffer,sizeOfBuffer);
    return 0;
}

/**
 * @brief The URI handler function to be called during GET /socket request.
 * Returns ESP_OK in case of success.
 * 
 * @param req The http request
 * @return esp_err_t  
 */
static esp_err_t  WIFI_v_getHandlerWebsocket(httpd_req_t *req)
{
    httpd_ws_frame_t wsFrame;
    uint8_t inputBuffer[256]={0};
    memset(&wsFrame, 0, sizeof(httpd_ws_frame_t));
	wsFrame.type = HTTPD_WS_TYPE_TEXT;
    wsFrame.payload=inputBuffer;

    #if WEBSOCKET_DEBUG == 1
        ESP_LOGI("Websocket","Start handler");
    #endif

    if(req->method == HTTP_GET)
    {
        #if WEBSOCKET_DEBUG == 1
            ESP_LOGI("Websocket","Handshake received");
        #endif
        return ESP_OK;
    }

    httpd_ws_recv_frame(req,&wsFrame,256);

    #if WEBSOCKET_DEBUG == 1
        ESP_LOGI("Websocket","Received frame");
    #endif



    uint8_t *buffer=wsFrame.payload;
    uint8_t dataBuffer[512];

    int commandCode,dataLen;

    sscanf((char *)buffer, "%d", &commandCode);

    #if WEBSOCKET_DEBUG == 1
        ESP_LOGI("WS command code","%d",commandCode);
    #endif

    switch (commandCode)
    {
    case FAN_SPEED_OFF:
        COMF_v_setSpeed(COMF_i_OFF_PRESET);
        break;
    case FAN_SPEED_LOW:
        COMF_v_setSpeed(COMF_i_LOW_PRESET);
        break;
    case FAN_SPEED_MED:
        COMF_v_setSpeed(COMF_i_MED_PRESET);
        break;
    case FAN_SPEED_HIGH:
        COMF_v_setSpeed(COMF_i_HIGH_PRESET);
        break;
    case FAN_MODE_REC:
        COMF_v_setMode(COMF_i_REC);
        break;
    case FAN_MODE_NONREC:
        COMF_v_setMode(COMF_i_NONREC);
        break;
    case FAN_MODE_IN_OUT:
        COMF_v_setMode(COMF_i_ITO);
        break;
    case FAN_MODE_OUT_IN:
        COMF_v_setMode(COMF_i_OTI);
        break;
    case DATA_REQUEST:
        dataLen = compileDataPacket(dataBuffer);

        #if WEBSOCKET_DEBUG == 1
            ESP_LOGI("WS response","%s",dataBuffer);
            ESP_LOGI("WS response","Size of response: %d",dataLen);
        #endif

        httpd_ws_frame_t wsframe = {.final = false,
                                    .fragmented = false,
                                    .type = HTTPD_WS_TYPE_TEXT,
                                    .payload = dataBuffer,
                                    .len = dataLen};
        
        httpd_ws_send_frame(req, &wsframe);

        break;
    default:
        break;
    }

    return ESP_OK;
}

/**
 * @brief Function for registering the URI handlers. 
 * 
 * @param server The server's handle
 */
static void WIFI_v_RegisterHandlers(httpd_handle_t server)
{
    /*URI handler structure for GET /socket */
    static const httpd_uri_t ws =
    {
        .uri = "/socket",
        .method = HTTP_GET,
        .handler =  WIFI_v_getHandlerWebsocket,
        .user_ctx = NULL,
        .is_websocket = true
    };

    /*URI handler structure for GET /status */
    static const httpd_uri_t sta = 
    {
        .uri = "/status",
        .method = HTTP_GET,
        .handler =  WIFI_v_getHandlerWebserver,
        .user_ctx = NULL,
        .is_websocket = false
    };

    /*URI handler structure for GET /connect*/
    static const httpd_uri_t credentials = 
    {
        .uri = "/connect",
        .method = HTTP_GET,
        .handler = WIFI_v_getHandlerCredentials,
        .user_ctx = NULL
    };

    /**
     * @brief URI handler structure for /error
     * 
     */
    static  const httpd_uri_t errorURI = 
    {
        .uri = "/error",
        .method = HTTP_GET,
        .handler = WIFI_v_getHandlerErrors,
        .user_ctx = NULL
    };
    
    /* Register websocket URI handler */
    httpd_register_uri_handler(server, &ws);

    /* Register station URI handler */
    httpd_register_uri_handler(server, &sta);

    /* Register credentials URI handler */
    httpd_register_uri_handler(server, &credentials);

    httpd_register_uri_handler(server, &errorURI);
    
}

/**
 * @brief Function for starting the webserver.
 * Returns the server's handle, or NULL if the server failed to start.
 */
static void WIFI_v_start_webserver(void)
{
    /* Empty handle to esp_http_server */
    httpd_handle_t t_server = NULL;

    /* Generate default configuration */
    httpd_config_t t_config = HTTPD_DEFAULT_CONFIG();
    t_config.lru_purge_enable = true;

    #if WIFI_DEBUG_FLAG == 1
        ESP_LOGD(TAG, "Starting server on port: '%d'", t_config.server_port);
    #endif

    /* Start the webserver and check if it started successfully. */
    if (httpd_start(&t_server, &t_config) == ESP_OK)
    {
        /* Register the URI handlers. */
        WIFI_v_RegisterHandlers(t_server);

        #if WIFI_DEBUG_FLAG == 1
            ESP_LOGD(TAG, "Registering URI handlers");
        #endif

    }
    else
    {
        #if WIFI_DEBUG_FLAG == 1
            ESP_LOGD(TAG, "Error starting server!");
        #endif
    }
}

/**
 * @brief The event handler registered for Wi-fi and IP events.
 * It implements the state machine based on Wi-fi and IP events.
 * 
 * @param v_arg  Event handler argument
 * @param t_event_base Unique pointer to a subsystem that exposes events
 * @param i_event_id    The id of the event
 * @param v_event_data  Data of the event
 *  
 */
static void WIFI_v_event_handler(void *v_arg, esp_event_base_t t_event_base, int32_t i_event_id, void *v_event_data)
{
    switch (i_event_id)
    {
        case WIFI_EVENT_STA_START:
        {
            /* In case Wi-fi started successfully,
            Connect the ESP32 Wi-fi station to the AP.*/
            esp_wifi_connect();
            break;
        }
        case WIFI_EVENT_STA_DISCONNECTED:
        {
            /* In case Wi-fi in station mode disconnected from the AP,
            or could not connect to the AP check if the current status of number of attempts
            to connect to the Wi-fi reached maximum number of retries*/

            if (WIFI_i_retry_num < WIFI_MAXIMUM_RETRY)
            {   
                /* Connect the ESP32 WiFi station to the AP. */
                esp_wifi_connect();

                /* Increase the number of attempts to connect to Wi-fi*/
                WIFI_i_retry_num++;

                #if WIFI_DEBUG_FLAG == 1
                    ESP_LOGD(TAG, "Retring to connect to the AP...");
                #endif
                
            }
            else
            {   
                /* Set bits (flags) within an RTOS event group */
                xEventGroupSetBits(WIFI_t_event_group, WIFI_FAIL_BIT);
                 /* The Wi-fi has disconnected or was unable to connect, flag for connection becomes 0 */
                WIFI_i_connected_flag = 0;

                ESP_LOGI(TAG, "Connect to the AP failed.");
                
                #if WIFI_DEBUG_FLAG == 1
                    ESP_LOGD(TAG, "Switching Wifi in softAP mode.");
                #endif

                /* Destroy the esp_netif object for the Wi-fi interface in AP mode before creating a new one.*/
                esp_netif_destroy(WIFI_t_ap);

                /* Create a new esp_netif object and configure it in AP operating mode. */
                WIFI_v_ConfigSoftAP();
            }
            break;
        }
        case WIFI_EVENT_GOT_IP:
        {
            /* In case Wi-fi station got IP from connected AP the connection is established 
            and the WIFI_CONNECTED_BIT is set */

            /* Store the WiFi credentials in the non-volatile memory. */
            nvm_err_t t_err = NVM_t_setWifiCredentials(WIFI_aub_ssid, WIFI_aub_pass);

            /*  Check if write operation in NVS was successful. */
            if (t_err == NVM_i_OK)
            {
                #if WIFI_DEBUG_FLAG == 1
                    ESP_LOGD(TAG, "Write ssid: %s and password: %s in NVS", WIFI_aub_ssid, WIFI_aub_pass);
                #endif

            }
            else
            {
                #if WIFI_DEBUG_FLAG == 1
                    ESP_LOGD(TAG, "Did not write in NVS");
                #endif
            }

            
            #if WIFI_DEBUG_FLAG == 1
                /* Event structure for IP EVENT*/
                ip_event_got_ip_t *t_event = (ip_event_got_ip_t *)v_event_data;

                ESP_LOGD(TAG, "Got IP from DHCP server:" IPSTR, IP2STR(&t_event->ip_info.ip));
            #endif
            
            /* Reinitialize number of retries with 0, in case a disconnect event will occur*/
            WIFI_i_retry_num = 0;

            /* Set bits (flags) within an RTOS event group */
            xEventGroupSetBits(WIFI_t_event_group, WIFI_CONNECTED_BIT);

            /*Wi-fi connected successfully, flag is set to 1*/
            WIFI_i_connected_flag = 1;

            break;
        }
    }
}

/**
 * @brief Function for initializing and configuring the Wii-fi interface
 * in station operating mode.
 * 
 * @param i_credentials_from Parameter for selecting where the credentials for connecting to a network come from.
 */
void WIFI_v_ConifgStation(int8_t i_credentials_from)
{
  
    /* Wi-fi default initialization in station mode */
    WIFI_t_station = esp_netif_create_default_wifi_sta();

    /* Conifguration is set to default values */
    wifi_init_config_t t_cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&t_cfg);

    /* Configure the Wi-fi*/
    wifi_config_t t_wifi_config =
    {
        .sta = 
        {
            /* Setting a password implies station will connect to all security modes including WEP/WPA.
             * However these modes are deprecated and not advisable to be used. Incase your Access point
             * doesn't support WPA2, these mode can be enabled by commenting below line */
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,

            .pmf_cfg = 
            {
                .capable = true,
                .required = false
            },
        }
    };

    /* Check if the the credentials are from webserver or nvs */
    if (i_credentials_from == WIFI_CONNECT_FROM_AP)
    {   
        /* Set the ssid and password recieved from the webserver into wifi configuration. */
        memcpy(t_wifi_config.sta.ssid, WIFI_aub_ssid, sizeof(t_wifi_config.sta.ssid));
        memcpy(t_wifi_config.sta.password, WIFI_aub_pass, sizeof(t_wifi_config.sta.password));
    }
    else
    {
        /* Get Wi-fi credentials saved in NVS. */
        nvm_err_t t_err = NVM_t_getWifiCredentials(WIFI_aub_ssid, WIFI_aub_pass);

        /* Check if read operation from NVS was executed successfully.*/
        if (t_err == NVM_i_OK)
        {
            #if WIFI_DEBUG_FLAG == 1
                ESP_LOGD(TAG, "Found SSID: %s and password: %s", WIFI_aub_ssid, WIFI_aub_pass);
            #endif
            

            /* Set the ssid and password from the NVS into Wi-fi configuration. */
            memcpy(t_wifi_config.sta.ssid, WIFI_aub_ssid, sizeof(t_wifi_config.sta.ssid));
            memcpy(t_wifi_config.sta.password, WIFI_aub_pass, sizeof(t_wifi_config.sta.password));
        }
        else
        {   
            #if WIFI_DEBUG_FLAG == 1
                ESP_LOGD(TAG, "No SSID and password found Iin NVS");
            #endif
            
        }
    }
 
    /* Wi-fi operating mode is set as station */
    esp_wifi_set_mode(WIFI_MODE_STA);

    /* Configuration of the ESP32 STA is set */
    esp_wifi_set_config(WIFI_IF_STA, &t_wifi_config);

    /* Start Wi-fi with the current configuration */
    esp_wifi_start();

    #if WIFI_DEBUG_FLAG == 1
        ESP_LOGD(TAG, "Initialization and configuration for Wi-fi in station mode finished.");
    #endif
    
}

/**
 * @brief Function for initializing and configuring the Wi-fi interface
 * in AP operating mode.
 * 
 */
void WIFI_v_ConfigSoftAP(void)
{
    /* Wi-fi default initialization in AP mode */
    WIFI_t_ap = esp_netif_create_default_wifi_ap();

    /* Conifguration is set to default values */
    wifi_init_config_t t_cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&t_cfg);

    /* Configuration for the Wi-fi in AP mode*/
    wifi_config_t t_wifi_config =
    {
        .ap = 
        {
            .ssid = WIFI_AP_SSID,
            .ssid_len = strlen(WIFI_AP_SSID),
            .password = WIFI_AP_PASS,
            .max_connection = WIFI_AP_MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK
        },
    };

    /* Wi-fi operating mode is set as AP */
    esp_wifi_set_mode(WIFI_MODE_AP);

    /* Configuration of the ESP32 STA is set */
    esp_wifi_set_config(WIFI_IF_AP, &t_wifi_config);

    /* Start Wi-fi with the current configuration */
    esp_wifi_start();

    #if WIFI_DEBUG_FLAG == 1
       ESP_LOGD(TAG, "Wifi configuration in softAp mode is finished."); 
    #endif

    ESP_LOGI(TAG, "SoftAP configured with SSID: %s password: %s", WIFI_AP_SSID, WIFI_AP_PASS);
    
}

/* Exported functions */
/*============================================================================*/

/**
 * @brief The function initializes the netif intreface, creates an esp event loop
 * and registers event handlers on esp Wi-fi and IP events. The Wi-fi will be at first configured
 * in station operating mode with credentials saved in NVS.
 * 
 */
void WIFI_v_Init(void)
{
    /* Initialize the underlying TCP/IP stack */
    esp_netif_init();

    /* Create a new esp loop event */
    esp_event_loop_create_default();

    /* Create the event group. */
    WIFI_t_event_group = xEventGroupCreate();

    /* Initialoize the connection flag with 0.*/
    WIFI_i_connected_flag = 0;
    
    /* Initialize the current rety number with 0. */
    WIFI_i_retry_num = 0;

    /* Register an instance of event handler to the default loop for Wi-fi events*/
    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &WIFI_v_event_handler, NULL, NULL);

    /* Register an instance of event handler to the default loop for IP events*/
    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &WIFI_v_event_handler, NULL, NULL);

     NVM_t_setWifiCredentials("practica2021","1234567890");
    //NVM_t_setWifiCredentials("Maria","76163026dff0");
    
    /* Configure and start Wi-fi in station mode*/
    WIFI_v_ConifgStation(WIFI_CONNECT_NVS_CREDENTIALS);

    /* Start webserver */
    WIFI_v_start_webserver();

    /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
     * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler()  */
    EventBits_t t_bits = xEventGroupWaitBits(WIFI_t_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT, pdFALSE, pdFALSE, portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually happened. */
    if (t_bits & WIFI_CONNECTED_BIT)
    {
        ESP_LOGI(TAG, "Connected to AP with SSID: %s", WIFI_aub_ssid);

    }
    else if (t_bits & WIFI_FAIL_BIT)
    {
        ESP_LOGI(TAG, "Failed to connect to SSID: %s",WIFI_aub_ssid);
    }
    else
    {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }

}

/**
 * @brief Returns the Wi-fi flag for connection. 
 * If connection to Wi-fi is provided, flag is 1, otherwise flag is 0.
 * 
 * @return int8_t 
 */
int8_t WIFI_i_IsConnected(void)
{
    return WIFI_i_connected_flag;
}