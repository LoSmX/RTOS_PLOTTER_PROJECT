/**
 * @file os_cfg_app.h
 *
 * @brief Application specific configuration
 */
#ifndef OS_CFG_APP_H
#define OS_CFG_APP_H

/************************************************************** MISCELLANEOUS */
/* Maximum number of messages */
#define  OS_CFG_MSG_POOL_SIZE            100u

/* Stack size of ISR stack (number of CPU_STK elements) */
#define  OS_CFG_ISR_STK_SIZE             100u

/* Stack limit position in percentage to empty */
#define  OS_CFG_TASK_STK_LIMIT_PCT_EMPTY  10u


/****************************************************************** IDLE TASK */
/* Stack size (number of CPU_STK elements) */
#define  OS_CFG_IDLE_TASK_STK_SIZE       128u

/*********************************************************** ISR HANDLER TASK */
/* Size of ISR handler task queue */
#define  OS_CFG_INT_Q_SIZE                10u
/* Stack size (number of CPU_STK elements) */
#define  OS_CFG_INT_Q_TASK_STK_SIZE      100u

/************************************************************* STATISTIC TASK */
/* Priority */
#define  OS_CFG_STAT_TASK_PRIO            11u

/* Rate of execution (1 to 10 Hz) */
#define  OS_CFG_STAT_TASK_RATE_HZ         10u

/* Stack size (number of CPU_STK elements) */
#define  OS_CFG_STAT_TASK_STK_SIZE       100u


/********************************************************************** TICKS */
/* Tick rate in Hertz (10 to 1000 Hz) */
#define  OS_CFG_TICK_RATE_HZ            1000u

/* Priority */
#define  OS_CFG_TICK_TASK_PRIO             1u

/* Stack size (number of CPU_STK elements) */
#define  OS_CFG_TICK_TASK_STK_SIZE       100u

/********************************************************************* TIMERS */
/* Priority of 'Timer Task' */
#define  OS_CFG_TMR_TASK_PRIO             11u

/* Rate for timers (10 Hz Typ.) */
#define  OS_CFG_TMR_TASK_RATE_HZ          10u

/* Stack size (number of CPU_STK elements) */
#define  OS_CFG_TMR_TASK_STK_SIZE        100u

#endif
/** EOF */