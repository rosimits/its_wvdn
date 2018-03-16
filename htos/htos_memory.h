/*
********************************************************************************
*  Copyright (C), 2011-2016, ROSIM Co.,Ltd.
*
*  FileName: htos_memory.h
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

#ifndef _HTOS_MEMORY_H
#define _HTOS_MEMORY_H

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
*                      INCLUDES
*/

 
/*********************************************************************
 * CONSTANTS
 */

#if !defined ( OSALMEM_METRICS )
  #define OSALMEM_METRICS  FALSE
#endif

/*********************************************************************
 * MACROS
 */
  
#define htos_stack_used()  OnBoard_stack_used()
    
#define  HTOS_MAX_HEAP_SIZE    1500
    
    
#ifndef  NULL
#define  NULL    (void *)0
#endif
    
#ifndef TRUE
#define TRUE     1
#endif
    
#ifndef FALSE
#define FALSE    0
#endif

/*********************************************************************
 * TYPEDEFS
 */
typedef unsigned short          halDataAlign_t;

/*********************************************************************
 * GLOBAL VARIABLES
 */
 
/*******************************************************************************
*                           FUNCTIONS
*/

 /*
  * Initialize memory manager.
  */
  void htos_mem_init( void );

 /*
  * Setup efficient search for the first free block of heap.
  */
  void htos_mem_kick( void );

 /*
  * Allocate a block of memory.
  */
#ifdef DPRINTF_OSALHEAPTRACE
  void *htos_mem_alloc_dbg( uint16 size, const char *fname, unsigned lnum );
#define htos_mem_alloc(_size ) htos_mem_alloc_dbg(_size, __FILE__, __LINE__)
#else /* DPRINTF_OSALHEAPTRACE */
  void *htos_mem_alloc( unsigned short size );
#endif /* DPRINTF_OSALHEAPTRACE */

 /*
  * Free a block of memory.
  */
#ifdef DPRINTF_OSALHEAPTRACE
  void htos_mem_free_dbg( void *ptr, const char *fname, unsigned lnum );
#define htos_mem_free(_ptr ) htos_mem_free_dbg(_ptr, __FILE__, __LINE__)
#else /* DPRINTF_OSALHEAPTRACE */
  void htos_mem_free( void *ptr );
#endif /* DPRINTF_OSALHEAPTRACE */

#if ( OSALMEM_METRICS )
 /*
  * Return the maximum number of blocks ever allocated at once.
  */
  uint16 htos_heap_block_max( void );

 /*
  * Return the current number of blocks now allocated.
  */
  uint16 htos_heap_block_cnt( void );

 /*
  * Return the current number of free blocks.
  */
  uint16 htos_heap_block_free( void );

 /*
  * Return the current number of bytes allocated.
  */
  uint16 htos_heap_mem_used( void );
#endif

#if defined (ZTOOL_P1) || defined (ZTOOL_P2)
 /*
  * Return the highest number of bytes ever used in the heap.
  */
  uint16 htos_heap_high_water( void );
#endif

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* #ifndef OSAL_MEMORY_H */
