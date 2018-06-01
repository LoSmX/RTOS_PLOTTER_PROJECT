/**
 * @file app_cfg.h
 *
 * @brief Application Configuration
 */

#ifndef  APP_CFG_MODULE_PRESENT
#define  APP_CFG_MODULE_PRESENT

/**************************************************** MODULE ENABLE / DISABLE */
/**
 * enable/disable semi hosted debug outputs
 *
 * @note: (1) Using the command-line debugger un-comment the GDB_ARGS in the
 *            Makefile below SEMI_HOSTED DEBUGGING
 *        (2) Start "telnet localhost 2333" before invoking "make debug"
 */

#define SEMI_HOSTING 1
/**
 * enable/disable SEGGER RTT JLINK functionality
 * see: https://www.segger.com/jlink-real-time-terminal.html
 *
 * @note: (1) Un-comment the GDB_ARGS in the Makefile below RTT_OPTION
 *        (2) Start "JLinkRTTClient" before invoking "make debug"
 *        Doesn't work using eclipse! :-(
 */
#define JLINK_RTT 0

/************************************************************ TASK PRIORITIES */
#define  APP_CFG_TASK_START_PRIO  		2u
#define  APP_CFG_TASK_COM_PRIO     		10u
#define  APP_CFG_TASK_ENDSTOPS_PRIO    	11u
#define  APP_CFG_TASK_SERVO_PRIO     	11u



/*********************************************************** TASK STACK SIZES */
#define  APP_CFG_TASK_START_STK_SIZE 	256u
#define  APP_CFG_TASK_COM_STK_SIZE 		256u
#define  APP_CFG_TASK_IO_STK_SIZE 		256u


/************************************************ TRACE / DEBUG CONFIGURATION */

#ifndef TRACE_LEVEL_OFF
#define TRACE_LEVEL_OFF 0
#endif

#ifndef TRACE_LEVEL_INFO
#define TRACE_LEVEL_INFO 1
#endif

#ifndef TRACE_LEVEL_DBG
#define TRACE_LEVEL_DBG 2
#endif

#ifndef SEMI_HOSTING_PRINTF
#define SEMI_HOSTING_PRINTF printf
#endif

#ifndef JLINK_RTT_PRINTF
#define JLINK_RTT_PRINTF SEGGER_RTT_printf
#endif

/* set the following define to enable trace messages off/info/debug */
#define APP_TRACE_LEVEL TRACE_LEVEL_DBG
#if SEMI_HOSTING
#define APP_TRACE SEMI_HOSTING_PRINTF
#endif

#if JLINK_RTT
#define APP_TRACE JLINK_RTT_PRINTF
#endif

#if SEMI_HOSTING
#define APP_TRACE_INFO(x) \
	((APP_TRACE_LEVEL >= TRACE_LEVEL_INFO) ? (void)(APP_TRACE(x)) : (void)0)
#define APP_TRACE_DBG(x) \
	((APP_TRACE_LEVEL >= TRACE_LEVEL_DBG) ? (void)(APP_TRACE(x)) : (void)0)
#endif

#if JLINK_RTT
#define APP_TRACE_INFO(x) \
	((APP_TRACE_LEVEL >= TRACE_LEVEL_INFO) ? (void)(APP_TRACE(0,x)) : (void)0)
#define APP_TRACE_DBG(x) \
	((APP_TRACE_LEVEL >= TRACE_LEVEL_DBG) ? (void)(APP_TRACE(0,x)) : (void)0)
#endif

#endif
/** EOF */
