/*
********************************************************************************
*  Copyright (C), 2011-2016, ROSIM Co.,Ltd.
*
*  FileName: htos_debug.h
*
*  Author:  wabbow      Version :  v1.0        Date: 2015/11/05
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
#ifndef _HTOS_TYPE_H
#define _HTOS_TYPE_H

#include "htos_cpu.h"

#ifdef __cplusplus
extern "C"
{
#endif


typedef   CPU_VOID        OS_VOID;
typedef   CPU_CHAR        OS_CHAR;
typedef   CPU_BOOLEAN     OS_BOOLEAN;
typedef   CPU_INT08U      OS_INT08U ;
typedef   CPU_INT08S      OS_INT08S ;
typedef   CPU_INT16U      OS_INT16U ;
typedef   CPU_INT16S      OS_INT16S ;
typedef   CPU_INT32U      OS_INT32U;
typedef   CPU_INT32S      OS_INT32S;
typedef   CPU_INT64U      OS_INT64U ;
typedef   CPU_INT64S      OS_INT64S ;


typedef   CPU_INT08U      OS_TASK_ID;
typedef   CPU_INT32U      OS_EVENT_FLAG;
typedef   CPU_INT32U      OS_CPU;     //depend on cup bits
typedef   CPU_INT08U      OS_TASK_NUM;

/*******************************************************************************
*                        htos.c Structure define
*/ 

typedef struct _htos_msg_hdr_t
{
    void *next;
    unsigned short len;
    unsigned char dest_id;
}htos_msg_hdr_t;

typedef struct
{
  unsigned char  event;
  unsigned char  status;
} htos_event_hdr_t;

typedef void * htos_msg_q_t;


/*******************************************************************************
*                            htos_timer STRUCTURE DEFINE
*/
//Timer structure
typedef struct
{
    OS_INT08U         state;  // 0: unuse ; 1:start ; 2: stop
    OS_INT08U         mode;   // 0: unuse ; 1:once mode ; 2: periodic
    OS_TASK_ID        taskId;      //This timer will trigger the reference task
    OS_EVENT_FLAG     eventFlag;   //The reference event to deal with
    OS_INT32U         timeMatch;   //Timer expired value
    OS_INT32U         outSysTime;  //The value last time get
}OS_TIME;


/*******************************************************************************
*                            htos_app.h STRUCTURE DEFINE
*/
//function task event 
typedef  void (*TaskFuc)(void);
typedef  void (*pIdleHandlerFn)(void);

#ifdef __cplusplus
}
#endif

#endif