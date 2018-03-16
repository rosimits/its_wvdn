/*
********************************************************************************
*  Copyright (C), 2011-2016, ROSIM Co.,Ltd.
*
*  FileName: htos_timer.c
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
*    wabbow             2015/12/26       v1.0          modify
********************************************************************************
*/


/*******************************************************************************
*                            INCLUDES
*/
#include "htos_type.h"
#include "htos.h"
#include "htos_timer.h"
#include "htos_app.h"


/*******************************************************************************
*                          GLOBAL VARIABLES
*/
OS_INT32U    osSysTimeMs = 0;   //used for long delay



/*******************************************************************************
* Function: htos_start_timer()
*
* Input: taskId ---
*        eventFlag ---
*        matchValue ---
*        mode --- 
*
* Output: 
*
* Description: start a timer
*
* specification:
*
* by: wabbow
* data:2015-11-06
*******************************************************************************/
OS_INT08U  htos_start_timer (OS_TASK_ID      taskId,
                             OS_EVENT_FLAG   eventFlag,
                             OS_INT32U       matchValue,
                             OS_INT08U       mode)
{
#if CRITICAL_MODE == 1
    OS_CPU    x = 0;
#endif
    OS_TIME         *timer;
    
    
    if ( taskId >= osTasksCnt){
        return OS_TR_TASKID_ERR;
    }
    
    if( 0 == matchValue ){
        return  OS_TR_MATCH_ERR;
    }
    
    if(MODE_TR_ONCE != mode && MODE_TR_PERIODIC != mode){
        return  OS_TR_MODE_ERR; 
    }
    
    timer = htos_find_timer(taskId, eventFlag);
    
    if( NULL == timer ){
        return  OS_TR_NOT_FIND;
    }
    
    OS_DISABLE_INTERRUPTS(x);
    
    timer->state = ST_TR_START; 
    timer->mode = mode;
    timer->timeMatch = matchValue;
    timer->outSysTime = osSysTimeMs;

    OS_ENABLE_INTERRUPTS(x);
    
    return OS_TR_OPR_SUCCESS;
}

/*******************************************************************************
* Function: htos_start_timer()
* Input: taskId ----
* Output: none
* Description: system entry point function
* specification:
* by:
* data:2015-09-06
*******************************************************************************/
OS_INT08U  htos_stop_timer (OS_TASK_ID      taskId,
                            OS_EVENT_FLAG   eventFlag)
{
#if CRITICAL_MODE == 1
    OS_CPU    x = 0;
#endif
    OS_TIME         *timer;
    
    
    if ( taskId >= osTasksCnt){
        return OS_TR_TASKID_ERR;
    }
    
    timer = htos_find_timer(taskId, eventFlag);
    
    if( NULL == timer ){
        return  OS_TR_NOT_FIND;
    }
    
    OS_DISABLE_INTERRUPTS(x);
    
    timer->state = ST_TR_STOP; 
    timer->mode = MODE_TR_UNUSE;
    timer->timeMatch = 0;
    timer->outSysTime = 0;
        
    OS_ENABLE_INTERRUPTS(x);
    
    return OS_TR_OPR_SUCCESS;
}

/*******************************************************************************
* Function: htos_find_timer()
*
* Input: taskId --- 
*        eventFlag ---
* Output: none
* Description: system Entry point function
* specification:
* by:
* data:2015-09-06
*******************************************************************************/
OS_TIME  *htos_find_timer (OS_TASK_ID      taskId,
                           OS_EVENT_FLAG   eventFlag)
{
    OS_TASK_NUM  i;
    
    
    for (i = 0; i < osTimersCnt; i++){
        if (osTimer[i].taskId == taskId
            && osTimer[i].eventFlag == eventFlag){
                return &osTimer[i];
            }
    }
    
    return NULL;
}


/*******************************************************************************
* Function: htos_update_timer()
* Input: none
* Output: none
* Description: system Entry point function
* specification:
* by:
* data:2015-11-05
*******************************************************************************/
OS_VOID  htos_update_timer (OS_VOID)
{
#if CRITICAL_MODE == 1
    OS_CPU      x = 0;
#endif
    
    //OS_DISABLE_INTERRUPTS(x);
        osSysTimeMs++;
    //OS_ENABLE_INTERRUPTS(x);

}

/*******************************************************************************
* Function: htos_update_timer()
* Input: none
* Output: none
* Description: system Entry point function
* specification:
* by:
* data:2015-11-05
*******************************************************************************/
OS_VOID  htos_timerEvent_update (OS_VOID)
{
#if CRITICAL_MODE == 1
    OS_CPU      x = 0;
#endif
    OS_INT08U   i;
    OS_INT32U   timeElapse;
    
    
    OS_DISABLE_INTERRUPTS(x);
    
    for (i = 0; i < osTimersCnt; i++){
        if (osTimer[i].state == ST_TR_START){
            //Calculate elapse time
            if( osSysTimeMs >= osTimer[i].outSysTime ){  
                timeElapse = osSysTimeMs - osTimer[i].outSysTime;
            }
            else{
                timeElapse = OS_TIMERS_MAX_TIMEOUT - osTimer[i].outSysTime + osSysTimeMs;
            }
            //Timer is running
            if(osTimer[i].mode == MODE_TR_ONCE){
                //Once mode , If time expried , stop timer
                if (osTimer[i].timeMatch <= timeElapse){
                    osTimer[i].state = ST_TR_STOP; 
                    osTimer[i].mode = MODE_TR_UNUSE;
                    osTimer[i].timeMatch = 0;
                    osTimer[i].outSysTime = 0;
                    htos_set_event(osTimer[i].taskId, osTimer[i].eventFlag);
                }
                else {
                }
            }
            else if(osTimer[i].mode == MODE_TR_PERIODIC){
                //Periodic mode
                if (osTimer[i].timeMatch <= timeElapse){
                    osTimer[i].outSysTime = osSysTimeMs;
                    htos_set_event(osTimer[i].taskId, osTimer[i].eventFlag);
                }
                else {
                    
                }
            }
        }
    }
    
    OS_ENABLE_INTERRUPTS(x);
    
}
