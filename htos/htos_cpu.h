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
#ifndef _HTOS_CPU_H
#define _HTOS_CPU_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

/*******************************************************************************
*           Data type different between IAR_ARM ,IAR_MSP430 and IAR_8051
*                         IAR_ARM         IAR_MSP430      IAR_8051
* bool                     8bits             8bits          8bits
* char                     8bits             8bits          8bits
* signed char              8bits             8bits          8bits
* unsiged char             8bits             8bits          8bits
* signed short             16bits            16bits         16bits
* unsigned short           16bits            16bits         16bits
* signed int               32bits            16bits         16bits
* unsigned int             32bits            16bits         16bits
* signed long              32bits            32bits         32bits
* unsigned long            32bits            32bits         32bits
* signed long long         64bits            64bits         32bits
* unsigned long long       64bits            64bits         32bits
* float                    32bits            32bits         32bits
* double                   64bits            32bits(64)     32bits
* long double              64bits            32bits(64)     32bits
*******************************************************************************/
#define IAR_NONE            0
#define IAR_ARM             1
#define IAR_MSP430          2
#define IAR_8051            3

#define CPU_CORE      IAR_ARM

#if CPU_CORE == IAR_ARM
//#warning "CPU_CORE = IAR_ARM select"
typedef            void        CPU_VOID;
typedef            char        CPU_CHAR;                        /*  8-bit character                                     */
typedef            bool        CPU_BOOLEAN;                     /*  8-bit boolean or logical                            */
typedef  unsigned  char        CPU_INT08U;                      /*  8-bit unsigned integer                              */
typedef    signed  char        CPU_INT08S;                      /*  8-bit   signed integer                              */
typedef  unsigned  short       CPU_INT16U;                      /* 16-bit unsigned integer                              */
typedef    signed  short       CPU_INT16S;                      /* 16-bit   signed integer                              */
typedef  unsigned  int         CPU_INT32U;                      /* 32-bit unsigned integer                              */
typedef    signed  int         CPU_INT32S;                      /* 32-bit   signed integer                              */
typedef  unsigned  long  long  CPU_INT64U;                      /* 64-bit unsigned integer                              */
typedef    signed  long  long  CPU_INT64S;                      /* 64-bit   signed integer                              */

typedef            float       CPU_FP32;                        /* 32-bit floating point                                */
typedef            double      CPU_FP64;                        /* 64-bit floating point                                */


typedef  volatile  CPU_INT08U  CPU_REG08;                       /*  8-bit register                                      */
typedef  volatile  CPU_INT16U  CPU_REG16;                       /* 16-bit register                                      */
typedef  volatile  CPU_INT32U  CPU_REG32;                       /* 32-bit register                                      */
typedef  volatile  CPU_INT64U  CPU_REG64;                       /* 64-bit register                                      */


typedef            void      (*CPU_FNCT_VOID)(void);            /* See Note #2a.                                        */
typedef            void      (*CPU_FNCT_PTR )(void *p_obj);     /* See Note #2b.                                        */

#elif CPU_CORE == IAR_MSP430
#warning "CPU_CORE = IAR_MSP430 select"
typedef            void        CPU_VOID;
typedef            char        CPU_CHAR;                        /*  8-bit character                                     */
typedef            bool        CPU_BOOLEAN;                     /*  8-bit boolean or logical                            */
typedef  unsigned  char        CPU_INT08U;                      /*  8-bit unsigned integer                              */
typedef    signed  char        CPU_INT08S;                      /*  8-bit   signed integer                              */
typedef  unsigned  short       CPU_INT16U;                      /* 16-bit unsigned integer                              */
typedef    signed  short       CPU_INT16S;                      /* 16-bit   signed integer                              */
typedef  unsigned  int         CPU_INT32U;                      /* 32-bit unsigned integer                              */
typedef    signed  int         CPU_INT32S;                      /* 32-bit   signed integer                              */
typedef  unsigned  long  long  CPU_INT64U;                      /* 64-bit unsigned integer                              */
typedef    signed  long  long  CPU_INT64S;                      /* 64-bit   signed integer                              */

typedef            float       CPU_FP32;                        /* 32-bit floating point                                */
typedef            double      CPU_FP32;                        /* 64-bit floating point                                */


typedef  volatile  CPU_INT08U  CPU_REG08;                       /*  8-bit register                                      */
typedef  volatile  CPU_INT16U  CPU_REG16;                       /* 16-bit register                                      */
typedef  volatile  CPU_INT32U  CPU_REG32;                       /* 32-bit register                                      */
typedef  volatile  CPU_INT64U  CPU_REG64;                       /* 64-bit register                                      */


typedef            void      (*CPU_FNCT_VOID)(void);            /* See Note #2a.                                        */
typedef            void      (*CPU_FNCT_PTR )(void *p_obj);     /* See Note #2b.                                        */

#elif CPU_CORE == IAR_8051
#warning "CPU_CORE = IAR_8051 select"
typedef            void        CPU_VOID;
typedef            char        CPU_CHAR;                        /*  8-bit character                                     */
typedef            bool        CPU_BOOLEAN;                     /*  8-bit boolean or logical                            */
typedef  unsigned  char        CPU_INT08U;                      /*  8-bit unsigned integer                              */
typedef    signed  char        CPU_INT08S;                      /*  8-bit   signed integer                              */
typedef  unsigned  short       CPU_INT16U;                      /* 16-bit unsigned integer                              */
typedef    signed  short       CPU_INT16S;                      /* 16-bit   signed integer                              */
typedef  unsigned  int         CPU_INT32U;                      /* 32-bit unsigned integer                              */
typedef    signed  int         CPU_INT32S;                      /* 32-bit   signed integer                              */
//typedef  unsigned  long  long  CPU_INT32U;                      /* 64-bit unsigned integer                              */
//typedef    signed  long  long  CPU_INT32S;                      /* 64-bit   signed integer                              */

typedef            float       CPU_FP32;                        /* 32-bit floating point                                */
typedef            double      CPU_FP32;                        /* 64-bit floating point                                */


typedef  volatile  CPU_INT08U  CPU_REG08;                       /*  8-bit register                                      */
typedef  volatile  CPU_INT16U  CPU_REG16;                       /* 16-bit register                                      */
typedef  volatile  CPU_INT32U  CPU_REG32;                       /* 32-bit register                                      */
//typedef  volatile  CPU_INT64U  CPU_REG64;                       /* 64-bit register                                      */


typedef            void      (*CPU_FNCT_VOID)(void);            /* See Note #2a.                                        */
typedef            void      (*CPU_FNCT_PTR )(void *p_obj);     /* See Note #2b.                                        */

#else
#error "CPU_CORE no select"
#endif

/*******************************************************************************
*            MACRO FOR CRITICAL CODE
*
*******************************************************************************/
#define CRITICAL_MODE    0

#if  CRITICAL_MODE == 0
static inline void disable_irp( void )
{
asm( "CPSID I" );
}

static inline void enable_irp( void)
{
asm( "CPSIE I" );
}

#define OS_DISABLE_INTERRUPTS(x)    disable_irp()
#define OS_ENABLE_INTERRUPTS(x)     enable_irp()

#elif CRITICAL_MODE == 1

#else

#endif

/*******************************************************************************
*            MACRO FOR WATCH CODE
*
*******************************************************************************/
#define OS_WATCH_START()
#define OS_WATCH_CLEAR()
#define OS_WATCHDOG


/*******************************************************************************
*         FLASH MACRO DEFINE
*
*******************************************************************************/


#ifdef __cplusplus
}
#endif

#endif