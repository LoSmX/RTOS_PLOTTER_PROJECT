/*
 * @file bsp_uart.c
 *
 * @date: 02-2015
 * @author: Beneder Roman
 * @revision: 0.2, 02-2018, M. Horauer
 */

#include <bsp_uart.h>

XMC_UART_CH_CONFIG_t uart_config = {
	.data_bits = 8U,
	.stop_bits = 1U,
	.baudrate = 9600U
};

/**
 * @brief  Initialize UART1 CH1 - Tx=P0.1, Rx=P0.0, 9600-8N1
 * @return true on success, false otherwise
 */
_Bool BSP_UART_Init (void)
{

	XMC_UART_CH_Init (XMC_UART1_CH1, &uart_config);
	XMC_UART_CH_Init (XMC_UART1_CH0, &uart_config);
	XMC_UART_CH_SetInputSource (XMC_UART1_CH1, XMC_UART_CH_INPUT_RXD, 
	                            USIC1_C1_DX0_P0_0);

	XMC_UART_CH_SetInterruptNodePointer	(XMC_UART1_CH1, 0) ;
	XMC_UART_CH_EnableEvent (XMC_UART1_CH1, XMC_UART_CH_EVENT_STANDARD_RECEIVE);
	XMC_UART_CH_EnableEvent (XMC_UART1_CH1, 
	                         XMC_UART_CH_EVENT_ALTERNATIVE_RECEIVE);
	NVIC_EnableIRQ (USIC1_0_IRQn);

	XMC_UART_CH_Start (XMC_UART1_CH1);
	XMC_UART_CH_Start (XMC_UART1_CH0);

	XMC_GPIO_SetMode (UART_TX, XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT2);
	XMC_GPIO_SetMode (UART_RX, XMC_GPIO_MODE_INPUT_TRISTATE);

	return true;
}

/*! EOF */
