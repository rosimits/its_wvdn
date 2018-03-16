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
*    wabbow             2015/10/29       v1.0          modify
********************************************************************************
*/
#define   APP_TASK

/*******************************************************************************
*                          INCLUDE
*/
#include "htos_type.h"
#include "htos.h"
#include "htos_memory.h"
#include "htos_timer.h"
#include "htos_app.h"
#include "stm32l1xx.h"

/*******************************************************************************
*                          FUNCTION
*/
void halInit(void);
void halTaskDeal(void);
void nwkInit(void);
void nwkTaskDeal(void);
void appInit(void);
void appTaskDeal(void);

/*******************************************************************************
*                         GLOABLE VARIABLES 
*/
// Establish task timer
OS_TIME osTimer[] = {
    {0 , 0 , HAL_TASK_ID , HAL_CLEAR_OUTSIDE_WDT_EVENT },
    {0 , 0 , APP_TASK_ID , APP_SYS_MANAGER_EVENT },
    {0 , 0 , APP_TASK_ID , APP_MAG_SAMPLE_EVENT },
    {0 , 0 , APP_TASK_ID , APP_IR_DEAL_EVENT},
    {0 , 0 , NWK_TASK_ID , NWK_MANAGE_EVENT},
    {0 , 0 , APP_TASK_ID , APP_SEND_CFG_CHN_EVENT},
    {0 , 0 , APP_TASK_ID , APP_SEND_HEARTBEAT_EVENT}
};
// Calculate timer number
const unsigned char osTimersCnt = sizeof( osTimer ) / sizeof( osTimer[0] );


//task stack
const TaskFuc osTaskFuc[]={\
    halTaskDeal,
    nwkTaskDeal,
    appTaskDeal
};


const OS_TASK_NUM    osTasksCnt = sizeof( osTaskFuc ) / sizeof( osTaskFuc[0] );
OS_EVENT_FLAG        osTasksEvents[sizeof( osTaskFuc ) / sizeof( osTaskFuc[0] )];
OS_EVENT_FLAG        osTaskEventsMasks[sizeof( osTaskFuc ) / sizeof( osTaskFuc[0] )];


/*******************************************************************************
* Function: halTaskInit()
* Input: none
* Output: none
* Description: 
* specification:
*******************************************************************************/
__weak void halTaskInit(void)
{
   return ; 
}


/*******************************************************************************
* Function: halTaskDeal()
* Input: none
* Output: none
* Description: 
* specification:
*******************************************************************************/
__weak void halTaskDeal(void)
{
   return ; 
}


/*******************************************************************************
* Function: nwkTaskInit()
* Input: none
* Output: none
* Description: 
* specification:
*******************************************************************************/
__weak void nwkTaskInit(void)
{
   return ; 
}


/*******************************************************************************
* Function: nwkTaskDeal()
* Input: none
* Output: none
* Description: 
* specification:
*******************************************************************************/
__weak void nwkTaskDeal(void)
{
   return ;
}


/*******************************************************************************
* Function: appTaskInit()
* Input: none
* Output: none
* Description: 
* specification:
*******************************************************************************/
__weak void appTaskInit(void)
{
   return ; 
}


/*******************************************************************************
* Function: appTaskDeal()
* Input: none
* Output: none
* Description: 
* specification:
*******************************************************************************/
__weak void appTaskDeal(void)
{
    
   return ;
}


/*******************************************************************************
* Function: htosInitTasks()
* Input: none
* Output: none
* Description: 
* specification:
*******************************************************************************/
unsigned char htosInitTasks( void )
{
    unsigned char err = 0;
    
    
    halTaskInit();
    nwkTaskInit();
    appTaskInit();
    
    return err;
}
