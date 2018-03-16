/*
********************************************************************************
*  Copyright (C), 2011-2016, ROSIM Co.,Ltd.
*
*  FileName: htos.c
*
*  Author: wabbow       Version : v1.0         Date: 2015-10-29
*
*  Description:     
*
*  Version:         
*
*  Function List:   
*    1. -------
*
*  History:         
*    <author>            <time>       <version >       <desc>
*    wabbow             2015/11/05       v1.0          modify
********************************************************************************
*/

/*******************************************************************************
*                            INCLUDES
*/
#include "htos.h"
#include "htos_pwr_mgr.h"
#include "htos_timer.h"
#include "htos_app.h"
#include "syscfg.h"



/*******************************************************************************
* Function: htos_set_event()
* Input: OS_TASK_ID task_id --- receiving tasks ID
*        OS_EVENT_FLAG event_flag --- what event to set
*
* Output: SUCCESS, INVALID_TASK
*
* Description: This function is called to set the event flags for a task. The
*              event passed in is OR'd into the task's event variable.
*
* specification: null
*******************************************************************************/
OS_INT08U  htos_set_event (OS_TASK_ID task_id,
                           OS_EVENT_FLAG event_flag)
{
#if CRITICAL_MODE == 1
    OS_CPU     x = 0;
#endif
    
    
    if (task_id < osTasksCnt) {
        OS_DISABLE_INTERRUPTS(x);
        osTasksEvents[task_id] |= event_flag;      // Stuff the event bit(s)
        OS_ENABLE_INTERRUPTS(x);
        return (HTOS_SUCCESS);
    }
    else{
        return (HTOS_INVALID_TASK);
    }
    
}

/*******************************************************************************
* Function: htos_clear_event()
* Input: OS_TASK_ID task_id --- receiving tasks ID
*        OS_EVENT_FLAG event_flag --- what event to set
*
* Output: SUCCESS, INVALID_TASK
*
* Description:  This function is called to clear the event flags for a task. The
*               event passed in is masked out of the task's event variable.
*
* specification: null
*******************************************************************************/
OS_INT08U  htos_clear_event ( OS_TASK_ID task_id,
                              OS_EVENT_FLAG event_flag )
{
#if CRITICAL_MODE == 1
    OS_CPU     x = 0;
#endif
    
    
    if (task_id < osTasksCnt) {
        OS_DISABLE_INTERRUPTS(x);
        osTasksEvents[task_id] &= ~(event_flag);   // Clear the event bit(s)
        OS_ENABLE_INTERRUPTS(x);
        return (HTOS_SUCCESS);
    }
    else{
        return (HTOS_INVALID_TASK);
    }
    
}

/*******************************************************************************
* Function: htos_get_event()
* Input: OS_TASK_ID task_id --- receiving tasks ID
*        OS_EVENT_FLAG event_flag --- what event to set
*
* Output: SUCCESS, INVALID_TASK
*
* Description:  This function is called to get the event flags for a task. The
*               event passed in is masked out of the task's event variable.
*
* specification: null
*******************************************************************************/
OS_EVENT_FLAG  htos_get_event (OS_TASK_ID task_id)
{
#if CRITICAL_MODE == 1
    OS_CPU      x = 0;
#endif
    OS_EVENT_FLAG   event;

    
    
    event = 0;
    if ( task_id < osTasksCnt) {
        OS_DISABLE_INTERRUPTS(x);
        event = osTasksEvents[task_id] & (~osTaskEventsMasks[task_id]);  // Stuff the event bit(s)
        OS_ENABLE_INTERRUPTS(x);
    }
    
    return event;
}

/*******************************************************************************
* Function: htos_mask_event()
* Input: OS_TASK_ID task_id --- receiving tasks ID
*        OS_EVENT_FLAG event_flag --- what event to mask
*
* Output: SUCCESS, INVALID_TASK
*
* Description:  This function is called to get the event flags for a task. The
*               event passed in is masked out of the task's event variable.
*
* specification: null
*******************************************************************************/
OS_INT08U  htos_mask_event (OS_TASK_ID task_id,
                            OS_EVENT_FLAG event_flag)
{
#if CRITICAL_MODE == 1
    OS_CPU      x = 0;
#endif
    
    
    if(task_id < osTasksCnt){ 
        OS_DISABLE_INTERRUPTS(x);
        osTaskEventsMasks[task_id] |=  event_flag;
        OS_ENABLE_INTERRUPTS(x);
        return (HTOS_SUCCESS);
    }
    else{
        return (HTOS_INVALID_TASK);
    }
    
}

/*******************************************************************************
* Function: htos_unmask_event()
* Input: OS_TASK_ID task_id --- receiving tasks ID
*        OS_EVENT_FLAG event_flag --- what event to unmask
*
* Output: SUCCESS, INVALID_TASK
*
* Description:  This function is called to get the event flags for a task. The
*               event passed in is masked out of the task's event variable.
*
* specification: null
*******************************************************************************/
OS_INT08U  htos_unmask_event (OS_TASK_ID task_id,
                              OS_EVENT_FLAG event_flag)
{
#if CRITICAL_MODE == 1
    OS_CPU      x = 0;
#endif
    
    
    if (task_id < osTasksCnt) {
        OS_DISABLE_INTERRUPTS(x);
        osTaskEventsMasks[task_id] &=  ~event_flag;
        OS_ENABLE_INTERRUPTS(x);
        return (HTOS_SUCCESS);
    }
    else{
        return (HTOS_INVALID_TASK);
    }
    
}

/*******************************************************************************
* Function: htos_run_system()
*
* Input: none
*
* Output: none
*
* Description:  This function will make one pass through the htos taskEvents table
*   and call the task_event_processor() function for the first task that
*   is found with at least one event pending. If there are no pending
*   events (all tasks), this function puts the processor into Sleep.
*
* specification: null
*******************************************************************************/
OS_VOID  htos_run_system (OS_VOID)
{
    OS_TASK_ID          idx = 0;
    static OS_TASK_ID   taskId = 0;
    
    
    htos_timerEvent_update();
    
    do{
        if (osTasksEvents[taskId] & (~osTaskEventsMasks[taskId])) {
            break;
        }
        taskId++;
        if (taskId == osTasksCnt) {
            taskId = 0;
        }
    }while (++idx < osTasksCnt);
    
    if (idx < osTasksCnt) {
        (osTaskFuc[taskId])();
        taskId++;
        if (taskId == osTasksCnt) {
            taskId = 0;
        }
    }
    else{     
#if POWER_SAVING_EN > 0
        htos_pwrmgr_powerconserve();
#endif
    }
    
}
