/**
 * @file cpu_bsp.c
 *
 * @brief CPU board support package
 */

/******************************************************************* INCLUDES */
#define CPU_BSP_MODULE
#include  <cpu_core.h>
#include  <bsp_sys.h>

/********************************************************* FILE LOCAL DEFINES */
#define  CPU_BSP_REG_DEMCR             (*(CPU_REG32 *)0xE000EDFC)
#define  CPU_BSP_REG_DWT_CR            (*(CPU_REG32 *)0xE0001000)
#define  CPU_BSP_REG_DWT_CYCCNT        (*(CPU_REG32 *)0xE0001004)

/**
 * @function CPU_TS_TmrInit()
 * @params none
 * @returns none
 *
 * @caller CPU_TS_Init()
 * @brief This function is an INTERNAL CPU module function & MUST be implemented
 *        by application/BSP function(s) but MUST NOT be called by application
 *        function(s).
 *
 * @note (1) CPU_TS_TmrInit() is an application/BSP function that MUST be
 *           defined by the developer if either of the following CPU features
 *           is enabled:
 *           (a) CPU timestamps
 *           (b) CPU interrupts disabled time measurements
 *           See also cpu_cfg.h
 *       (2) (a) Timer count values MUST be returned via word-size-configurable
 *               CPU_TS_TMR data type.
 *               - If timer has more bits, truncate timer values' higher-order
 *                 bits greater than the configured CPU_TS_TMR timestamp timer
 *                 data type word size.
 *               - Since the timer MUST NOT have less bits than the configured
 *                 CPU_TS_TMR timestamp timer data type word size;
 *                 CPU_CFG_TS_TMR_SIZE MUST be configured so that ALL bits in
 *                 CPU_TS_TMR data type are significant.
 *
 *                 In other words, if timer size is not a binary-multiple of
 *                 8-bit octets (e.g., 20-bits or even 24-bits), then the next
 *                 lower, binary-multiple octet word size SHOULD be configured
 *                 (e.g., to 16-bits). However, the minimum supported word size
 *                 for CPU timestamp timers is 8-bits.
 *
 *               See also cpu_cfg.h and cpu_core.h
 *
 *          (b) Timer SHOULD be an 'up'  counter whose values increase with each
 *              time count.
 *
 *          (c) When applicable, timer period SHOULD be less than the typical
 *              measured time but MUST be less than the maximum measured time;
 *              otherwise, timer resolution inadequate to measure desired times.
 *
 *              See also CPU_TS_TmrRd()
 */
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void  CPU_TS_TmrInit (void)
{
	CPU_INT32U  fclk_freq;

	fclk_freq = BSP_SysClkFreqGet();

	CPU_BSP_REG_DEMCR  |= DEF_BIT_24;
	CPU_BSP_REG_DWT_CR |= DEF_BIT_00;

	CPU_TS_TmrFreqSet ( (CPU_TS_TMR_FREQ) fclk_freq);
}
#endif

/**
 * @function CPU_TS_TmrRd()
 * @params none
 * @returns timestamp timer count
 *
 * @caller CPU_TS_Init(), CPU_TS_Get32(), CPU_TS_Get64(), CPU_IntDisMeasStart(),
 *         CPU_IntDisMeasStop().
 *
 * @brief This function is an INTERNAL CPU module function & MUST be implemented
 *         by application/BSP function(s) but SHOULD NOT be called by
 *         application function(s).
 *
 * (1) CPU_TS_TmrRd() is an application/BSP function that MUST be defined by the
 *     developer if either of the following CPU features is enabled :
 *
 *     (a) CPU timestamps
 *     (b) CPU interrupts disabled time measurements
 *
 *     See also cpu_cfg.h
 *
 * (2) (a) Timer count values MUST be returned via word-size-configurable
 *         CPU_TS_TMR data type.
 *
 *         (1) If timer has more bits, truncate timer values' higher-order bits
 *             greater than the configured 'CPU_TS_TMR' timestamp timer data
 *             type word size.
 *
 *         (2) Since the timer MUST NOT have less bits than the configured
 *             CPU_TS_TMR timestamp timer data type word size;
 *             CPU_CFG_TS_TMR_SIZE MUST be configured so that ALL bits in
 *             CPU_TS_TMR data type are significant.
 *
 *             In other words, if timer size is not a binary-multiple of 8-bit
 *             octets (e.g., 20-bits or even 24-bits), then the next lower,
 *             binary-multiple octet word size SHOULD be configured
 *             (e.g., to 16-bits). However, the minimum supported word size for
 *             CPU timestamp timers is 8-bits.
 *
 *         See also cpu_cfg.h & cpu_core.h
 *
 *     (b) Timer SHOULD be an 'up'  counter whose values increase with each time
 *         count.
 *
 *         (1) If timer is a 'down' counter whose values decrease with each time
 *             count, then the returned timer value MUST be ones-complemented.
 *
 *     (c) (1) When applicable, the amount of time measured by CPU timestamps is
 *             calculated by either of the following equations :
 *
 *             (A) Time measured  =  Number timer counts  *  Timer period
 *
 *                     where
 *
 *                         Number timer counts  Number of timer counts measured
 *                         Timer period ....... Timer's period in some units of
 *                                                  (fractional) seconds
 *                         Time measured ..... Amount of time measured, in same
 *                                             units of (fractional) seconds
 *                                             as the Timer period
 *
 *                                    Number timer counts
 *             (B) Time measured  =  ---------------------
 *                                      Timer frequency
 *
 *                     where
 *
 *                         Number timer counts  Number of timer counts measured
 *                         Timer frequency .... Timer's frequency in some units
 *                                                   of counts per second
 *                         Time measured ...... Amount of time measured, in sec
 *
 *         (2) Timer period SHOULD be less than the typical measured time but
 *             MUST be less than the maximum measured time; otherwise, timer
 *             resolution inadequate to measure desired times.
 *
 */
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_TS_TMR  CPU_TS_TmrRd (void)
{
	CPU_TS_TMR  ts_tmr_cnts;


	ts_tmr_cnts = (CPU_TS_TMR) CPU_BSP_REG_DWT_CYCCNT;

	return (ts_tmr_cnts);
}
#endif


/**
 * @function
 * @params ts_cnts ... CPU timestamp
 * @returns converted CPU timestamp in us
 *
 * @caller application
 * @brief This function is an (optional) CPU module application interface (API)
 *        function which MAY be implemented by application/BSP function(s) &
 *        MAY be called by application function(s).
 *
 * @note (1) CPU_TS32_to_uSec()/CPU_TS64_to_uSec() are application/BSP functions
 *           that MAY be optionally defined by the developer when either of the
 *           following CPU features is enabled:
 *           (a) CPU timestamps
 *           (b) CPU interrupts disabled time measurements
 *           See also cpu_cfg.h
 *
 * (2) (a) The amount of time measured by CPU timestamps is calculated by either
 *         ofthe following equations:
 *
 *                                            10^6 microseconds
 *  Time measured  =   Number timer counts * ------------------- * Timer period
 *                                                 1 second
 *
 *                     Number timer counts       10^6 microseconds
 *  Time measured  =  ---------------------  *  -------------------
 *                        Timer frequency             1 second
 *
 *                 where
 *
 *         (A) Number timer counts     Number of timer counts measured
 *         (B) Timer frequency         Timer's frequency in some units
 *                                         of counts per second
 *         (C) Timer period            Timer's period in some units of
 *                                         (fractional)  seconds
 *         (D) Time measured           Amount of time measured,
 *                                         in microseconds
 *
 *     (b) Timer period SHOULD be less than the typical measured time but MUST
 *         be less than the maximum measured time; otherwise, timer resolution
 *         inadequate to measure desired times.
 *
 *     (c) Specific implementations may convert any number of CPU_TS32 or
 *         CPU_TS64 bits -- up to 32 or 64, respectively -- into microseconds.
 */
#if (CPU_CFG_TS_32_EN == DEF_ENABLED)
CPU_INT64U  CPU_TS32_to_uSec (CPU_TS32  ts_cnts)
{
	CPU_INT64U  ts_us;
	CPU_INT64U  fclk_freq;


	fclk_freq = BSP_SysClkFreqGet();
	ts_us     = ts_cnts / (fclk_freq / DEF_TIME_NBR_uS_PER_SEC);

	return (ts_us);
}
#endif

#if (CPU_CFG_TS_64_EN == DEF_ENABLED)
CPU_INT64U  CPU_TS64_to_uSec (CPU_TS64  ts_cnts)
{
	CPU_INT64U  ts_us;
	CPU_INT64U  fclk_freq;


	fclk_freq = BSP_SysClkFreqGet();
	ts_us     = ts_cnts / (fclk_freq / DEF_TIME_NBR_uS_PER_SEC);

	return (ts_us);
}
#endif

/*! EOF */
