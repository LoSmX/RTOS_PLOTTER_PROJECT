/*
 * bsp_gpio.c
 *
 *  Created on: 16.04.2018
 *      Author: Lo5mX
 */

#include <bsp_gpio.h>

XMC_GPIO_CONFIG_t servo_config = {
	.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT3,
	.output_level = XMC_GPIO_OUTPUT_LEVEL_LOW,
	.output_strength = XMC_GPIO_OUTPUT_STRENGTH_MEDIUM
};

/**
 * @brief  Initialize UART1 CH1 - Tx=P0.1, Rx=P0.0, 9600-8N1
 * @return true on success, false otherwise
 */
_Bool BSP_GPIO_Init (void)
{

	XMC_GPIO_Init(SERVO_CCU4, &servo_config);

	XMC_GPIO_SetMode(ENDSTOP1,XMC_GPIO_MODE_INPUT_PULL_DOWN);
	XMC_GPIO_SetMode(ENDSTOP2,XMC_GPIO_MODE_INPUT_PULL_DOWN);
	XMC_GPIO_SetMode(ENDSTOP3,XMC_GPIO_MODE_INPUT_PULL_DOWN);
	XMC_GPIO_SetMode(ENDSTOP4,XMC_GPIO_MODE_INPUT_PULL_DOWN);


	return true;
}


_Bool BSP_SERVO_HIGH (void)
{
	XMC_GPIO_SetOutputLow(SERVO_CCU4);

	return 0;
}

_Bool BSP_SERVO_LOW (void)
{
	XMC_GPIO_SetOutputHigh(SERVO_CCU4);
	return 0;
}
