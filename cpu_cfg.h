/**
 * @file cpu_cfg.h
 *
 * @brief CPU CONFIGURATION FILE
 */
#ifndef  CPU_CFG_MODULE_PRESENT
#define  CPU_CFG_MODULE_PRESENT

/***************************************************** CPU NAME CONFIGURATION */

/* Configure CPU host name feature: DEF_DISABLED .... CPU host name DISABLED
 *                                  DEF_ENABLED .... CPU host name ENABLED
 */
#define CPU_CFG_NAME_EN  DEF_ENABLED

/* Configure CPU host name ASCII string size ... see also 'cpu_core.h
 */
#define CPU_CFG_NAME_SIZE 16


/************************************************* CPU TIMESTAMP CONFIGURATION */

/* Configure CPU timestamp features DEF_DISABLED  ... CPU timestamps DISABLED
 *                                  DEF_ENABLED   ... CPU timestamps ENABLED
 */
#define CPU_CFG_TS_32_EN DEF_ENABLED
#define CPU_CFG_TS_64_EN DEF_DISABLED

/* Configure CPU timestamp timer word size:
 *  CPU_WORD_SIZE_08         8-bit word size
 *  CPU_WORD_SIZE_16        16-bit word size
 *  CPU_WORD_SIZE_32        32-bit word size
 *  CPU_WORD_SIZE_64        64-bit word size
 *
 * @note: If the size of the CPU timestamp timer is not a binary multiple of
 *        8-bit octets (e.g. 20-bits or even 24-bits), then the next lower,
 *        binary-multiple octet word size SHOULD be configured (e.g., to
 *        16-bits). However, the minimum supported word size for CPU timestamp
 *        timers is 8-bits. ... see also cpu_core.h
 */
#define CPU_CFG_TS_TMR_SIZE CPU_WORD_SIZE_32

/********************* CPU INTERRUPTS DISABLED TIME MEASUREMENT CONFIGURATION */

/*
 * @note Configure CPU_CFG_INT_DIS_MEAS_EN to enable/disable measuring CPU's
 *       interrupts disabled time:
 *       (a)  Enabled, if CPU_CFG_INT_DIS_MEAS_EN      #define'd in 'cpu_cfg.h'
 *       (b) Disabled, if CPU_CFG_INT_DIS_MEAS_EN  NOT #define'd in 'cpu_cfg.h'
 *       See also cpu_core.h
 */
#if 0
#define  CPU_CFG_INT_DIS_MEAS_EN
#endif

/*
 * @note Configure CPU_CFG_INT_DIS_MEAS_OVRHD_NBR with the number of times to
 *       measure & average the interrupts disabled time measurements overhead.
 *       Recommend a single (1) overhead time measurement, even for
 *       instruction-cache-enabled CPUs, since critical sections are NOT
 *       typically called within instruction-cached loops.
 *       Thus a single non-cached/non-averaged time measurement is a more
 *       realistic overhead for the majority of non-cached interrupts disabled
 *       time measurements.
 *       See also cpu_core.c
 */
#define  CPU_CFG_INT_DIS_MEAS_OVRHD_NBR 1u

/********************************************** CPU COUNT ZEROS CONFIGURATION */

/**
 * @note Configure CPU_CFG_LEAD_ZEROS_ASM_PRESENT to define count leading zer *       bits function(s) in:
 *       (1) 'cpu_a.asm', if CPU_CFG_LEAD_ZEROS_ASM_PRESENT #define'd n cpu.h
 *           and cpu_cfg.h to enable assembly-optimized function(s)
 *       (2) 'cpu_core.c', if CPU_CFG_LEAD_ZEROS_ASM_PRESENT NOT #define'd in
 *            cpu.h/cpu_cfg.h to enable C-source-optimized function(s)
 *            otherwise
 */
#if 1
#define CPU_CFG_LEAD_ZEROS_ASM_PRESENT
#endif

/**
 * @note Configure CPU_CFG_TRAIL_ZEROS_ASM_PRESENT to define count traling
 *       zeros bits function(s) in:
 *       (1) 'cpu_a.asm', if CPU_CFG_TRAIL_ZEROS_ASM_PRESENT #define'd in
 *            cpu.h/cpu_cfg.h to enable assembly-optimized function(s)
 *       (2) 'cpu_core.c', if CPU_CFG_TRAIL_ZEROS_ASM_PRESENT  NOT #define'd in
 *            cpu.h/cpu_cfg.h to enable C-source-optimized function(s)
 *            otherwise
 */
#if 0
#define CPU_CFG_TRAIL_ZEROS_ASM_PRESENT
#endif

/*********************************************************** CACHE MANAGEMENT */

/**
 * @note Configure CPU_CFG_CACHE_MGMT_EN to enable the cache managment API.
 *       Defining CPU_CFG_CACHE_MGMT_EN to DEF_ENABLED only enable the cache
 *       management function. Cache are assumed to be configured and enabled by
 *       the time CPU_init() is called.
 */
#define CPU_CFG_CACHE_MGMT_EN DEF_DISABLED

#endif
/** EOF */