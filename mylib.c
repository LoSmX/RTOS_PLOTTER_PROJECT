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
	CCU40_0_SetCapture(2);
}

void pen_down(void){
	CCU40_0_SetCapture(1);
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
