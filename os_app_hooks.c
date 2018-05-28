/**
 * @file os_app_hooks.c
 *
 * @brief APPLICATION HOOKS - functions that "dock into the OS" and execute
 *        application specific code, e.g., when a task is created or deleted,
 *        when a system tick occurs, when a context switch is done, etc.
 *
 */

#define  MICRIUM_SOURCE
#include "os.h"
#include <os_app_hooks.h>


/**
 * @function App_OS_SetAllHooks
 * @brief Set ALL application hooks.
 */
void  App_OS_SetAllHooks (void)
{
#if OS_CFG_APP_HOOKS_EN > 0u
	CPU_SR_ALLOC();


	CPU_CRITICAL_ENTER();
	OS_AppTaskCreateHookPtr = App_OS_TaskCreateHook;
	OS_AppTaskDelHookPtr    = App_OS_TaskDelHook;
	OS_AppTaskReturnHookPtr = App_OS_TaskReturnHook;

	OS_AppIdleTaskHookPtr   = App_OS_IdleTaskHook;
	OS_AppStatTaskHookPtr   = App_OS_StatTaskHook;
	OS_AppTaskSwHookPtr     = App_OS_TaskSwHook;
	OS_AppTimeTickHookPtr   = App_OS_TimeTickHook;
	CPU_CRITICAL_EXIT();
#endif
}

/**
 * @function App_OS_ClrAllHooks
 *
 * @brief Clear ALL application hooks.
 */
void  App_OS_ClrAllHooks (void)
{
#if OS_CFG_APP_HOOKS_EN > 0u
	CPU_SR_ALLOC();


	CPU_CRITICAL_ENTER();
	OS_AppTaskCreateHookPtr = (OS_APP_HOOK_TCB) 0;
	OS_AppTaskDelHookPtr    = (OS_APP_HOOK_TCB) 0;
	OS_AppTaskReturnHookPtr = (OS_APP_HOOK_TCB) 0;

	OS_AppIdleTaskHookPtr   = (OS_APP_HOOK_VOID) 0;
	OS_AppStatTaskHookPtr   = (OS_APP_HOOK_VOID) 0;
	OS_AppTaskSwHookPtr     = (OS_APP_HOOK_VOID) 0;
	OS_AppTimeTickHookPtr   = (OS_APP_HOOK_VOID) 0;
	CPU_CRITICAL_EXIT();
#endif
}

/**
 * @function App_OS_TaskCreateHook
 *
 * @param  p_tcb ... a pointer to the task control block of the task being
 *                   created
 *
 * @brief This function is called when a task is created; you can do something
 *        application specific here when a task is created.
 */
void  App_OS_TaskCreateHook (OS_TCB  *p_tcb)
{
	(void) &p_tcb;
}

/**
 * @function App_OS_TaskDelHook
 *
 * @param  p_tcb ... a pointer to the task control block of the task being
 *                       deleted
 *
 * @brief This function is called when a task is deleted. You can do something
 *        application specific when a task is deleted.
 */
void  App_OS_TaskDelHook (OS_TCB  *p_tcb)
{
	(void) &p_tcb;
}

/**
 * @function App_OS_TaskReturnHook
 *
 * @param  p_tcb ... a pointer to the task control block of the task that
 *                   returns
 *
 * @brief This function is called if a task accidentally returns. In other
 *        words, a task should either be an infinite loop or delete itself when
 *        done.
 */
void  App_OS_TaskReturnHook (OS_TCB  *p_tcb)
{
	(void) &p_tcb;
}

/**
 * @function App_OS_IdleTaskHook
 *
 * @brief This function is called by the idle task. This hook has been added to
 *        allow you to do such things as STOP the CPU to conserve power.
 *
 */
void  App_OS_IdleTaskHook (void)
{

}

/**
 * @function App_OS_InitHook
 *
 * @brief This function is called by OSInit() at the beginning of OSInit().
 *
 */
void  App_OS_InitHook (void)
{

}

/**
 * @function App_OS_StatTaskHook
 *
 * @brief This function is called every second by uC/OS-III's statistics task.
 *        This allows your application to add functionality to the statistics
 *        task.
 *
 */
void  App_OS_StatTaskHook (void)
{

}

/**
 * @function App_OS_TaskSwHook
 *
 * @brief This function is called when a task switch is performed.
 *        This allows you to perform other operations during a context switch.
 *
 * @note (1) Interrupts are disabled during this call.
 *       (2) It is assumed that the global pointer 'OSTCBHighRdyPtr' points to
 *           the TCB of the task that will be 'switched in' (i.e. the highest
 *           priority task) and, 'OSTCBCurPtr' points to the task being switched
 *           out (i.e. the preempted task).
 */
void  App_OS_TaskSwHook (void)
{

}

/**
 * @function App_OS_TimeTickHook
 *
 * @brief This function is called every tick.
 *
 * @note (1) IntThis function is assumed to be called from the Tick ISR.
 */
void  App_OS_TimeTickHook (void)
{

}
/** EOF */