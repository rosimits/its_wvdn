/*
********************************************************************************
*  Copyright (C), 2011-2016, ROSIM Co.,Ltd.
*
*  FileName: htos_pwr_mgr.c
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

/*******************************************************************************
* INCLUDES
*/
#include "htos.h"
#include "htos_app.h"
#include "htos_pwr_mgr.h"
#include "htos_timer.h"

#include "includes.h"


/*******************************************************************************
* Function: htos_pwrmgr_powerconserve()
* Input: none
* Output: none
* Description: 
* specification:
*******************************************************************************/
void htos_pwrmgr_powerconserve( void )
{
    // PWR->CR |= PWR_CR_ULP;
    SysTick->CTRL &= (~0x000000001);      
    //SetSysClockLoPower();
    //PWR->CR |= PWR_CR_ULP + PWR_CR_FWU;
    //rfIOLowPowerIn();
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON,PWR_STOPENTRY_WFI);
    //HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON,PWR_SLEEPENTRY_WFI);
    //HAL_PWR_EnterSTANDBYMode();
}

/*********************************************************************
*********************************************************************/
