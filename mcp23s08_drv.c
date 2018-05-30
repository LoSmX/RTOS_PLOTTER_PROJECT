/**
 * @file     mcp23s08_drv.c
 * @version  V0.1
 * @date     March 2017
 * @author   Roman Beneder
 *
 * @brief   MCP23S08 Driver Library
 *
 */

#include "mcp23s08_drv.h"
#include <app_cfg.h>
#include <mylib.h>
#include "../XMCLIB/inc/xmc_spi.h"
#include <bsp_gpio.h>





uint8_t mcp23s08_addr = 0x40;
uint8_t mcp23s08_nop = 0x00;

/*!
 *  @brief This function resets the Slave Select
 *  @param XMC_GPIO_PORT_t *const port, const uint8_t pin
 *  @return on success this function returns MCP23S08_OK (0) otherwise it check
 *  the given port on validity
 */
uint8_t _mcp23s08_reset_ss(XMC_GPIO_PORT_t *const port, const uint8_t pin)
{
  XMC_ASSERT("XMC_GPIO_Init: Invalid port", XMC_GPIO_CHECK_PORT(port));

  XMC_GPIO_SetOutputLow(port,pin);

  return MCP23S08_OK;
}

/*!
 *  @brief This function sets the Slave Select
 *  @param XMC_GPIO_PORT_t *const port, const uint8_t pin
 *  @return on success this function returns MCP23S08_OK (0) otherwise it check
 *  the given port on validity
 */
uint8_t _mcp23s08_set_ss(XMC_GPIO_PORT_t *const port, const uint8_t pin)
{
  XMC_ASSERT("XMC_GPIO_Init: Invalid port", XMC_GPIO_CHECK_PORT(port));

  XMC_GPIO_SetOutputHigh(port,pin);

  return MCP23S08_OK;
}

/*!
 *  @brief This function toggles the reset for the MCP23S08
 *  @param XMC_GPIO_PORT_t *const port, const uint8_t pin
 *  @return on success this function returns MCP23S08_OK (0)
 */
uint8_t _mcp23s08_reset(void)
{
  XMC_GPIO_SetOutputLow(MCP23S08_RESET);
  XMC_GPIO_SetOutputHigh(MCP23S08_RESET);

  return MCP23S08_OK;
}

/*!
 *  @brief This function reads or writes from or to the MCP23S08
 *  @param channel ... SPI channel
 *		   reg_name .. register address of the MCP23S08
 *		   data ...... content of the register
 *		   rd_wr ..... read/write
 *  @return on success this function returns MCP23S08_OK (0)
 */
uint8_t _mcp23s08_reg_xfer(XMC_USIC_CH_t *const channel, uint8_t reg_name, uint8_t data, uint8_t rd_wr)
{
  uint8_t recv = 0, mcp23s08_addr_rd = 0;

  XMC_ASSERT("XMC_USIC_CH_Enable: channel not valid", XMC_USIC_IsChannelValid(channel));

  if(rd_wr)
  {
	mcp23s08_addr_rd = mcp23s08_addr|MCP23S08_RD;

	_spi_transmit(channel,mcp23s08_addr_rd);
	_spi_receive(channel);

	_spi_transmit(channel,reg_name);
	_spi_receive(channel);

	_spi_transmit(channel,mcp23s08_nop);
	recv = _spi_receive(channel);

  }
  else
  {
	_spi_transmit(channel,mcp23s08_addr);
	_spi_receive(channel);

	_spi_transmit(channel,reg_name);
	_spi_receive(channel);

	_spi_transmit(channel,data);
	_spi_receive(channel);
  }

  return recv;
}

uint8_t _mcp23s08_step_posy(void){
	if(debounce(ENDSTOP1)){

		_mcp23s08_reset_ss(MCP23S08_SS);
		_mcp23s08_reg_xfer(XMC_SPI1_CH0,MCP23S08_GPIO,0x2,MCP23S08_WR);
		_mcp23s08_set_ss(MCP23S08_SS);

		//APP_TRACE_INFO ("PLOTTERSTEP HIGH...\n");

		_mcp23s08_reset_ss(MCP23S08_SS);
		_mcp23s08_reg_xfer(XMC_SPI1_CH0,MCP23S08_GPIO,0x00,MCP23S08_WR);
		_mcp23s08_set_ss(MCP23S08_SS);

		return 1;
	}else{
		return 0;
	}
}


uint8_t _mcp23s08_step_negy(void){
	if(debounce(ENDSTOP2)){
		_mcp23s08_reset_ss(MCP23S08_SS);
		_mcp23s08_reg_xfer(XMC_SPI1_CH0,MCP23S08_GPIO,0x3,MCP23S08_WR);
		_mcp23s08_set_ss(MCP23S08_SS);

		//APP_TRACE_INFO ("PLOTTERSTEP HIGH...\n");

		_mcp23s08_reset_ss(MCP23S08_SS);
		_mcp23s08_reg_xfer(XMC_SPI1_CH0,MCP23S08_GPIO,0x01,MCP23S08_WR);
		_mcp23s08_set_ss(MCP23S08_SS);

		return 1;
	}else{
		return 0;
	}
}


uint8_t _mcp23s08_step_posx(void){
	if(debounce(ENDSTOP4)){
		_mcp23s08_reset_ss(MCP23S08_SS);
		_mcp23s08_reg_xfer(XMC_SPI1_CH0,MCP23S08_GPIO,0x0C,MCP23S08_WR);
		_mcp23s08_set_ss(MCP23S08_SS);

		//APP_TRACE_INFO ("PLOTTERSTEP HIGH...\n");

		_mcp23s08_reset_ss(MCP23S08_SS);
		_mcp23s08_reg_xfer(XMC_SPI1_CH0,MCP23S08_GPIO,0x04,MCP23S08_WR);
		_mcp23s08_set_ss(MCP23S08_SS);

		return 1;
	}else{
		return 0;
	}
}

uint8_t _mcp23s08_step_negx(void){
	if(debounce(ENDSTOP3)){
		_mcp23s08_reset_ss(MCP23S08_SS);
		_mcp23s08_reg_xfer(XMC_SPI1_CH0,MCP23S08_GPIO,0x8,MCP23S08_WR);
		_mcp23s08_set_ss(MCP23S08_SS);

		//APP_TRACE_INFO ("PLOTTERSTEP HIGH...\n");

		_mcp23s08_reset_ss(MCP23S08_SS);
		_mcp23s08_reg_xfer(XMC_SPI1_CH0,MCP23S08_GPIO,0x00,MCP23S08_WR);
		_mcp23s08_set_ss(MCP23S08_SS);

		return 1;
	}else{
		return 0;
	}
}
