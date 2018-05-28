/*
 * bsp_gpio.h
 *
 *  Created on: 16.04.2018
 *      Author: Lo5mX
 */

#ifndef APP_TASK2_BSP_BSP_GPIO_H_
#define APP_TASK2_BSP_BSP_GPIO_H_

#define SERVO_CCU4 P1_3
#define SERVO_CCU8 P0_3

#define ENDSTOP1 P1_15
#define ENDSTOP2 P1_13
#define ENDSTOP3 P1_14
#define ENDSTOP4 P1_12


#include <xmc_gpio.h>
#include <xmc_uart.h>
#include <stdio.h>

_Bool BSP_GPIO_Init (void) ;
_Bool BSP_SERVO_HIGH(void);
_Bool BSP_SERVO_LOW(void);
#endif /* APP_TASK2_BSP_BSP_GPIO_H_ */
