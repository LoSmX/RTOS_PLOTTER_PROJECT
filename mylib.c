/*
 * mylib.c
 *
 *  Created on: 14.03.2018
 *      Author: Lo5mX
 */
#include <cpu_core.h>
#include <os.h>
#include <mylib.h>
#include <bsp_gpio.h>
#include <bsp_ccu4.h>
#include <app_cfg.h>
#include "mcp23s08_drv.h"

#define MAX_MSG_LENGTH 20




//_________________________________DEBOUNCE
_Bool debounce(int port,  int pin){
	//CPU_INT08U i=2;
	CPU_INT08U f_press=0;
	//CPU_INT08U press=0;
	//OS_ERR      err;
	f_press=XMC_GPIO_GetInput((XMC_GPIO_PORT_t *const) port,(const uint8_t)pin);
	/*while(--i){
		press=XMC_GPIO_GetInput((XMC_GPIO_PORT_t *const) port,(const uint8_t)pin);
		if(f_press==press){

		}else{
			return press;
		}
		OSTimeDlyHMSM  (0,
			   			0,
  						0,
		 	    		1,
						OS_OPT_TIME_HMSM_STRICT ,
			       		&err);
  		if (err != OS_ERR_NONE)
  			return 3;
	}*/
	return f_press;
}

void pen_up(void){
	OS_ERR      err;

	CCU40_0_SetCapture(2);
	OSTimeDlyHMSM  (0,
				   	0,
					0,
	 	    		1,
					OS_OPT_TIME_HMSM_STRICT ,
				    &err);
}

void pen_down(void){
	OS_ERR      err;

	CCU40_0_SetCapture(1);
	OSTimeDlyHMSM  (0,
				   	0,
					0,
					1,
					OS_OPT_TIME_HMSM_STRICT ,
				    &err);
}

void diagonal(int times,_Bool xdir,_Bool ydir){
	while(times--){
		if(xdir){
			_mcp23s08_step_posx();
		}else{
			_mcp23s08_step_negx();
		}
		if(xdir){
			_mcp23s08_step_posy();
		}else{
			_mcp23s08_step_negy();
		}
	}
}

void drawline(int x, int y){
	void        *p_msg;
	OS_ERR      err;
	OS_MSG_SIZE msg_size;
	CPU_TS      ts;
	extern OS_MEM      Mem_Partition1;
	extern OS_MEM      Mem_Partition2;
	extern OS_Q        Q_STEP_1;
	extern OS_Q        Q_STEP_2;


	p_msg = (CPU_CHAR *) OSMemGet (&Mem_Partition2, &err);
	if (err != OS_ERR_NONE)
		APP_TRACE_DBG ("Error OSMemGet1: AppTaskCom\n");
	sprintf(p_msg,"Msg send to Step Task: %d:%d\n",x,y);
	APP_TRACE_INFO (p_msg);
	sprintf(p_msg,"%d:%d\0",x,y);

	OSQPost ( 	(OS_Q      *) &Q_STEP_1,
			(void      *) p_msg,
			(OS_MSG_SIZE) MAX_MSG_LENGTH,
			(OS_OPT)      OS_OPT_POST_FIFO,
			(OS_ERR    *) &err);
	if (err != OS_ERR_NONE)
		APP_TRACE_DBG ("Error OSQPost1: AppTaskCom\n");


	// Check if dones
	APP_TRACE_INFO ("Whait for endinf plott\n");

	OSQPend (&Q_STEP_2,
			0,
			OS_OPT_PEND_BLOCKING,
			&msg_size,
			&ts,
			&err);
	if (err != OS_ERR_NONE)
	  	 APP_TRACE_DBG ("Error OSQPend1: AppSTEPY\n");
	OSMemPut (&Mem_Partition1, p_msg, &err);
	if (err != OS_ERR_NONE)
		APP_TRACE_DBG ("Error OSQPut: drawline\n");

	APP_TRACE_INFO ("PLot ended!\n");
}
