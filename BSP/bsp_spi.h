/*
 * bsp_spi.h
 *
 *  Created on: 30.05.2018
 *      Author: Lo5mX
 */

#ifndef APP_PROJECT_TEST_BSP_BSP_SPI_H_
#define APP_PROJECT_TEST_BSP_BSP_SPI_H_

#include "xmc_spi.h"
#include "xmc_gpio.h"
#include "errno.h"

#define SPI_MISO 	P0_4
#define SPI_MOSI 	P0_5
#define SPI_SCLK 	P0_11
#define MCP23S08_SS P1_2
#define MCP3004_SS  P1_4

#define SPI_OK 		0x00

uint8_t BSP_SPI_Init(void);
uint8_t _spi_transmit(XMC_USIC_CH_t *const channel, uint8_t spi_data);
uint8_t _spi_receive(XMC_USIC_CH_t *const channel);



#endif /* APP_PROJECT_TEST_BSP_BSP_SPI_H_ */
