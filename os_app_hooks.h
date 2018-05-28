/**
 * @file os_app_hooks.h
 * @brief Application Hooks
 */
#ifndef  OS_APP_HOOKS_H
#define  OS_APP_HOOKS_H


#ifdef   OS_APP_HOOKS_H_GLOBALS
#define  OS_APP_HOOKS_H_EXT
#else
#define  OS_APP_HOOKS_H_EXT  extern
#endif

/******************************************************************* INCLUDES */
#include <os.h>

/******************************************************** FUNCTION PROTOTYPES */
void  App_OS_SetAllHooks (void);
void  App_OS_ClrAllHooks (void);

void  App_OS_TaskCreateHook (OS_TCB  *p_tcb);
void  App_OS_TaskDelHook (OS_TCB  *p_tcb);
void  App_OS_TaskReturnHook (OS_TCB  *p_tcb);

void  App_OS_IdleTaskHook (void);
void  App_OS_InitHook (void);
void  App_OS_StatTaskHook (void);
void  App_OS_TaskSwHook (void);
void  App_OS_TimeTickHook (void);

#endif
/** EOF */