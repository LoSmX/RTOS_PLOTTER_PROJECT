/**
 * @file lib_cfg.h
 *
 * @brief CUSTOM LIBRARY CONFIGURATION FILE
 */
#ifndef  LIB_CFG_MODULE_PRESENT
#define  LIB_CFG_MODULE_PRESENT

/******************************************************************************/
/*********************************************** MEMORY LIBRARY CONFIGURATION */

/******************************** MEMORY LIBRARY ARGUMENT CHECK CONFIGURATION */

/**
 * @note Configure LIB_MEM_CFG_ARG_CHK_EXT_EN to enable/disable the memory
 *       library suite external argument check feature :
 *       (a) When DEF_ENABLED, arguments received from any port interface
 *           provided by the developer or application are checked/validated.
 *       (b) When DEF_DISABLED, NO arguments received from any port interface
 *           provided by the developer or application are checked/validated.
 */
#define LIB_MEM_CFG_ARG_CHK_EXT_EN DEF_DISABLED

/************************* MEMORY LIBRARY ASSEMBLY OPTIMIZATION CONFIGURATION */

/**
 * @note Configure LIB_MEM_CFG_OPTIMIZE_ASM_EN to enable/disable
 *       assembly-optimized memory function(s).
 *       DEF_DISABLED     Assembly-optimized function(s) DISABLED
 *       DEF_ENABLED      Assembly-optimized function(s) ENABLED
 */
#define LIB_MEM_CFG_OPTIMIZE_ASM_EN DEF_ENABLED

/******************************************** MEMORY ALLOCATION CONFIGURATION */

/**
 * @note Configure LIB_MEM_CFG_ALLOC_EN to enable/disable memory allocation
 *       functions.
 *       DEF_DISABLED     Memory allocation DISABLED
 *       DEF_ENABLED      Memory allocation ENABLED
 */
#define LIB_MEM_CFG_ALLOC_EN DEF_ENABLED

/**
 * @note Configure LIB_MEM_CFG_HEAP_SIZE with the desired size of heap memory
 *       (in octets).
 */
#define LIB_MEM_CFG_HEAP_SIZE (1u * 1024u)

/**
 * @note Configure LIB_MEM_CFG_HEAP_BASE_ADDR to specify a base address for heap
 *       memory:
 *      (1) Heap initialized to specified application memory,
 *          if LIB_MEM_CFG_HEAP_BASE_ADDR #define'd in app_cfg.h
 *          CANNOT #define to address 0x0
 *      (2) Heap declared to Mem_Heap[] in lib_mem.c,
 *          if LIB_MEM_CFG_HEAP_BASE_ADDR NOT #define'd in app_cfg.h
 */
#if 0
#define LIB_MEM_CFG_HEAP_BASE_ADDR 0x00000000u
#endif

/******************************************************************************/
/*********************************************** STRING LIBRARY CONFIGURATION */

/**************************************** STRING FLOATING POINT CONFIGURATION */

/**
 * @note Configure LIB_STR_CFG_FP_EN to enable/disable floating point string
 *       function(s).
 *       DEF_DISABLED     Floating point functions DISABLED
 *       DEF_ENABLED      Floating point functions ENABLED
 */
#define LIB_STR_CFG_FP_EN DEF_DISABLED

/**
 * @note Configure LIB_STR_CFG_FP_MAX_NBR_DIG_SIG to configure the maximum
 *       number of significant digits to calculate &/or display for floating
 *       point string function(s).
 *       See also lib_str.h
 */
#define LIB_STR_CFG_FP_MAX_NBR_DIG_SIG LIB_STR_FP_MAX_NBR_DIG_SIG_DFLT

#endif
/** EOF */
