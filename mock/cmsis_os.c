#include <string.h>
#include "cmsis_os.h"

/*********************** Thread Management *****************************/
/**
* @brief  Create a thread and add it to Active Threads and set it to state READY.
* @param  thread_def    thread definition referenced with \ref osThread.
* @param  argument      pointer that is passed to the thread function as start argument.
* @retval thread ID for reference by other functions or NULL in case of error.
* @note   MUST REMAIN UNCHANGED: \b osThreadCreate shall be consistent in every CMSIS-RTOS.
*/
osThreadId osThreadCreate (const osThreadDef_t *thread_def, void *argument)
{
//   TaskHandle_t handle;
//  
//#if( configSUPPORT_STATIC_ALLOCATION == 1 ) &&  ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
//  if((thread_def->buffer != NULL) && (thread_def->controlblock != NULL)) {
//    handle = xTaskCreateStatic((TaskFunction_t)thread_def->pthread,(const portCHAR *)thread_def->name,
//              thread_def->stacksize, argument, makeFreeRtosPriority(thread_def->tpriority),
//              thread_def->buffer, thread_def->controlblock);
//  }
//  else {
//    if (xTaskCreate((TaskFunction_t)thread_def->pthread,(const portCHAR *)thread_def->name,
//              thread_def->stacksize, argument, makeFreeRtosPriority(thread_def->tpriority),
//              &handle) != pdPASS)  {
//      return NULL;
//    } 
//  }
//#elif( configSUPPORT_STATIC_ALLOCATION == 1 )
//
//    handle = xTaskCreateStatic((TaskFunction_t)thread_def->pthread,(const portCHAR *)thread_def->name,
//              thread_def->stacksize, argument, makeFreeRtosPriority(thread_def->tpriority),
//              thread_def->buffer, thread_def->controlblock);
//#else
//  if (xTaskCreate((TaskFunction_t)thread_def->pthread,(const portCHAR *)thread_def->name,
//                   thread_def->stacksize, argument, makeFreeRtosPriority(thread_def->tpriority),
//                   &handle) != pdPASS)  {
//    return NULL;
//  }     
//#endif
//  
  return NULL;
}

osThreadId osThreadGetId (void)
{
  return NULL;
}

osStatus osThreadTerminate (osThreadId thread_id)
{
  return osOK;
}

osStatus osThreadYield (void)
{
  return osOK;
}

osStatus osDelay (uint32_t millisec)
{
  return osOK;
}

/***********************  Timer Management Functions ***************************/
/**
* @brief  Create a timer.
* @param  timer_def     timer object referenced with \ref osTimer.
* @param  type          osTimerOnce for one-shot or osTimerPeriodic for periodic behavior.
* @param  argument      argument to the timer call back function.
* @retval  timer ID for reference by other functions or NULL in case of error.
* @note   MUST REMAIN UNCHANGED: \b osTimerCreate shall be consistent in every CMSIS-RTOS.
*/
osTimerId osTimerCreate (const osTimerDef_t *timer_def, os_timer_type type, void *argument)
{
#if (configUSE_TIMERS == 1)
	return 1;

  	//return xTimerCreate((const char *)"",
    //                 	1, // period should be filled when starting the Timer using osTimerStart
    //                 	(type == osTimerPeriodic) ? pdTRUE : pdFALSE,
    //                 	(void *) argument,
    //                 	(TaskFunction_t)timer_def->ptimer);
#else 
	return NULL;
#endif
}

/**
* @brief  Start or restart a timer.
* @param  timer_id      timer ID obtained by \ref osTimerCreate.
* @param  millisec      time delay value of the timer.
* @retval  status code that indicates the execution status of the function
* @note   MUST REMAIN UNCHANGED: \b osTimerStart shall be consistent in every CMSIS-RTOS.
*/
osStatus osTimerStart (osTimerId timer_id, uint32_t millisec)
{
  osStatus result = osOK;
  return result;
}

/**
* @brief  Stop a timer.
* @param  timer_id      timer ID obtained by \ref osTimerCreate
* @retval  status code that indicates the execution status of the function.
* @note   MUST REMAIN UNCHANGED: \b osTimerStop shall be consistent in every CMSIS-RTOS.
*/
osStatus osTimerStop (osTimerId timer_id)
{
  osStatus result = osOK;
  return result;
}

/**
* @brief  Delete a timer.
* @param  timer_id      timer ID obtained by \ref osTimerCreate
* @retval  status code that indicates the execution status of the function.
* @note   MUST REMAIN UNCHANGED: \b osTimerDelete shall be consistent in every CMSIS-RTOS.
*/
osStatus osTimerDelete (osTimerId timer_id)
{
osStatus result = osOK;
  return result;
}


/***************************  Signal Management ********************************/
/**
* @brief  Set the specified Signal Flags of an active thread.
* @param  thread_id     thread ID obtained by \ref osThreadCreate or \ref osThreadGetId.
* @param  signals       specifies the signal flags of the thread that should be set.
* @retval previous signal flags of the specified thread or 0x80000000 in case of incorrect parameters.
* @note   MUST REMAIN UNCHANGED: \b osSignalSet shall be consistent in every CMSIS-RTOS.
*/
int32_t osSignalSet (osThreadId thread_id, int32_t signal)
{
//#if( configUSE_TASK_NOTIFICATIONS == 1 )  
//  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
//  uint32_t ulPreviousNotificationValue = 0;
//  
//  if (inHandlerMode())
//  {
//    if(xTaskGenericNotifyFromISR( thread_id , (uint32_t)signal, eSetBits, &ulPreviousNotificationValue, &xHigherPriorityTaskWoken ) != pdPASS )
//      return 0x80000000;
//    
//    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
//  }  
//  else if(xTaskGenericNotify( thread_id , (uint32_t)signal, eSetBits, &ulPreviousNotificationValue) != pdPASS )
//    return 0x80000000;
//  
//  return ulPreviousNotificationValue;
//#else
//  (void) thread_id;
//  (void) signal;
//
//  return 0x80000000; /* Task Notification not supported */  
//#endif
  return 0;   //TODO: Better mock
}

/**
* @brief  Clear the specified Signal Flags of an active thread.
* @param  thread_id  thread ID obtained by \ref osThreadCreate or \ref osThreadGetId.
* @param  signals    specifies the signal flags of the thread that shall be cleared.
* @retval  previous signal flags of the specified thread or 0x80000000 in case of incorrect parameters.
* @note   MUST REMAIN UNCHANGED: \b osSignalClear shall be consistent in every CMSIS-RTOS.
*/
int32_t osSignalClear (osThreadId thread_id, int32_t signal);

/**
* @brief  Wait for one or more Signal Flags to become signaled for the current \b RUNNING thread.
* @param  signals   wait until all specified signal flags set or 0 for any single signal flag.
* @param  millisec  timeout value or 0 in case of no time-out.
* @retval  event flag information or error code.
* @note   MUST REMAIN UNCHANGED: \b osSignalWait shall be consistent in every CMSIS-RTOS.
*/
osEvent osSignalWait (int32_t signals, uint32_t millisec)
{
  osEvent ret;
  return ret;
}