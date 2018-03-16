/*
********************************************************************************
*  Copyright (C), 2011-2016, ROSIM Co.,Ltd.
*
*  FileName: htos_app.h
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
#ifndef _HTOS_APP_H
#define _HTOS_APP_H

#ifdef __cplusplus
extern "C"
{
#endif

    
//#include "htos_timer.h"

#ifdef APP_TASK
#define APP_TYPE
#else
#define APP_TYPE   extern
#endif

/*******************************************************************************
*                         Task  define
*******************************************************************************/
#define    ALL_CLEAR_EVENT                  0xFFFFFFFF

/*
 * HAL task referance
 */ 
#define    HAL_TASK_ID              0  //task ID

#define    HAL_CLEAR_OUTSIDE_WDT_EVENT                0x00000001  //received message from PHY    
//#define    PHY_RX_DATA_EVENT                0x00000001  //received message from PHY

    
    
/*
 * NWK task referance
 */
#define    NWK_TASK_ID		    1
//NWK manager event
#define    NWK_MANAGE_EVENT	            0x00000001
#define    NWK_MANAGE_EVENT1	            0x00000002
#define    NWK_MANAGE_EVENT2	            0x00000004
#define    NWK_MANAGE_EVENT3	            0x00000008
//NWK receive event
#define    NWK_REC_MESSAGE_EVENT            0x00000010
     
//NWK send event
#define    NWK_SEND_MESSAGE_EVENT           0x00000100

//NWK SACN TIMER event
//#define    NWK_SCAN_TIMER_EVENT             0x00001000
//#define    NWK_JION_TIMER_EVENT             0x00002000
//#define    NWK_DATA_SEND_TIMER_EVENT        0x00004000

     
    
/*
 * APP task referance
 */   
#define    APP_TASK_ID            2
    
//App system  event
#define    APP_SYS_MANAGER_EVENT                    0x00000001
//#define APP_SYS_EVENT1		            0x00000002
//#define APP_SYS_EVENT2		            0x00000004
//#define APP_SYS_EVENT3		            0x00000008
//#define APP_SYS_EVENT4		            0x00000010
//#define APP_SYS_EVENT6		            0x00000020
//#define APP_SYS_EVENT7		            0x00000040
//#define APP_SYS_EVENT5		            0x00000080

//App maginatic event     
#define    APP_MAG_SAMPLE_EVENT             0x00000100
#define    APP_IR_DEAL_EVENT               0x00000200
//#define APP_MAG_DEAL_EVENT1                       0x00000400
//#define APP_MAG_DEAL_EVENT2                       0x00000800    
    
//App RF event  
//#define    APP_RF_DATA_DEAL_EVENT               0x00001000
#define    APP_RF_DATA_SEND_ASYN_EVENT          0x00002000

//App idr event  
//#define    APP_VOLT_DEAL_EVENT              0x00010000
    
//App send heartbeat
#define    APP_SEND_CFGINFO_EVENT           0x00100000
#define    APP_SEND_HEARTBEAT_EVENT         0x00200000
#define    APP_SEND_REALTIME_DETECT_EVENT   0x00400000
//#define    APP_SEND_UPG_SWITCH_EVENT_EVENT  0x00800000
//#define    APP_SEND_MODIFY_CHN_EVENT        0x01000000
//App in configuration channel
#define    APP_SEND_CFG_CHN_EVENT                     0x10000000
//#define    APP_SEND_CFG_CHN_TOKEN_OK_EVENT            0x20000000
//#define    APP_SEND_CFG_CHN_DEV_INFO_OK_EVENT         0x40000000

    



/*******************************************************************************
*                         GLOABLE VARIAVLES DEFINE
*******************************************************************************/
APP_TYPE unsigned char   halTaskID;
APP_TYPE unsigned char   nwkTaskID;
APP_TYPE unsigned char   appTaskID;

extern const unsigned char   osTimersCnt;
extern OS_TIME osTimer[];

extern const OS_TASK_NUM     osTasksCnt;
extern OS_EVENT_FLAG         osTasksEvents[];
extern OS_EVENT_FLAG         osTaskEventsMasks[];
extern const TaskFuc         osTaskFuc[];


/*******************************************************************************
*                         FUNCTIONS DECLARATION
*******************************************************************************/
extern unsigned char htosInitTasks( void );


#ifdef __cplusplus
}
#endif

#endif