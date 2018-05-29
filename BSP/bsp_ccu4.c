/*
 * bsp_ccu4.c
 *
 *  Created on: 24.04.2018
 *      Author: Lo5mX
 */
#include <bsp_ccu4.h>
#include <bsp_gpio.h>

_Bool BSP_CCU4_Init (void)
{
	XMC_CCU4_SLICE_COMPARE_CONFIG_t g_timer_object =
	{
	  .timer_mode 		     = XMC_CCU4_SLICE_TIMER_COUNT_MODE_EA,
	  .monoshot   		     = true,
	  .shadow_xfer_clear   = 0U,
	  .dither_timer_period = 0U,
	  .dither_duty_cycle   = 0U,
	  .prescaler_mode	   = XMC_CCU4_SLICE_PRESCALER_MODE_NORMAL,
	  .mcm_enable		   = 0U,
	  .prescaler_initval   = 8U,
	  .float_limit		   = 0U,
	  .dither_limit		   = 0U,
	  .passive_level 	   = XMC_CCU4_SLICE_OUTPUT_PASSIVE_LEVEL_LOW,
	  .timer_concatenation = 0U
	};

	/* CCU Slice Capture Initialization Data */
	/*XMC_CCU4_SLICE_CAPTURE_CONFIG_t g_capture_object =
	{
	  .fifo_enable 		   = false,
	  .timer_clear_mode    = XMC_CCU4_SLICE_TIMER_CLEAR_MODE_NEVER,
	  .same_event          = false,
	  .ignore_full_flag    = false,
	  .prescaler_mode	   = XMC_CCU4_SLICE_PRESCALER_MODE_NORMAL,
	  .prescaler_initval   = 0,
	  .float_limit		   = 0,
	  .timer_concatenation = false

	  XMC_GPIO_CONFIG_t SLICE0_OUTPUT_config =
	  {
		.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT4,
		.input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_STANDARD,
		.output_level = XMC_GPIO_OUTPUT_LEVEL_LOW,
		};
	};*/

	  /* Local variable which holds configuration of Event-1 */
	XMC_CCU4_SLICE_EVENT_CONFIG_t config;
	config.duration = XMC_CCU4_SLICE_EVENT_FILTER_DISABLED;
	config.edge     = XMC_CCU4_SLICE_EVENT_EDGE_SENSITIVITY_RISING_EDGE;
	config.level    = XMC_CCU4_SLICE_EVENT_LEVEL_SENSITIVITY_ACTIVE_HIGH; /* Not needed */
	config.mapped_input = XMC_CCU4_SLICE_INPUT_I;

	/* Ensure fCCU reaches CCU42 */
	XMC_CCU4_SetModuleClock(MODULE_PTR, XMC_CCU4_CLOCK_SCU);
	XMC_CCU4_Init(MODULE_PTR, XMC_CCU4_SLICE_MCMS_ACTION_TRANSFER_PR_CR);

	/* Get the slice out of idle mode */
	XMC_CCU4_EnableClock(MODULE_PTR, SLICE_NUMBER);
/* Start the prescaler and restore clocks to slices */
	XMC_CCU4_StartPrescaler(MODULE_PTR);

/* Initialize the Slice */
	XMC_CCU4_SLICE_CompareInit(SLICE_PTR, &g_timer_object);
/* Enable compare match and period match events */
	XMC_CCU4_SLICE_EnableEvent(SLICE_PTR, XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH);
	XMC_CCU4_SLICE_EnableEvent(SLICE_PTR, XMC_CCU4_SLICE_IRQ_ID_COMPARE_MATCH_UP);
/* Connect period match event to SR0 */
	XMC_CCU4_SLICE_SetInterruptNode(SLICE_PTR, XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH, XMC_CCU4_SLICE_SR_ID_0);

/* Connect compare match event to SR1 */
	XMC_CCU4_SLICE_SetInterruptNode(SLICE_PTR, XMC_CCU4_SLICE_IRQ_ID_COMPARE_MATCH_UP, XMC_CCU4_SLICE_SR_ID_1);

/* Configure NVIC */
	/* Set priority */
	NVIC_SetPriority(CCU40_0_IRQn, 10U);
	NVIC_SetPriority(CCU40_1_IRQn, 10U);
	//NVIC_SetPriority(CCU40_3_IRQn, 10U);
	/* Enable IRQ */
	NVIC_EnableIRQ(CCU40_0_IRQn);
	NVIC_EnableIRQ(CCU40_1_IRQn);
	/* Program a very large value into PR and CR */
	XMC_CCU4_SLICE_SetTimerPeriodMatch(SLICE_PTR, PERIODE); //65500U
	XMC_CCU4_SLICE_SetTimerCompareMatch(SLICE_PTR, CAPTURE1);//32000U

	/* Enable shadow transfer */
	XMC_CCU4_EnableShadowTransfer(MODULE_PTR, 							\
			(uint32_t)(XMC_CCU4_SHADOW_TRANSFER_SLICE_0|				\
			XMC_CCU4_SHADOW_TRANSFER_PRESCALER_SLICE_0));
	/* Configure Event-1 and map it to Input-I */
	XMC_CCU4_SLICE_ConfigureEvent(SLICE_PTR, XMC_CCU4_SLICE_EVENT_0, &config);
	/* Map Event-1 to Start function */
	XMC_CCU4_SLICE_StartConfig(SLICE_PTR, XMC_CCU4_SLICE_EVENT_0, XMC_CCU4_SLICE_START_MODE_TIMER_START_CLEAR);

	/* Generate an external start trigger */
	XMC_SCU_SetCcuTriggerHigh(CAPCOM_MASK);
	XMC_CCU4_EnableClock(MODULE_PTR, SLICE_NUMBER);
	XMC_CCU4_SLICE_StartTimer(SLICE_PTR);
	return true;
}

void CCU40_0_SetCapture(int dc){
	XMC_CCU4_SLICE_SetTimerCompareMatch(SLICE_PTR, (PERIODE/(dc*10)));
	XMC_CCU4_EnableShadowTransfer(MODULE_PTR,XMC_CCU4_SHADOW_TRANSFER_SLICE_0);
}
