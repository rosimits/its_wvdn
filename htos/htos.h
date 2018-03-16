/*
********************************************************************************
*  Copyright (C), 2011-2016, ROSIM Co.,Ltd.
*
*  FileName: htos.h
*
*  Author:  wabbow      Version :  v1.0        Date: 2015/11/06
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
#ifndef _HTOS_H
#define _HTOS_H

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
*                        Header file
*/      
#include "htos_cpu.h"
#include "htos_type.h"

#ifndef  NULL
#define  NULL    (void *)0
#endif
    
    
/*******************************************************************************
*                        Macros
*/    
/*** Generic Status Return Values ***/
#define HTOS_SUCCESS                   0x00
#define HTOS_FAILURE                   0x01
#define HTOS_INVALIDPARAMETER          0x02
#define HTOS_INVALID_TASK              0x03
#define HTOS_MSG_BUFFER_NOT_AVAIL      0x04
#define HTOS_INVALID_MSG_POINTER       0x05
#define HTOS_INVALID_EVENT_ID          0x06
#define HTOS_INVALID_INTERRUPT_ID      0x07
#define HTOS_NO_TIMER_AVAIL            0x08
#define HTOS_NV_ITEM_UNINIT            0x09
#define HTOS_NV_OPER_FAILED            0x0A
#define HTOS_INVALID_MEM_SIZE          0x0B
#define HTOS_NV_BAD_ITEM_LEN           0x0C

#define HTOS_TASK_NO_TASK      0xFF

#define HTOS_INTS_ALL    0xFF
    
#define HTOS_POWER_SAVING_EN           1  // 1:enable power saving func

    
#ifdef PARK_SENSOR_NODE_DEVICE_FOR_GATEWAY
	#define TASKS_IDLE_TIME_OUT_VALUE		50
#else
	#define TASKS_IDLE_TIME_OUT_VALUE		200
#endif    
    



/*******************************************************************************
*                          GLOBAL VARIABLES
*/


/*******************************************************************************
*                           FUNCTIONS
*/
extern OS_INT08U      htos_set_event( OS_TASK_ID     task_id,
                                      OS_EVENT_FLAG  event_flag );
extern OS_INT08U      htos_clear_event( OS_TASK_ID      task_id,
                                        OS_EVENT_FLAG   event_flag );
extern OS_EVENT_FLAG  htos_get_event( OS_TASK_ID task_id);
extern OS_INT08U      htos_mask_event( OS_TASK_ID      task_id,
                                       OS_EVENT_FLAG   event_flag);
extern OS_INT08U      htos_unmask_event( OS_TASK_ID      task_id,
                                         OS_EVENT_FLAG   event_flag);
extern OS_VOID        htos_run_system( OS_VOID );
  



#ifdef __cplusplus
}
#endif

#endif