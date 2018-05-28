/*
 * @file bsp_uart.h
 *
 * @date 02-2016
 * @author: Beneder Roman
 */

#ifndef SRC_BSP_BSP_UART_H_
#define SRC_BSP_BSP_UART_H_

#define UART_TX P0_1
#define UART_RX P0_0

#include <xmc_gpio.h>
#include <xmc_uart.h>
#include <stdio.h>

_Bool BSP_UART_Init (void) ;

#endif

/*! EOF */

