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
#include "htos_memory.h"
#include "htos.h"

/*******************************************************************************
*                            CONSTANTS
*/

#define HTOS_MEM_IN_USE             0x8000 

#if (HTOS_MAX_HEAP_SIZE & HTOS_MEM_IN_USE)
#error HTOS_MAX_HEAP_SIZE is too big to manage!
#endif

#define HTOS_MEM_HDR_SIZE              sizeof(htosMemHdr_t)  //头部大小

// Round a value up to the ceiling of HTOS_MEM_HDR_SIZE for critical dependencies on even multiples.
#define HTOSMEM_ROUND(X)       ((((X) + HTOS_MEM_HDR_SIZE - 1) / HTOS_MEM_HDR_SIZE) * HTOS_MEM_HDR_SIZE)

/* Minimum wasted bytes to justify splitting a block before allocation.
* Adjust accordingly to attempt to balance the tradeoff of wasted space and runtime throughput
* spent splitting blocks into sizes that may not be practically usable when sandwiched between
* two blocks in use (and thereby not able to be coalesced.)
* Ensure that this size is an even multiple of HTOS_MEM_HDR_SIZE.
*/
#if !defined HTOSMEM_MIN_BLKSZ
#define HTOSMEM_MIN_BLKSZ         (HTOSMEM_ROUND((HTOS_MEM_HDR_SIZE * 2)))
#endif



/* Adjust accordingly to attempt to accomodate the block sizes of the vast majority of
* very high frequency allocations/frees by profiling the system runtime.
* This default of 16 accomodates the HTOS timers block, htosTimerRec_t, and many others.
* Ensure that this size is an even multiple of HTOSMEM_MIN_BLKSZ for run-time efficiency.
*/
#if !defined HTOSMEM_SMALL_BLKSZ
#define HTOSMEM_SMALL_BLKSZ       (HTOSMEM_ROUND(16))
#endif
#if !defined HTOSMEM_SMALL_BLKCNT
#define HTOSMEM_SMALL_BLKCNT       8
#endif

#if !defined HTOSMEM_LL_BLKSZ
#define HTOSMEM_LL_BLKSZ          (HTOSMEM_ROUND(417) + (19 * HTOS_MEM_HDR_SIZE))
#endif
/*
* These numbers setup the size of the small-block bucket which is reserved at the front of the
* heap for allocations of HTOSMEM_SMALL_BLKSZ or smaller.
*/

// Size of the heap bucket reserved for small block-sized allocations.
// Adjust accordingly to attempt to accomodate the vast majority of very high frequency operations.
#define HTOSMEM_SMALLBLK_BUCKET  ((HTOSMEM_SMALL_BLKSZ * HTOSMEM_SMALL_BLKCNT) + HTOSMEM_LL_BLKSZ)
// Index of the first available htosMemHdr_t after the small-block heap which will be set in-use in
// order to prevent the small-block bucket from being coalesced with the wilderness.
#define HTOSMEM_SMALLBLK_HDRCNT   (HTOSMEM_SMALLBLK_BUCKET / HTOS_MEM_HDR_SIZE)
// Index of the first available htosMemHdr_t after the small-block heap which will be set in-use in
#define HTOSMEM_BIGBLK_IDX        (HTOSMEM_SMALLBLK_HDRCNT + 1)
// The size of the wilderness after losing the small-block heap, the wasted header to block the
// small-block heap from being coalesced, and the wasted header to mark the end of the heap.
#define HTOSMEM_BIGBLK_SZ         (HTOS_MAX_HEAP_SIZE - HTOSMEM_SMALLBLK_BUCKET - HTOS_MEM_HDR_SIZE*2)
// Index of the last available htosMemHdr_t at the end of the heap which will be set to zero for
// fast comparisons with zero to determine the end of the heap.
#define HTOSMEM_LASTBLK_IDX      ((HTOS_MAX_HEAP_SIZE / HTOS_MEM_HDR_SIZE) - 1)

// For information about memory profiling, refer to SWRA204 "Heap Memory Management", section 1.5.
#if !defined HTOSMEM_PROFILER
#define HTOSMEM_PROFILER           FALSE  // Enable/disable the memory usage profiling buckets.
#endif
#if !defined HTOSMEM_PROFILER_LL
#define HTOSMEM_PROFILER_LL        FALSE  // Special profiling of the Long-Lived bucket.
#endif

#if HTOSMEM_PROFILER
#define HTOSMEM_INIT              'X'
#define HTOSMEM_ALOC              'A'
#define HTOSMEM_REIN              'F'
#endif

/* ------------------------------------------------------------------------------------------------
*                                           Typedefs
* ------------------------------------------------------------------------------------------------
*/

typedef struct 
{ 
    unsigned short len : 15;  // The 15 LSB's of 'val' indicate the total item size, including the header, in 8-bit bytes.
    unsigned short inUse : 1; // The 1 MSB of 'val' is used as a boolean to indicate in-use or freed.
}htosMemHdrHdr_t;

/* Dummy variable so compiler forces structure to alignment of largest element while not wasting
* space on targets when the halDataAlign_t is smaller than a UINT16.
*/
typedef union 
{  
    unsigned char   alignDummy;
    unsigned short  val;
    htosMemHdrHdr_t hdr;
} htosMemHdr_t;

/* ------------------------------------------------------------------------------------------------
*                                           Local Variables
* ------------------------------------------------------------------------------------------------
*/

static __no_init htosMemHdr_t theHeap[HTOS_MAX_HEAP_SIZE / HTOS_MEM_HDR_SIZE];
static __no_init htosMemHdr_t *ff1;  // First free block in the small-block bucket.

static unsigned char htosMemStat;            // Discrete status flags: 0x01 = kicked.

#if HTOSMEM_METRICS
static uint16 blkMax;  // Max cnt of all blocks ever seen at once.
static uint16 blkCnt;  // Current cnt of all blocks.
static uint16 blkFree; // Current cnt of free blocks.
static uint16 memAlo;  // Current total memory allocated.
static uint16 memMax;  // Max total memory ever allocated at once.
#endif

#if HTOSMEM_PROFILER
#define HTOSMEM_PROMAX  8
/* The profiling buckets must differ by at least HTOSMEM_MIN_BLKSZ; the
* last bucket must equal the max alloc size. Set the bucket sizes to
* whatever sizes necessary to show how your application is using memory.
*/
static uint16 proCnt[HTOSMEM_PROMAX] = {\
    HTOSMEM_SMALL_BLKSZ, 48, 112, 176, 192, 224, 256, 65535 };
static uint16 proCur[HTOSMEM_PROMAX] = { 0 };
static uint16 proMax[HTOSMEM_PROMAX] = { 0 };
static uint16 proTot[HTOSMEM_PROMAX] = { 0 };
static uint16 proSmallBlkMiss;
#endif

/* ------------------------------------------------------------------------------------------------
*                                           Global Variables
* ------------------------------------------------------------------------------------------------
*/

#ifdef DPRINTF_HEAPTRACE
extern int dprintf(const char *fmt, ...);
#endif /* DPRINTF_HEAPTRACE */

/**************************************************************************************************
* @fn          htos_mem_init
*
* @brief       This function is the HTOS heap memory management initialization callback.
*
* input parameters
*
* None.
*
* output parameters
*
* None.
*
* @return      None.
*/
void htos_mem_init(void)
{
    //HAL_ASSERT(((HTOSMEM_MIN_BLKSZ % HTOS_MEM_HDR_SIZE) == 0));
    //HAL_ASSERT(((HTOSMEM_LL_BLKSZ % HTOS_MEM_HDR_SIZE) == 0));
    //HAL_ASSERT(((HTOSMEM_SMALL_BLKSZ % HTOS_MEM_HDR_SIZE) == 0));
    
#if HTOSMEM_PROFILER
    (void)htos_memset(theHeap, HTOSMEM_INIT, HTOS_MAX_HEAP_SIZE);
#endif
    
    // Setup a NULL block at the end of the heap for fast comparisons with zero.
    theHeap[HTOSMEM_LASTBLK_IDX].val = 0;
    
    // Setup the small-block bucket.
    ff1 = theHeap;
    ff1->val = HTOSMEM_SMALLBLK_BUCKET;                   // Set 'len' & clear 'inUse' field.
    // Set 'len' & 'inUse' fields - this is a 'zero data bytes' lifetime allocation to block the
    // small-block bucket from ever being coalesced with the wilderness.
    theHeap[HTOSMEM_SMALLBLK_HDRCNT].val = (HTOS_MEM_HDR_SIZE | HTOS_MEM_IN_USE);
    
    // Setup the wilderness.
    theHeap[HTOSMEM_BIGBLK_IDX].val = HTOSMEM_BIGBLK_SZ;  // Set 'len' & clear 'inUse' field.
    
#if ( HTOSMEM_METRICS )
    /* Start with the small-block bucket and the wilderness - don't count the
    * end-of-heap NULL block nor the end-of-small-block NULL block.
    */
    blkCnt = blkFree = 2;
#endif
}

/**************************************************************************************************
* @fn          htos_mem_kick
*
* @brief       This function is the HTOS task initialization callback.
* @brief       Kick the ff1 pointer out past the long-lived HTOS Task blocks.
*              Invoke this once after all long-lived blocks have been allocated -
*              presently at the end of htos_init_system().
*
* input parameters
*
* None.
*
* output parameters
*
* None.
*
* @return      None.
*/
void htos_mem_kick(void)
{
#if CRITICAL_MODE == 1
    unsigned short x = 0;
#endif
    
    htosMemHdr_t *tmp = htos_mem_alloc(1);
    OS_DISABLE_INTERRUPTS(x);
    //HAL_ENTER_CRITICAL_SECTION(intState);  // Hold off interrupts.
    
    /* All long-lived allocations have filled the LL block reserved in the small-block bucket.
    * Set 'htosMemStat' so searching for memory in this bucket from here onward will only be done
    * for sizes meeting the HTOSMEM_SMALL_BLKSZ criteria.
    */
    ff1 = tmp - 1;       // Set 'ff1' to point to the first available memory after the LL block.
    htos_mem_free(tmp);
    htosMemStat = 0x01;  // Set 'htosMemStat' after the free because it enables memory profiling.
    OS_ENABLE_INTERRUPTS(x);
    //HAL_EXIT_CRITICAL_SECTION(intState);  // Re-enable interrupts.
}

/**************************************************************************************************
* @fn          htos_mem_alloc
*
* @brief       This function implements the HTOS dynamic memory allocation functionality.
*
* input parameters
*
* @param size - the number of bytes to allocate from the HEAP.
*
* output parameters
*
* None.
*
* @return      None.
*/
#ifdef DPRINTF_HTOSHEAPTRACE
void *htos_mem_alloc_dbg( uint16 size, const char *fname, unsigned lnum )
#else /* DPRINTF_HTOSHEAPTRACE */
void *htos_mem_alloc( unsigned short size )
#endif /* DPRINTF_HTOSHEAPTRACE */
{
    htosMemHdr_t *prev = NULL;
    htosMemHdr_t *hdr;
#if CRITICAL_MODE == 1
    unsigned short x = 0;
#endif
    unsigned char coal = 0;
    
    size += HTOS_MEM_HDR_SIZE;
    
    // Calculate required bytes to add to 'size' to align to halDataAlign_t.
    if ( sizeof( halDataAlign_t ) == 2 ){
        size += (size & 0x01);
    }
    else if ( sizeof( halDataAlign_t ) != 1 ){
        const unsigned char mod = size % sizeof( halDataAlign_t );
        
        if ( mod != 0 ){
            size += (sizeof( halDataAlign_t ) - mod);
        }
    }
    
    if(size == 0 || size > HTOS_MAX_HEAP_SIZE){
        return NULL;
    }
    
    OS_ENABLE_INTERRUPTS(x);
    
    // Smaller allocations are first attempted in the small-block bucket, and all long-lived
    // allocations are channeled into the LL block reserved within this bucket.
    if ((htosMemStat == 0) || (size <= HTOSMEM_SMALL_BLKSZ)){
        hdr = ff1;
    }
    else{
        hdr = (theHeap + HTOSMEM_BIGBLK_IDX);
    }
    
    do{
        if ( hdr->hdr.inUse ){
            coal = 0;
        }
        else{
            if ( coal != 0 ){
#if ( HTOSMEM_METRICS )
                blkCnt--;
                blkFree--;
#endif
                
                prev->hdr.len += hdr->hdr.len;
                
                if ( prev->hdr.len >= size ){
                    hdr = prev;
                    break;
                }
            }
            else{
                if ( hdr->hdr.len >= size ){
                    break;
                }
                
                coal = 1;
                prev = hdr;
            }
        }
        
        hdr = (htosMemHdr_t *)((unsigned char *)hdr + hdr->hdr.len);
        
        if ( hdr->val == 0 )
        {
            hdr = NULL;
            break;
        }
    } while (1);
    
    if ( hdr != NULL )
    {
        unsigned short tmp = hdr->hdr.len - size;
        
        // Determine whether the threshold for splitting is met.
        if ( tmp >= HTOSMEM_MIN_BLKSZ )
        {
            // Split the block before allocating it.
            htosMemHdr_t *next = (htosMemHdr_t *)((unsigned char *)hdr + size);
            next->val = tmp;                     // Set 'len' & clear 'inUse' field.
            hdr->val = (size | HTOS_MEM_IN_USE);  // Set 'len' & 'inUse' field.
            
#if ( HTOSMEM_METRICS )
            blkCnt++;
            if ( blkMax < blkCnt )
            {
                blkMax = blkCnt;
            }
            memAlo += size;
#endif
        }
        else
        {
#if ( HTOSMEM_METRICS )
            memAlo += hdr->hdr.len;
            blkFree--;
#endif
            
            hdr->hdr.inUse = TRUE;
        }
        
#if ( HTOSMEM_METRICS )
        if ( memMax < memAlo )
        {
            memMax = memAlo;
        }
#endif
        
#if ( HTOSMEM_PROFILER )
#if !HTOSMEM_PROFILER_LL
        if (htosMemStat != 0)  // Don't profile until after the LL block is filled.
#endif
        {
            uint8 idx;
            
            for ( idx = 0; idx < HTOSMEM_PROMAX; idx++ )
            {
                if ( hdr->hdr.len <= proCnt[idx] )
                {
                    break;
                }
            }
            proCur[idx]++;
            if ( proMax[idx] < proCur[idx] )
            {
                proMax[idx] = proCur[idx];
            }
            proTot[idx]++;
            
            /* A small-block could not be allocated in the small-block bucket.
            * When this occurs significantly frequently, increase the size of the
            * bucket in order to restore better worst case run times. Set the first
            * profiling bucket size in proCnt[] to the small-block bucket size and
            * divide proSmallBlkMiss by the corresponding proTot[] size to get % miss.
            * Best worst case time on TrasmitApp was achieved at a 0-15% miss rate
            * during steady state Tx load, 0% during idle and steady state Rx load.
            */
            if ((hdr->hdr.len <= HTOSMEM_SMALL_BLKSZ) && (hdr >= (theHeap + HTOSMEM_BIGBLK_IDX)))
            {
                proSmallBlkMiss++;
            }
        }
        
        (void)htos_memset((uint8 *)(hdr+1), HTOSMEM_ALOC, (hdr->hdr.len - HTOS_MEM_HDR_SIZE));
#endif
        
        if ((htosMemStat != 0) && (ff1 == hdr))
        {
            ff1 = (htosMemHdr_t *)((unsigned char *)hdr + hdr->hdr.len);
        }
        
        hdr++;
    }
    
    //HAL_EXIT_CRITICAL_SECTION( intState );  // Re-enable interrupts.
    OS_ENABLE_INTERRUPTS(x);
        
#ifdef DPRINTF_HTOSHEAPTRACE
        dprintf("htos_mem_alloc(%u)->%lx:%s:%u\n", size, (unsigned) hdr, fname, lnum);
#endif /* DPRINTF_HTOSHEAPTRACE */
    return (void *)hdr;
}

/**************************************************************************************************
* @fn          htos_mem_free
*
* @brief       This function implements the HTOS dynamic memory de-allocation functionality.
*
* input parameters
*
* @param ptr - A valid pointer (i.e. a pointer returned by htos_mem_alloc()) to the memory to free.
*
* output parameters
*
* None.
*
* @return      None.
*/
#ifdef DPRINTF_HTOSHEAPTRACE
void htos_mem_free_dbg(void *ptr, const char *fname, unsigned lnum)
#else /* DPRINTF_HTOSHEAPTRACE */
void htos_mem_free(void *ptr)
#endif /* DPRINTF_HTOSHEAPTRACE */
{
    htosMemHdr_t *hdr = (htosMemHdr_t *)ptr - 1;
#if CRITICAL_MODE == 1
    unsigned short x = 0;
#endif
    
#ifdef DPRINTF_HTOSHEAPTRACE
    dprintf("htos_mem_free(%lx):%s:%u\n", (unsigned) ptr, fname, lnum);
#endif /* DPRINTF_HTOSHEAPTRACE */
    
    
    //HAL_ENTER_CRITICAL_SECTION( intState );  // Hold off interrupts.
    OS_DISABLE_INTERRUPTS(x);
    
    hdr->hdr.inUse = FALSE;
    
    if (ff1 > hdr)
    {
        ff1 = hdr;
    }
    
#if HTOSMEM_PROFILER
#if !HTOSMEM_PROFILER_LL
    if (htosMemStat != 0)  // Don't profile until after the LL block is filled.
#endif
    {
        uint8 idx;
        
        for (idx = 0; idx < HTOSMEM_PROMAX; idx++)
        {
            if (hdr->hdr.len <= proCnt[idx])
            {
                break;
            }
        }
        
        proCur[idx]--;
    }
    
    (void)htos_memset((uint8 *)(hdr+1), HTOSMEM_REIN, (hdr->hdr.len - HTOS_MEM_HDR_SIZE) );
#endif
#if HTOSMEM_METRICS
    memAlo -= hdr->hdr.len;
    blkFree++;
#endif
    
    OS_ENABLE_INTERRUPTS(x);
    //HAL_EXIT_CRITICAL_SECTION( intState );  // Re-enable interrupts.
}

#if HTOSMEM_METRICS
/*********************************************************************
* @fn      htos_heap_block_max
*
* @brief   Return the maximum number of blocks ever allocated at once.
*
* @param   none
*
* @return  Maximum number of blocks ever allocated at once.
*/
uint16 htos_heap_block_max( void )
{
    return blkMax;
}

/*********************************************************************
* @fn      htos_heap_block_cnt
*
* @brief   Return the current number of blocks now allocated.
*
* @param   none
*
* @return  Current number of blocks now allocated.
*/
uint16 htos_heap_block_cnt( void )
{
    return blkCnt;
}

/*********************************************************************
* @fn      htos_heap_block_free
*
* @brief   Return the current number of free blocks.
*
* @param   none
*
* @return  Current number of free blocks.
*/
uint16 htos_heap_block_free( void )
{
    return blkFree;
}

/*********************************************************************
* @fn      htos_heap_mem_used
*
* @brief   Return the current number of bytes allocated.
*
* @param   none
*
* @return  Current number of bytes allocated.
*/
uint16 htos_heap_mem_used( void )
{
    return memAlo;
}
#endif

#if defined (ZTOOL_P1) || defined (ZTOOL_P2)
/*********************************************************************
* @fn      htos_heap_high_water
*
* @brief   Return the highest byte ever allocated in the heap.
*
* @param   none
*
* @return  Highest number of bytes ever used by the stack.
*/
uint16 htos_heap_high_water( void )
{
#if ( HTOSMEM_METRICS )
    return memMax;
#else
    return HTOS_MAX_HEAP_SIZE;
#endif
}
#endif

/**************************************************************************************************
*/
