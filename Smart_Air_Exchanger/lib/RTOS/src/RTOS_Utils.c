/*============================================================================*/
/*                                 ALTRAN                                     */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*!
 * $Source: RTOS_Utils.c $
 * $Revision: 1 $
 * $Author: George Ciuclea $
 * $Date: 2021.08.05 $
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** \file
    Implements RTOS task scheduling and timer initialization and operation
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
 * $Log: RTOS_Utils.c  $
  ============================================================================*/

/* Includes */
/*============================================================================*/
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>
#include <freertos/event_groups.h>
#include <esp_log.h>
#include <esp_system.h>
#include "RTOS_API.h"
#include "RTOS_Utils.h"
#include "RTOS_Performance.h"
#include "RTOS_Interface.h"
#include "RTOS_Config.h"

/* Constants and types  */
/*============================================================================*/
/**
 * @brief Bit definition of the reboot flag
 * 
 */
#define REBOOT_FLAG (1 << 0)

/**
 * @brief Tags used for debug logging the timers
 * 
 */
#if RTOS_DEBUG == 1
static const char *TAG5 = "Timer5";
static const char *TAG50 = "Timer50";
static const char *TAG500 = "Timer500";
static const char *TAG1000 = "Timer1000";
static const char *TAG1MIN = "Timer1min";
#endif

#define NUMBER_OF_TIMERS 5

/**
 * @brief Structure for storing all timer handles
 * 
 */
typedef struct
{
    TimerHandle_t tHandles[NUMBER_OF_TIMERS];
} TIMER_ARGUMENTS;

/* Variables */
/*============================================================================*/
/**
 * @brief Handle of the task that will initialize all other runtime tasks
 * 
 */
TaskHandle_t initTaskHandle;

/**
 * @brief Handle of the maximum priority task that will await suspended for the reboot flag
 * 
 */
TaskHandle_t rebootTaskHandle;

/**
 * @brief Handle of the network task tree on core 1
 * 
 */
TaskHandle_t networkTaskHandle;

/**
 * @brief Handle of the MQTT task tree on core 0
 * 
 */
TaskHandle_t mqttTaskHandle;

/**
 * @brief Structure for the handles of the 3 different timers
 * 
 */
TIMER_ARGUMENTS tArg;

/**
 * @brief Handle for the shutdown event group 
 * 
 */
EventGroupHandle_t shutdownEventGroup;

/* Private functions prototypes */
/*============================================================================*/

/**
 * @brief Function used as a task callback. Will initialize runtime tasks.
 * 
 * @param args Will receive the structure that holds the timer handles, cast to void*
 */
void rtosRuntimeInitializer(void *args);

/**
 * @brief Function used as a task callback. Will initialize network modules
 * 
 * @param args Do not use, placeholder to 0
 */
void rtosNetworkInitializer(void *args);

/**
 * @brief Function used as a task callback. Task has highest priority and waits suspended for reboot flag to be set
 * 
 * @param args RTOS requirement. Receives placeholder (void*)0
 */
void rtosRebootEventListener(void *args);

/**
 * @brief Call assigned functions every 5ms
 * 
 * @param xTimer RTOS requirement. Its timer handle
 */
void rtos1000msTaskWrapperC1(TimerHandle_t xTimer);

/**
 * @brief Call assigned functions every 50ms
 * 
 * @param xTimer RTOS requirement. Its timer handle
 */
void rtos50msTaskWrapper(TimerHandle_t xTimer);

/**
 * @brief Call assigned functions every 500ms
 * 
 * @param xTimer RTOS requirement. Its timer handle
 */
void rtos500msTaskWrapper(TimerHandle_t xTimer);

/**
 * @brief Call assigned functions every 1000ms
 * 
 * @param xTimer RTOS requirement. Its timer handle
 */
void rtos1000msTaskWrapper(TimerHandle_t xTimer);

/**
 * @brief Call assigned functions every 1min
 * 
 * @param xTimer RTOS requirement. Its timer handle
 */
void rtos1minTaskWrapper(TimerHandle_t xTimer);

/**
 * @brief Internal call for the startup routine
 * 
 */
void rtosStartupRoutine();

/**
 * @brief Internal call for the shutdown routine
 * 
 */
void rtosShutdownRoutine();

/* Inline functions */
/*============================================================================*/

/* Private functions */
/*============================================================================*/
void rtos1000msTaskWrapperC1(void *args)
{
    for (;;)
    {
        //Measures starting RTOS tick count, 1 tick = 1ms
        TickType_t startTickCount = xTaskGetTickCount();

        //Calls defined functions
        rtos1000msTaskC1();

        //Measures ending RTOS tick count, 1 tick = 1ms
        TickType_t endTickCount = xTaskGetTickCount();

        registerTicks(CYCLE_1000MSC1, endTickCount - startTickCount);

//Logs (debugging) how long the called functions took to run (in ms)
#if RTOS_DEBUG == 1
        ESP_LOGD(TAG5, "Task ran for %dms\n", endTickCount - startTickCount);
#endif
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

void rtos50msTaskWrapper(TimerHandle_t xTimer)
{
    //Measures starting RTOS tick count, 1 tick = 1ms
    TickType_t startTickCount = xTaskGetTickCount();

    //Calls defined functions
    rtos50msTask();

    //Measures ending RTOS tick count, 1 tick = 1ms
    TickType_t endTickCount = xTaskGetTickCount();

    registerTicks(CYCLE_50MS, endTickCount - startTickCount);

//Logs (debugging) how long the called functions took to run (in ms)
#if RTOS_DEBUG == 1
    ESP_LOGD(TAG50, "Task ran for %dms\n", endTickCount - startTickCount);
#endif
}

void rtos500msTaskWrapper(TimerHandle_t xTimer)
{
    //Measures starting RTOS tick count, 1 tick = 1ms
    TickType_t startTickCount = xTaskGetTickCount();

    //Calls defined functions
    rtos500msTask();

    //Measures ending RTOS tick count, 1 tick = 1ms
    TickType_t endTickCount = xTaskGetTickCount();

    registerTicks(CYCLE_500MS, endTickCount - startTickCount);

//Logs (debugging) how long the called functions took to run (in ms)
#if RTOS_DEBUG == 1
    ESP_LOGD(TAG500, "Task ran for %dms\n", endTickCount - startTickCount);
#endif
}

void rtos1000msTaskWrapper(TimerHandle_t xTimer)
{
    //Measures starting RTOS tick count, 1 tick = 1ms
    TickType_t startTickCount = xTaskGetTickCount();

    //Calls defined functions
    rtos1000msTask();

    //Measures ending RTOS tick count, 1 tick = 1ms
    TickType_t endTickCount = xTaskGetTickCount();

    registerTicks(CYCLE_1000MS, endTickCount - startTickCount);

//Logs (debugging) how long the called functions took to run (in ms)
#if RTOS_DEBUG == 1
    ESP_LOGD(TAG1000, "Task ran for %dms\n", endTickCount - startTickCount);
#endif
}

void rtos1minTaskWrapper(TimerHandle_t xTimer)
{
    //Measures starting RTOS tick count, 1 tick = 1ms
    TickType_t startTickCount = xTaskGetTickCount();

    //Calls defined functions
    rtos1minTask();

    //Measures ending RTOS tick count, 1 tick = 1ms
    TickType_t endTickCount = xTaskGetTickCount();

    registerTicks(CYCLE_1MIN, endTickCount - startTickCount);

//Logs (debugging) how long the called functions took to run (in ms)
#if RTOS_DEBUG == 1
    ESP_LOGD(TAG1MIN, "Task ran for %dms\n", endTickCount - startTickCount);
#endif
}

void rtosNetworkInitializer(void *args)
{
    rtosNetworkInitTask();

    vTaskDelete(NULL);
}

void rtosMQTTInitializer(void *args)
{
    rtosMQTTInitTask();

    vTaskDelete(NULL);
}

void rtosRuntimeInitializer(void *args)
{
    //Saves the timer handle structure in a local variable for memory access efficiency
    TIMER_ARGUMENTS timerHandleStructure = *((TIMER_ARGUMENTS *)args);

    TaskHandle_t tHandleC1;

    //Creates (stopped) the timers for the 3 different required intervals
    //The callback functions will have a priority of 1
    xTaskCreatePinnedToCore(rtos1000msTaskWrapperC1, "Delay_1000ms", 4096, (void *)0, 1, &tHandleC1, 1);
    timerHandleStructure.tHandles[1] = xTimerCreate("Timer_50ms", 50 / portTICK_PERIOD_MS, pdTRUE, (void *)0, rtos50msTaskWrapper);
    timerHandleStructure.tHandles[2] = xTimerCreate("Timer_500ms", 500 / portTICK_PERIOD_MS, pdTRUE, (void *)0, rtos500msTaskWrapper);
    timerHandleStructure.tHandles[3] = xTimerCreate("Timer_1000ms", 1000 / portTICK_PERIOD_MS, pdTRUE, (void *)0, rtos1000msTaskWrapper);
    timerHandleStructure.tHandles[4] = xTimerCreate("Timer_1min", 60000 / portTICK_PERIOD_MS, pdTRUE, (void *)0, rtos1minTaskWrapper);

    //Starts the three timers, from slowest to fastest
    xTimerStart(timerHandleStructure.tHandles[4], 50);
    xTimerStart(timerHandleStructure.tHandles[3], 50);
    xTimerStart(timerHandleStructure.tHandles[2], 50);
    xTimerStart(timerHandleStructure.tHandles[1], 50);

    //Ensures this function does not return (because it is a task), by deleting the task
    vTaskDelete(NULL);
}

void rtosStartupRoutine()
{

    //Creates the event group for shutdown/reboot
    shutdownEventGroup = xEventGroupCreate();

    //Creates and starts the reboot event handler task with maximum priority
    xTaskCreatePinnedToCore(rtosRebootEventListener, "RTOS Reboot", 4096, (void *)0, configMAX_PRIORITIES, &rebootTaskHandle, 0);

    TickType_t startTickCount = xTaskGetTickCount();

    //Calls defined startup functions
    rtosInitTask();

    TickType_t endTickCount = xTaskGetTickCount();

    registerTimeStartup(endTickCount - startTickCount);

    xTaskCreatePinnedToCore(rtosNetworkInitializer, "RTOS Network", 4096, (void *)0, 1, &networkTaskHandle, 1);

    vTaskDelay(5000);

    xTaskCreatePinnedToCore(rtosMQTTInitializer, "RTOS MQTT", 4096, (void *)0, 1, &mqttTaskHandle, 0);

    vTaskDelay(2000);

#if RTOS_DEBUG == 1
    ESP_LOGD("Startup task", "Task ran for %dms\n", endTickCount - startTickCount);
#endif

    //Creates and starts the task responsible for runtime initialization
    xTaskCreatePinnedToCore(rtosRuntimeInitializer, "RTOS Runtime", 4096, (void *)&tArg, 1, &initTaskHandle, 0);
}

void rtosShutdownRoutine()
{
    //Sets the reboot flag in order to trigger the reboot event handler task to resume
    xEventGroupSetBits(shutdownEventGroup, REBOOT_FLAG);
}

void rtosRebootEventListener(void *args)
{
    //Is in a loop because the flag wait functionality of RTOS has a timeout
    for (;;)
    {
        EventBits_t uxBits;

        //Waits for specified bits to be set and reads the current status of the shutdown flag register
        uxBits = xEventGroupWaitBits(shutdownEventGroup, REBOOT_FLAG, pdFALSE, pdTRUE, portMAX_DELAY);

        //Checks wether the wait completed because of the trigger or because of the timeout
        if ((uxBits & REBOOT_FLAG) != 0)
        {
            //Because of the trigger

#if RTOS_DEBUG == 1
            TickType_t startTickCount = xTaskGetTickCount();
#endif

            //Calls defined funtions
            rtosShutdownTask();

#if RTOS_DEBUG == 1
            TickType_t endTickCount = xTaskGetTickCount();
#endif

#if RTOS_DEBUG == 1
            ESP_LOGD("Shutdown task", "Task ran for %dms\n", endTickCount - startTickCount);
#endif

            //Clears flag - maybe not needed (in case the restart fails)
            xEventGroupClearBits(shutdownEventGroup, REBOOT_FLAG);

            //Hard-restarts the device, reboots from bootloader stage
            esp_restart();
        }
        //Because of the timeout - loops back again
    }
}
/* Exported functions */
/*============================================================================*/
void rtosStart()
{
    rtosStartupRoutine();
}

void rtosReboot()
{
    rtosShutdownRoutine();
}
