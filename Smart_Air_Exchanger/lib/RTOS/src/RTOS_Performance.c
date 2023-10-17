/*============================================================================*/
/*                                 ALTRAN                                     */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/*!
 * $Source: RTOS_Performance.c $
 * $Revision: 1 $
 * $Author: George Ciuclea $
 * $Date: 2021.08.10 $
 */
/*============================================================================*/
/* DESCRIPTION :                                                              */
/** @file
    
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
 * $Log: RTOS_Performance.c  $
  ============================================================================*/

/* Includes */
/*============================================================================*/
#include "RTOS_Performance.h"
#include "RTOS_Interface.h"
#include <stdio.h>

/* Constants and types  */
/*============================================================================*/
/**
 * @brief Structure for the cyclic list (implemented in array due to coding constraints)
 * 
 */
typedef struct {
    /**
     * @brief The stored integer values, -1 for NIL
     * 
     */
    int values[MAX_TICK_VALUES_AVERAGE];
    /**
     * @brief Index pointer to next value to be written in the vector
     * 
     */
    int nextPosition;
}PERF_VALUE_STORAGE;

/* Variables */
/*============================================================================*/
volatile PERF_VALUE_STORAGE perfValues[TIME_CATEGORIES]=
{
    {
        .values={-1},
        .nextPosition=0
    }
};
volatile unsigned int startupTimeTicks=0;

/* Private functions prototypes */
/*============================================================================*/
unsigned int getMin(int timeCategory);

unsigned int getMax(int timeCategory);

unsigned int getAverage(int timeCategory);

void registerTime(int timeCategory, int numTicks);


/* Inline functions */
/*============================================================================*/


/* Private functions */
/*============================================================================*/
unsigned int getMin(int timeCategory)
{
    volatile int *values=perfValues[timeCategory].values;
    unsigned int min=values[0];
    for(int i=1;i<MAX_TICK_VALUES_AVERAGE;i++)
    {
        if(min==-1 || values[i]<min)
        {
            min=values[i];
        }
        else
        {
            //Empty
        }
    }
    return min;
}

unsigned int getMax(int timeCategory)
{
    volatile int *values=perfValues[timeCategory].values;
    unsigned int max=values[0];
    for(int i=1;i<MAX_TICK_VALUES_AVERAGE;i++)
    {
        if(max==-1 || values[i]>max)
        {
            max=values[i];
        }
        else
        {
            //Empty
        }
    }
    return max;
}

unsigned int getAverage(int timeCategory)
{
    volatile int *values=perfValues[timeCategory].values;
    unsigned int sum=0;
    for(int i=0;i<MAX_TICK_VALUES_AVERAGE;i++)
    {
        sum+=values[i]==-1?0:values[i];
    }
    return (1.0f*sum)/MAX_TICK_VALUES_AVERAGE;
}

void registerTime(int timeCategory, int numTicks)
{
    perfValues[timeCategory].values[perfValues[timeCategory].nextPosition]=numTicks;
    perfValues[timeCategory].nextPosition++;
    if(perfValues[timeCategory].nextPosition==MAX_TICK_VALUES_AVERAGE)
    {
        perfValues[timeCategory].nextPosition=0;
    }
}

/* Exported functions */
/*============================================================================*/
unsigned int getAverageTicks(TASK_TYPE_LIST typeOfTask)
{
    return getAverage(typeOfTask);
}
unsigned int getMinTicks(TASK_TYPE_LIST typeOfTask)
{
    return getMin(typeOfTask);
}
unsigned int getMaxTicks(TASK_TYPE_LIST typeOfTask)
{
    return getMax(typeOfTask);
}
unsigned int getTimeStartup()
{
    return startupTimeTicks;
}
void registerTicks(TASK_TYPE_LIST typeOfTask, int numTicks)
{
    registerTime(typeOfTask,numTicks);
}
void registerTimeStartup(int numTicks)
{
    startupTimeTicks=numTicks;
}

void getPerformanceStats(char *stats)
{
    
    sprintf(stats,
    "Startup time: %d ms\
    Duration of 5ms task: min:%d avg:%d max:%d\
    Duration of 50ms task: min:%d avg:%d max:%d\
    Duration of 500ms task: min:%d avg:%d max:%d\
    Duration of 1000ms task: min:%d avg:%d max:%d\
    Duration of 1min task: min:%d avg:%d max:%d\
    "
    ,
    getTimeStartup(),
    getMin(CYCLE_1000MSC1),(unsigned int)getAverage(CYCLE_1000MSC1),getMax(CYCLE_1000MSC1),
    getMin(CYCLE_50MS),(unsigned int)getAverage(CYCLE_50MS),getMax(CYCLE_50MS),
    getMin(CYCLE_500MS),(unsigned int)getAverage(CYCLE_500MS),getMax(CYCLE_500MS),
    getMin(CYCLE_1000MS),(unsigned int)getAverage(CYCLE_1000MS),getMax(CYCLE_1000MS),
    getMin(CYCLE_1MIN),(unsigned int)getAverage(CYCLE_1MIN),getMax(CYCLE_1MIN)
    );

}

unsigned int getPerformanceStatsSelective(TASK_TYPE_LIST selectedTask, STAT_METRIC_TYPE metricType)
{
    switch (metricType)
    {
    case MIN_VALUE:
        return getMin(selectedTask);
        break;
    case AVG_VALUE:
        return (unsigned int)getAverage(selectedTask);
        break;
    case MAX_VALUE:
        return getMax(selectedTask);
        break;
    }
    return 0;
}