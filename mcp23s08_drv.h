/**
 * @file     mcp23s08_drv.h
 * @version  V0.1
 * @date     March 2017
 * @author   Roman Beneder
 *
 * @brief   MCP23S08 Driver Library
 *
 */

#ifndef INC_MCP23S08_DRV_H_
#define INC_MCP23S08_DRV_H_

#include  <bsp_spi.h>
#include "errno.h"
#include <os.h>
#include <cpu_core.h>
#include "../XMCLIB/inc/xmc_gpio.h"
#include "../XMCLIB/inc/xmc_spi.h"



#define MCP23S08_OK			0x00
#define MCP23S08_RD 		0x01
#define MCP23S08_WR			0x00

#define MCP23S08_RESET 		P0_12

#define MCP23S08_IODIR 		0x00
#define MCP23S08_IPOL 		0x01
#define MCP23S08_GPINTEN 	0x02
#define MCP23S08_DEFVAL 	0x03
#define MCP23S08_INTCON 	0x04
#define MCP23S08_IOCON 		0x05
#define MCP23S08_GPPU 		0x06
#define MCP23S08_INTF 		0x07
#define MCP23S08_INTCAP 	0x08
#define MCP23S08_GPIO 		0x09
#define MCP23S08_OLAT 		0x0A

uint8_t _mcp23s08_reset_ss(XMC_GPIO_PORT_t *const port, const uint8_t pin);
uint8_t _mcp23s08_set_ss(XMC_GPIO_PORT_t *const port, const uint8_t pin);
uint8_t _mcp23s08_reset(void);
uint8_t _mcp23s08_reg_xfer(XMC_USIC_CH_t *const channel, uint8_t reg_name, uint8_t data, uint8_t rd_wr);
uint8_t _mcp23s08_step_posy(void);
uint8_t _mcp23s08_step_negx(void);
uint8_t _mcp23s08_step_posx(void);
uint8_t _mcp23s08_step_negy(void);
void 	_mcp23s08_goto_0_0(void);
void 	_mcp23s08_config_gpio(void);
void 	_mcp23s08_Plotter_Init(void);

#endif /* INC_MCP23S08_DRV_H_ */
