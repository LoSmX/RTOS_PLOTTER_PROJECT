/**
 * @file bsp_int.c
 * @brief board support to manage interrupts
 */

/******************************************************************* INCLUDES */
#define  BSP_INT_MODULE

#include <cpu.h>
#include <os.h>
#include <bsp_int.h>
#include <stdio.h>
#include <stdint.h>
#include <xmc_uart.h>
#include <xmc_ccu4.h>
#include <bsp_ccu4.h>
#include <bsp_gpio.h>

#include <lib_def.h>
#include <debug_lib.h>

/********************************************************* FILE LOCAL DEFINES */

/******************************************************* FILE LOCAL CONSTANTS */

/*********************************************************** FILE LOCAL TYPES */

/********************************************************* FILE LOCAL GLOBALS */
static  CPU_FNCT_VOID  BSP_IntVectTbl[BSP_INT_ID_MAX];

/* defines for UART communication */
static uint8_t  Start_of_Packet = '#';                       /* in ASCII '#'  */
static uint8_t  End_of_Packet  = '$';                        /* in ASCII '$' */

/****************************************************** FILE LOCAL PROTOTYPES */
static  void  BSP_IntHandler (CPU_DATA  int_id);
static  void  BSP_IntHandler_Uart_Recive (void);
static  void CCU40_0_IRQHandler(void);
static  void CCU40_1_IRQHandler(void);

// Message Queues
extern OS_Q         UART_ISR;
// Memory Block
extern OS_MEM       Mem_Partition;

/****************************************************************** FUNCTIONS */
/**
 * \function BSP_IntClr()
 * \params   int_id ... interrupt to clear
 * \returns  none
 *
 * \note An interrupt does not need to be cleared within the interrupt
 *       controller.
 */
void  BSP_IntClr (CPU_DATA  int_id)
{

}

/**
 * \function BSP_IntDis()
 * \params   int_id ... interrupt to disable
 * \returns  none
 */
void  BSP_IntDis (CPU_DATA  int_id)
{
	if (int_id < BSP_INT_ID_MAX) {
		CPU_IntSrcDis (int_id + 16);
	}
}

/**
 * \function BSP_IntDisAll()
 * \params   none
 * \returns  none
 * \brief    disable all interrupts
 */
void  BSP_IntDisAll (void)
{
	CPU_IntDis();
}

/**
 * \function BSP_intEn()
 * \params   int_id ... interrupt to enable
 * \returns  none
 * \brief    enable interrupt
 */
void  BSP_IntEn (CPU_DATA  int_id)
{
	if (int_id < BSP_INT_ID_MAX) {
		CPU_IntSrcEn (int_id + 16);
	}
}

/**
 * \function BSP_IntVectSet()
 * \params   int_id ... interrupt for which vector will be set
 *           isr ...... handler to assign
 * \returns  none
 * \brief    assign ISR handler
 */
void  BSP_IntVectSet (CPU_DATA int_id, CPU_FNCT_VOID  isr)
{
	CPU_SR_ALLOC();

	if (int_id < BSP_INT_ID_MAX) {
		CPU_CRITICAL_ENTER();
		BSP_IntVectTbl[int_id] = isr;
		CPU_CRITICAL_EXIT();
	}
}

/**
 * \function BSP_IntPrioSet()
 * \params   int_id ... interrupt for which vector will be set
 *           prio ..... priority to assign
 * \returns  none
 * \brief    assign ISR priority
 */
void  BSP_IntPrioSet (CPU_DATA int_id, CPU_INT08U prio)
{
	CPU_SR_ALLOC();


	if (int_id < BSP_INT_ID_MAX) {
		CPU_CRITICAL_ENTER();
		CPU_IntSrcPrioSet (int_id + 16, prio);
		CPU_CRITICAL_EXIT();
	}
}

/**
 * \function BSP_IntInit()
 * \params   none
 * \returns  none
 * \brief    initialize interrupts
 */
void  BSP_IntInit (void)
{
	BSP_IntVectSet (USIC1_1_IRQn, BSP_IntHandler_Uart_Recive); //**
	BSP_IntVectSet (USIC1_0_IRQn, BSP_IntHandler_Uart_Recive); //**
	BSP_IntVectSet (CCU40_0_IRQn, CCU40_0_IRQHandler);
	BSP_IntVectSet (CCU40_1_IRQn, CCU40_1_IRQHandler);
}

static void CCU40_0_IRQHandler(void)
{
  XMC_CCU4_SLICE_ClearEvent(SLICE_PTR, XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH);
  //APP_TRACE_INFO ("sH ...\n");
  XMC_SCU_SetCcuTriggerLow(CAPCOM_MASK);
  XMC_SCU_SetCcuTriggerHigh(CAPCOM_MASK);
}

static void CCU40_1_IRQHandler(void){

	XMC_CCU4_SLICE_ClearEvent(SLICE_PTR,  XMC_CCU4_SLICE_IRQ_ID_COMPARE_MATCH_UP);
		//APP_TRACE_INFO ("sL ...\n");
}

/**
 * \function BSP_IntHandler_Uart_Recive()
 * \params   none
 * \returns  none
 * \brief    UART interrupt handler
 *           https://doc.micrium.com/display/osiiidoc/Keeping+the+Data+in+Scope
 */
static  void  BSP_IntHandler_Uart_Recive (void)
{
	OS_ERR             err;
	CPU_CHAR           RxData;
	static CPU_CHAR    *RxDataPtr  = NULL ;
	static CPU_INT32U  RxDataCtr = 0;
	static CPU_CHAR    msg_receiving = 0;

	// receive byte
	RxData = XMC_UART_CH_GetReceivedData (XMC_UART1_CH1);                   // <1>

	// is the character a "start of packet" character?                      // <2>
	if ( ( (int) RxData == Start_of_Packet) && msg_receiving == 0) {
		// allocate a memory block
		RxDataPtr = (CPU_CHAR *) OSMemGet (&Mem_Partition, &err);
		if (err != OS_ERR_NONE)
			APP_TRACE_DBG ("Error OSMemGet: BSP_IntHandler_Uart_Recive\n");
		else
			msg_receiving = 1;
	}

	// receive characters and put them into the memory partition            // <3>
	// until we detect and "end of packet character"
	// NOTE: the message itself may also contain a "start of packet character"
	//       but no "end of packet character"
	if ( ( (int) RxData != Start_of_Packet) && ( (int) RxData != End_of_Packet)
	     && msg_receiving == 1) {
		// put byte into the memory partition at the respective address
		*RxDataPtr = RxData;
		// increment the address to point to the next location
		RxDataPtr++;
		// increment the receive counter
		RxDataCtr++;
	}

	// is the character a "end of packet" character?                        // <4>
	if ( ( (int) RxData == End_of_Packet) && msg_receiving == 1) {
		// send the message to the application task - to that end put a pointer to
		// the memory block into the queue to the application task
		OSQPost ( (OS_Q      *) &UART_ISR,
			  (void      *) (RxDataPtr - RxDataCtr),
			  (OS_MSG_SIZE) RxDataCtr + 1,
			  (OS_OPT)      OS_OPT_POST_FIFO,
			  (OS_ERR    *) &err);
		if (err != OS_ERR_NONE)
			APP_TRACE_DBG ("Error OSQPost: BSP_IntHandler_Uart_Recive\n");

		// clear the receive pointer and counter                              // <5>
		RxDataPtr = NULL;
		RxDataCtr = 0;
		// ready for a next message
		msg_receiving = 0;
	}
}

/**
 * \function BSP_IntHandler####()
 * \params   none
 * \returns  none
 * \brief    handle an interrupt
 */
void BSP_IntHandlerSCU (void)
{
	BSP_IntHandler (BSP_INT_ID_SCU);
}
void BSP_IntHandlerERU0_00 (void)
{
	BSP_IntHandler (BSP_INT_ID_ERU0_00);
}
void BSP_IntHandlerERU0_01 (void)
{
	BSP_IntHandler (BSP_INT_ID_ERU0_01);
}
void BSP_IntHandlerERU0_02 (void)
{
	BSP_IntHandler (BSP_INT_ID_ERU0_02);
}
void BSP_IntHandlerERU0_03 (void)
{
	BSP_IntHandler (BSP_INT_ID_ERU0_03);
}
void BSP_IntHandlerERU1_00 (void)
{
	BSP_IntHandler (BSP_INT_ID_ERU1_00);
}
void BSP_IntHandlerERU1_01 (void)
{
	BSP_IntHandler (BSP_INT_ID_ERU1_01);
}
void BSP_IntHandlerERU1_02 (void)
{
	BSP_IntHandler (BSP_INT_ID_ERU1_02);
}
void BSP_IntHandlerERU1_03 (void)
{
	BSP_IntHandler (BSP_INT_ID_ERU1_03);
}
void BSP_IntHandlerPMU0_00 (void)
{
	BSP_IntHandler (BSP_INT_ID_PMU0_00);
}
void BSP_IntHandlerVADC0_C0_00 (void)
{
	BSP_IntHandler (BSP_INT_ID_VADC0_C0_00);
}
void BSP_IntHandlerVADC0_C0_01 (void)
{
	BSP_IntHandler (BSP_INT_ID_VADC0_C0_01);
}
void BSP_IntHandlerVADC0_C0_02 (void)
{
	BSP_IntHandler (BSP_INT_ID_VADC0_C0_02);
}
void BSP_IntHandlerVADC0_C0_03 (void)
{
	BSP_IntHandler (BSP_INT_ID_VADC0_C0_03);
}
void BSP_IntHandlerVADC0_G0_00 (void)
{
	BSP_IntHandler (BSP_INT_ID_VADC0_G0_00);
}
void BSP_IntHandlerVADC0_G0_01 (void)
{
	BSP_IntHandler (BSP_INT_ID_VADC0_G0_01);
}
void BSP_IntHandlerVADC0_G0_02 (void)
{
	BSP_IntHandler (BSP_INT_ID_VADC0_G0_02);
}
void BSP_IntHandlerVADC0_G0_03 (void)
{
	BSP_IntHandler (BSP_INT_ID_VADC0_G0_03);
}
void BSP_IntHandlerVADC0_G1_00 (void)
{
	BSP_IntHandler (BSP_INT_ID_VADC0_G1_00);
}
void BSP_IntHandlerVADC0_G1_01 (void)
{
	BSP_IntHandler (BSP_INT_ID_VADC0_G1_01);
}
void BSP_IntHandlerVADC0_G1_02 (void)
{
	BSP_IntHandler (BSP_INT_ID_VADC0_G1_02);
}
void BSP_IntHandlerVADC0_G1_03 (void)
{
	BSP_IntHandler (BSP_INT_ID_VADC0_G1_03);
}
void BSP_IntHandlerVADC0_G2_00 (void)
{
	BSP_IntHandler (BSP_INT_ID_VADC0_G2_00);
}
void BSP_IntHandlerVADC0_G2_01 (void)
{
	BSP_IntHandler (BSP_INT_ID_VADC0_G2_01);
}
void BSP_IntHandlerVADC0_G2_02 (void)
{
	BSP_IntHandler (BSP_INT_ID_VADC0_G2_02);
}
void BSP_IntHandlerVADC0_G2_03 (void)
{
	BSP_IntHandler (BSP_INT_ID_VADC0_G2_03);
}
void BSP_IntHandlerVADC0_G3_00 (void)
{
	BSP_IntHandler (BSP_INT_ID_VADC0_G3_00);
}
void BSP_IntHandlerVADC0_G3_01 (void)
{
	BSP_IntHandler (BSP_INT_ID_VADC0_G3_01);
}
void BSP_IntHandlerVADC0_G3_02 (void)
{
	BSP_IntHandler (BSP_INT_ID_VADC0_G3_02);
}
void BSP_IntHandlerVADC0_G3_03 (void)
{
	BSP_IntHandler (BSP_INT_ID_VADC0_G3_03);
}
void BSP_IntHandlerDSD0_00 (void)
{
	BSP_IntHandler (BSP_INT_ID_DSD0_00);
}
void BSP_IntHandlerDSD0_01 (void)
{
	BSP_IntHandler (BSP_INT_ID_DSD0_01);
}
void BSP_IntHandlerDSD0_02 (void)
{
	BSP_IntHandler (BSP_INT_ID_DSD0_02);
}
void BSP_IntHandlerDSD0_03 (void)
{
	BSP_IntHandler (BSP_INT_ID_DSD0_03);
}
void BSP_IntHandlerDSD0_04 (void)
{
	BSP_IntHandler (BSP_INT_ID_DSD0_04);
}
void BSP_IntHandlerDSD0_05 (void)
{
	BSP_IntHandler (BSP_INT_ID_DSD0_05);
}
void BSP_IntHandlerDSD0_06 (void)
{
	BSP_IntHandler (BSP_INT_ID_DSD0_06);
}
void BSP_IntHandlerDSD0_07 (void)
{
	BSP_IntHandler (BSP_INT_ID_DSD0_07);
}
void BSP_IntHandlerDAC0_00 (void)
{
	BSP_IntHandler (BSP_INT_ID_DAC0_00);
}
void BSP_IntHandlerDAC0_01 (void)
{
	BSP_IntHandler (BSP_INT_ID_DAC0_01);
}
void BSP_IntHandlerCCU40_00 (void)
{
	BSP_IntHandler (BSP_INT_ID_CCU40_00);
}
void BSP_IntHandlerCCU40_01 (void)
{
	BSP_IntHandler (BSP_INT_ID_CCU40_01);
}
void BSP_IntHandlerCCU40_02 (void)
{
	BSP_IntHandler (BSP_INT_ID_CCU40_02);
}
void BSP_IntHandlerCCU40_03 (void)
{
	BSP_IntHandler (BSP_INT_ID_CCU40_03);
}
void BSP_IntHandlerCCU41_00 (void)
{
	BSP_IntHandler (BSP_INT_ID_CCU41_00);
}
void BSP_IntHandlerCCU41_01 (void)
{
	BSP_IntHandler (BSP_INT_ID_CCU41_01);
}
void BSP_IntHandlerCCU41_02 (void)
{
	BSP_IntHandler (BSP_INT_ID_CCU41_02);
}
void BSP_IntHandlerCCU41_03 (void)
{
	BSP_IntHandler (BSP_INT_ID_CCU41_03);
}
void BSP_IntHandlerCCU42_00 (void)
{
	BSP_IntHandler (BSP_INT_ID_CCU42_00);
}
void BSP_IntHandlerCCU42_01 (void)
{
	BSP_IntHandler (BSP_INT_ID_CCU42_01);
}
void BSP_IntHandlerCCU42_02 (void)
{
	BSP_IntHandler (BSP_INT_ID_CCU42_02);
}
void BSP_IntHandlerCCU42_03 (void)
{
	BSP_IntHandler (BSP_INT_ID_CCU42_03);
}
void BSP_IntHandlerCCU43_00 (void)
{
	BSP_IntHandler (BSP_INT_ID_CCU43_00);
}
void BSP_IntHandlerCCU43_01 (void)
{
	BSP_IntHandler (BSP_INT_ID_CCU43_01);
}
void BSP_IntHandlerCCU43_02 (void)
{
	BSP_IntHandler (BSP_INT_ID_CCU43_02);
}
void BSP_IntHandlerCCU43_03 (void)
{
	BSP_IntHandler (BSP_INT_ID_CCU43_03);
}
void BSP_IntHandlerCCU80_00 (void)
{
	BSP_IntHandler (BSP_INT_ID_CCU80_00);
}
void BSP_IntHandlerCCU80_01 (void)
{
	BSP_IntHandler (BSP_INT_ID_CCU80_01);
}
void BSP_IntHandlerCCU80_02 (void)
{
	BSP_IntHandler (BSP_INT_ID_CCU80_02);
}
void BSP_IntHandlerCCU80_03 (void)
{
	BSP_IntHandler (BSP_INT_ID_CCU80_03);
}
void BSP_IntHandlerCCU81_00 (void)
{
	BSP_IntHandler (BSP_INT_ID_CCU81_00);
}
void BSP_IntHandlerCCU81_01 (void)
{
	BSP_IntHandler (BSP_INT_ID_CCU81_01);
}
void BSP_IntHandlerCCU81_02 (void)
{
	BSP_IntHandler (BSP_INT_ID_CCU81_02);
}
void BSP_IntHandlerCCU81_03 (void)
{
	BSP_IntHandler (BSP_INT_ID_CCU81_03);
}
void BSP_IntHandlerPOSIF0_00 (void)
{
	BSP_IntHandler (BSP_INT_ID_POSIF0_00);
}
void BSP_IntHandlerPOSIF0_01 (void)
{
	BSP_IntHandler (BSP_INT_ID_POSIF0_01);
}
void BSP_IntHandlerPOSIF1_00 (void)
{
	BSP_IntHandler (BSP_INT_ID_POSIF1_00);
}
void BSP_IntHandlerPOSIF1_01 (void)
{
	BSP_IntHandler (BSP_INT_ID_POSIF1_01);
}
void BSP_IntHandlerCAN0_00 (void)
{
	BSP_IntHandler (BSP_INT_ID_CAN0_00);
}
void BSP_IntHandlerCAN0_01 (void)
{
	BSP_IntHandler (BSP_INT_ID_CAN0_01);
}
void BSP_IntHandlerCAN0_02 (void)
{
	BSP_IntHandler (BSP_INT_ID_CAN0_02);
}
void BSP_IntHandlerCAN0_03 (void)
{
	BSP_IntHandler (BSP_INT_ID_CAN0_03);
}
void BSP_IntHandlerCAN0_04 (void)
{
	BSP_IntHandler (BSP_INT_ID_CAN0_04);
}
void BSP_IntHandlerCAN0_05 (void)
{
	BSP_IntHandler (BSP_INT_ID_CAN0_05);
}
void BSP_IntHandlerCAN0_06 (void)
{
	BSP_IntHandler (BSP_INT_ID_CAN0_06);
}
void BSP_IntHandlerCAN0_07 (void)
{
	BSP_IntHandler (BSP_INT_ID_CAN0_07);
}
void BSP_IntHandlerUSIC0_00 (void)
{
	BSP_IntHandler (BSP_INT_ID_USIC0_00);
}
void BSP_IntHandlerUSIC0_01 (void)
{
	BSP_IntHandler (BSP_INT_ID_USIC0_01);
}
void BSP_IntHandlerUSIC0_02 (void)
{
	BSP_IntHandler (BSP_INT_ID_USIC0_02);
}
void BSP_IntHandlerUSIC0_03 (void)
{
	BSP_IntHandler (BSP_INT_ID_USIC0_03);
}
void BSP_IntHandlerUSIC0_04 (void)
{
	BSP_IntHandler (BSP_INT_ID_USIC0_04);
}
void BSP_IntHandlerUSIC0_05 (void)
{
	BSP_IntHandler (BSP_INT_ID_USIC0_05);
}
void BSP_IntHandlerUSIC1_00 (void)
{
	BSP_IntHandler (BSP_INT_ID_USIC1_00);
}
void BSP_IntHandlerUSIC1_01 (void)
{
	BSP_IntHandler (BSP_INT_ID_USIC1_01);
}
void BSP_IntHandlerUSIC1_02 (void)
{
	BSP_IntHandler (BSP_INT_ID_USIC1_02);
}
void BSP_IntHandlerUSIC1_03 (void)
{
	BSP_IntHandler (BSP_INT_ID_USIC1_03);
}
void BSP_IntHandlerUSIC1_04 (void)
{
	BSP_IntHandler (BSP_INT_ID_USIC1_04);
}
void BSP_IntHandlerUSIC1_05 (void)
{
	BSP_IntHandler (BSP_INT_ID_USIC1_05);
}
void BSP_IntHandlerUSIC2_00 (void)
{
	BSP_IntHandler (BSP_INT_ID_USIC2_00);
}
void BSP_IntHandlerUSIC2_01 (void)
{
	BSP_IntHandler (BSP_INT_ID_USIC2_01);
}
void BSP_IntHandlerUSIC2_02 (void)
{
	BSP_IntHandler (BSP_INT_ID_USIC2_02);
}
void BSP_IntHandlerUSIC2_03 (void)
{
	BSP_IntHandler (BSP_INT_ID_USIC2_03);
}
void BSP_IntHandlerUSIC2_04 (void)
{
	BSP_IntHandler (BSP_INT_ID_USIC2_04);
}
void BSP_IntHandlerUSIC2_05 (void)
{
	BSP_IntHandler (BSP_INT_ID_USIC2_05);
}
void BSP_IntHandlerLEDTS0_00 (void)
{
	BSP_IntHandler (BSP_INT_ID_LEDTS0_00);
}
void BSP_IntHandlerFCE0_00 (void)
{
	BSP_IntHandler (BSP_INT_ID_FCE0_00);
}
void BSP_IntHandlerGPDMA0_00 (void)
{
	BSP_IntHandler (BSP_INT_ID_GPDMA0_00);
}
void BSP_IntHandlerSDMMC0_00 (void)
{
	BSP_IntHandler (BSP_INT_ID_SDMMC0_00);
}
void BSP_IntHandlerUSB0_00 (void)
{
	BSP_IntHandler (BSP_INT_ID_USB0_00);
}
void BSP_IntHandlerETH0_00 (void)
{
	BSP_IntHandler (BSP_INT_ID_ETH0_00);
}
void BSP_IntHandlerGPDMA1_00 (void)
{
	BSP_IntHandler (BSP_INT_ID_GPDMA1_00);
}

/**
 * \function BSP_IntHandler####()
 * \params   int_id interrupt that will be handled
 * \returns  none
 * \brief    central interrupt handler
 */
static  void  BSP_IntHandler (CPU_DATA  int_id)
{
	CPU_FNCT_VOID  isr;
	CPU_SR_ALLOC();

	/* tell the OS that we are starting an ISR */
	CPU_CRITICAL_ENTER();
	OSIntEnter();
	CPU_CRITICAL_EXIT();

	if (int_id < BSP_INT_ID_MAX) {
		isr = BSP_IntVectTbl[int_id];
		if (isr != (CPU_FNCT_VOID) 0) {
			isr();
		}
	}
	/* tell the OS that we are leaving an ISR */
	OSIntExit();
}

/*! EOF */
