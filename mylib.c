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


//_________________________________DEBOUNCE
_Bool debounce(int port,  int pin){
	CPU_INT08U i=10;
	CPU_INT08U time=0;
	CPU_INT08U f_press=0;
	CPU_INT08U press=0;
	OS_ERR      err;
	f_press=XMC_GPIO_GetInput((XMC_GPIO_PORT_t *const) port,(const uint8_t)pin);
	while(--i){
		press=XMC_GPIO_GetInput((XMC_GPIO_PORT_t *const) port,(const uint8_t)pin);
		if(f_press==press){
			time++;
		}else{
			return press;
		}
		//wait(100000);
		OSTimeDlyHMSM  (0,
			   			0,
  						0,
		 	    		1,
						OS_OPT_TIME_HMSM_STRICT ,
			       		&err);
  		if (err != OS_ERR_NONE)
  			return 3;
	}
	return f_press;
}
