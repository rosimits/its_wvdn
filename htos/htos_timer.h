/*
********************************************************************************
*  Copyright (C), 2011-2016, ROSIM Co.,Ltd.
*
*  FileName: htos_timer.h
*
*  Author:  wabbow      Version :  v1.0        Date: 2015/09/08
*
*  Description:
*
*  Others
*
*  Function List:   
*    1. -------
*
*  History:    
*    
*    1.Date
*      Author:
*      Modification:
********************************************************************************
*/

#ifndef HTOS_TIMER_H
#define HTOS_TIMER_H

#include "htos_cpu.h"
#include "htos_type.h"
//#include "htos_app.h"

#ifdef __cplusplus
extern "C"
{
#endif
    
/*******************************************************************************
*                            MACRO DEFINE
*/
#define   OS_TIMERS_MAX_TIMEOUT  0xFFFFFFFF

//Timer set result
#define   OS_TR_OPR_SUCCESS      0
#define   OS_TR_TASKID_ERR       1
#define   OS_TR_MATCH_ERR        2
#define   OS_TR_MODE_ERR         3
#define   OS_TR_NOT_FIND         4
//Timer state 
#define   ST_TR_UNUSE            0
#define   ST_TR_START            1
#define   ST_TR_STOP             2
//Timer mode
#define   MODE_TR_UNUSE          0
#define   MODE_TR_ONCE           1
#define   MODE_TR_PERIODIC       2


    



/*******************************************************************************
*                     GLOBAL VARIABLES DECRLARATION
*/
extern OS_TIME          htosTimer[];



/*******************************************************************************
*                           FUNCTIONS
*/
OS_INT08U  htos_start_timer       (OS_TASK_ID      taskId,
                                   OS_EVENT_FLAG   eventFlag,
                                   OS_INT32U       matchValue,
                                   OS_INT08U        mode);
OS_INT08U  htos_stop_timer        (OS_TASK_ID      taskId,
                                   OS_EVENT_FLAG   eventFlag);
OS_VOID    htos_start_periodTimer (OS_TASK_ID taskId,
                                   OS_EVENT_FLAG eventFlag,
                                   OS_INT32U reloadTimeout);
OS_TIME    *htos_find_timer       (OS_TASK_ID taskId,
                                   OS_EVENT_FLAG eventFlag);
OS_VOID    htos_update_timer      (OS_VOID);
OS_VOID    htos_timerEvent_update (OS_VOID);


#ifdef __cplusplus
}
#endif

#endif