/**
 * @file bsp_int.h
 * @brief board support to manage interrupts
 */
#ifndef  BSP_INT_PRESENT
#define  BSP_INT_PRESENT

#ifdef   BSP_INT_MODULE
#define  BSP_INT_EXT
#else
#define  BSP_INT_EXT  extern
#endif

/******************************************************************** DEFINES */
/* INTERRUPT PRIORITY DEFINES */
#define  BSP_INT_PRIO_LEVEL_MASK DEF_BIT_MASK(7, 0)
#define  BSP_INT_PRIO_LEVEL_00   0u
#define  BSP_INT_PRIO_LEVEL_01   1u
#define  BSP_INT_PRIO_LEVEL_02   2u
#define  BSP_INT_PRIO_LEVEL_03   3u

/******************************************************************** DEFINES */
#define  BSP_INT_ID_SCU       0u
#define  BSP_INT_ID_ERU0_00   1u
#define  BSP_INT_ID_ERU0_01   2u
#define  BSP_INT_ID_ERU0_02   3u
#define  BSP_INT_ID_ERU0_03   4u
#define  BSP_INT_ID_ERU1_00   5u
#define  BSP_INT_ID_ERU1_01   6u
#define  BSP_INT_ID_ERU1_02   7u
#define  BSP_INT_ID_ERU1_03   8u

#define  BSP_INT_ID_RSVD_00   9u
#define  BSP_INT_ID_RSVD_01  10u
#define  BSP_INT_ID_RSVD_02  11u
#define  BSP_INT_ID_PMU0_00  12u
#define  BSP_INT_ID_RSVD_03  13u

#define  BSP_INT_ID_VADC0_C0_00  14u
#define  BSP_INT_ID_VADC0_C0_01  15u
#define  BSP_INT_ID_VADC0_C0_02  16u
#define  BSP_INT_ID_VADC0_C0_03  17u

#define  BSP_INT_ID_VADC0_G0_00  18u
#define  BSP_INT_ID_VADC0_G0_01  19u
#define  BSP_INT_ID_VADC0_G0_02  20u
#define  BSP_INT_ID_VADC0_G0_03  21u
#define  BSP_INT_ID_VADC0_G1_00  22u
#define  BSP_INT_ID_VADC0_G1_01  23u
#define  BSP_INT_ID_VADC0_G1_02  24u
#define  BSP_INT_ID_VADC0_G1_03  25u
#define  BSP_INT_ID_VADC0_G2_00  26u
#define  BSP_INT_ID_VADC0_G2_01  27u
#define  BSP_INT_ID_VADC0_G2_02  28u
#define  BSP_INT_ID_VADC0_G2_03  29u
#define  BSP_INT_ID_VADC0_G3_00  30u
#define  BSP_INT_ID_VADC0_G3_01  31u
#define  BSP_INT_ID_VADC0_G3_02  32u
#define  BSP_INT_ID_VADC0_G3_03  33u

#define  BSP_INT_ID_DSD0_00      34u
#define  BSP_INT_ID_DSD0_01      35u
#define  BSP_INT_ID_DSD0_02      36u
#define  BSP_INT_ID_DSD0_03      37u
#define  BSP_INT_ID_DSD0_04      38u
#define  BSP_INT_ID_DSD0_05      39u
#define  BSP_INT_ID_DSD0_06      40u
#define  BSP_INT_ID_DSD0_07      41u

#define  BSP_INT_ID_DAC0_00      42u
#define  BSP_INT_ID_DAC0_01      43u

#define  BSP_INT_ID_CCU40_00     44u
#define  BSP_INT_ID_CCU40_01     45u
#define  BSP_INT_ID_CCU40_02     46u
#define  BSP_INT_ID_CCU40_03     47u
#define  BSP_INT_ID_CCU41_00     48u
#define  BSP_INT_ID_CCU41_01     49u
#define  BSP_INT_ID_CCU41_02     50u
#define  BSP_INT_ID_CCU41_03     51u
#define  BSP_INT_ID_CCU42_00     52u
#define  BSP_INT_ID_CCU42_01     53u
#define  BSP_INT_ID_CCU42_02     54u
#define  BSP_INT_ID_CCU42_03     55u
#define  BSP_INT_ID_CCU43_00     56u
#define  BSP_INT_ID_CCU43_01     57u
#define  BSP_INT_ID_CCU43_02     58u
#define  BSP_INT_ID_CCU43_03     59u

#define  BSP_INT_ID_CCU80_00     60u
#define  BSP_INT_ID_CCU80_01     61u
#define  BSP_INT_ID_CCU80_02     62u
#define  BSP_INT_ID_CCU80_03     63u
#define  BSP_INT_ID_CCU81_00     64u
#define  BSP_INT_ID_CCU81_01     65u
#define  BSP_INT_ID_CCU81_02     66u
#define  BSP_INT_ID_CCU81_03     67u

#define  BSP_INT_ID_POSIF0_00    68u
#define  BSP_INT_ID_POSIF0_01    69u
#define  BSP_INT_ID_POSIF1_00    70u
#define  BSP_INT_ID_POSIF1_01    71u

#define  BSP_INT_ID_RSVD_04      72u
#define  BSP_INT_ID_RSVD_05      73u
#define  BSP_INT_ID_RSVD_06      74u
#define  BSP_INT_ID_RSVD_07      75u

#define  BSP_INT_ID_CAN0_00      76u
#define  BSP_INT_ID_CAN0_01      77u
#define  BSP_INT_ID_CAN0_02      78u
#define  BSP_INT_ID_CAN0_03      79u
#define  BSP_INT_ID_CAN0_04      80u
#define  BSP_INT_ID_CAN0_05      81u
#define  BSP_INT_ID_CAN0_06      82u
#define  BSP_INT_ID_CAN0_07      83u
#define  BSP_INT_ID_USIC0_00     84u
#define  BSP_INT_ID_USIC0_01     85u
#define  BSP_INT_ID_USIC0_02     86u
#define  BSP_INT_ID_USIC0_03     87u
#define  BSP_INT_ID_USIC0_04     88u
#define  BSP_INT_ID_USIC0_05     89u
#define  BSP_INT_ID_USIC1_00     90u
#define  BSP_INT_ID_USIC1_01     91u
#define  BSP_INT_ID_USIC1_02     92u
#define  BSP_INT_ID_USIC1_03     93u
#define  BSP_INT_ID_USIC1_04     94u
#define  BSP_INT_ID_USIC1_05     95u
#define  BSP_INT_ID_USIC2_00     96u
#define  BSP_INT_ID_USIC2_01     97u
#define  BSP_INT_ID_USIC2_02     98u
#define  BSP_INT_ID_USIC2_03     99u
#define  BSP_INT_ID_USIC2_04    100u
#define  BSP_INT_ID_USIC2_05    101u

#define  BSP_INT_ID_LEDTS0_00   102u
#define  BSP_INT_ID_RSVD_08     103u
#define  BSP_INT_ID_FCE0_00     104u
#define  BSP_INT_ID_GPDMA0_00   105u
#define  BSP_INT_ID_SDMMC0_00   106u
#define  BSP_INT_ID_USB0_00     107u
#define  BSP_INT_ID_ETH0_00     108u
#define  BSP_INT_ID_RSVD_09     109u
#define  BSP_INT_ID_GPDMA1_00   110u
#define  BSP_INT_ID_RSVD_10     111u

#define  BSP_INT_ID_MAX         112u

/***************************************************************** PROTOTYPES */
void  BSP_IntInit (void);
void  BSP_IntEn (CPU_DATA int_id);
void  BSP_IntDis (CPU_DATA int_id);
void  BSP_IntDisAll (void);
void  BSP_IntClr (CPU_DATA int_id);
void  BSP_IntVectSet (CPU_DATA int_id, CPU_FNCT_VOID isr);
void  BSP_IntPrioSet (CPU_DATA int_id, CPU_INT08U prio);

void BSP_IntHandlerSCU (void);
void BSP_IntHandlerERU0_00 (void);
void BSP_IntHandlerERU0_01 (void);
void BSP_IntHandlerERU0_02 (void);
void BSP_IntHandlerERU0_03 (void);
void BSP_IntHandlerERU1_00 (void);
void BSP_IntHandlerERU1_01 (void);
void BSP_IntHandlerERU1_02 (void);
void BSP_IntHandlerERU1_03 (void);
void BSP_IntHandlerPMU0_00 (void);
void BSP_IntHandlerVADC0_C0_00 (void);
void BSP_IntHandlerVADC0_C0_01 (void);
void BSP_IntHandlerVADC0_C0_02 (void);
void BSP_IntHandlerVADC0_C0_03 (void);
void BSP_IntHandlerVADC0_G0_00 (void);
void BSP_IntHandlerVADC0_G0_01 (void);
void BSP_IntHandlerVADC0_G0_02 (void);
void BSP_IntHandlerVADC0_G0_03 (void);
void BSP_IntHandlerVADC0_G1_00 (void);
void BSP_IntHandlerVADC0_G1_01 (void);
void BSP_IntHandlerVADC0_G1_02 (void);
void BSP_IntHandlerVADC0_G1_03 (void);
void BSP_IntHandlerVADC0_G2_00 (void);
void BSP_IntHandlerVADC0_G2_01 (void);
void BSP_IntHandlerVADC0_G2_02 (void);
void BSP_IntHandlerVADC0_G2_03 (void);
void BSP_IntHandlerVADC0_G3_00 (void);
void BSP_IntHandlerVADC0_G3_01 (void);
void BSP_IntHandlerVADC0_G3_02 (void);
void BSP_IntHandlerVADC0_G3_03 (void);
void BSP_IntHandlerDSD0_00 (void);
void BSP_IntHandlerDSD0_01 (void);
void BSP_IntHandlerDSD0_02 (void);
void BSP_IntHandlerDSD0_03 (void);
void BSP_IntHandlerDSD0_04 (void);
void BSP_IntHandlerDSD0_05 (void);
void BSP_IntHandlerDSD0_06 (void);
void BSP_IntHandlerDSD0_07 (void);
void BSP_IntHandlerDAC0_00 (void);
void BSP_IntHandlerDAC0_01 (void);
void BSP_IntHandlerCCU40_00 (void);
void BSP_IntHandlerCCU40_01 (void);
void BSP_IntHandlerCCU40_02 (void);
void BSP_IntHandlerCCU40_03 (void);
void BSP_IntHandlerCCU41_00 (void);
void BSP_IntHandlerCCU41_01 (void);
void BSP_IntHandlerCCU41_02 (void);
void BSP_IntHandlerCCU41_03 (void);
void BSP_IntHandlerCCU42_00 (void);
void BSP_IntHandlerCCU42_01 (void);
void BSP_IntHandlerCCU42_02 (void);
void BSP_IntHandlerCCU42_03 (void);
void BSP_IntHandlerCCU43_00 (void);
void BSP_IntHandlerCCU43_01 (void);
void BSP_IntHandlerCCU43_02 (void);
void BSP_IntHandlerCCU43_03 (void);
void BSP_IntHandlerCCU80_00 (void);
void BSP_IntHandlerCCU80_01 (void);
void BSP_IntHandlerCCU80_02 (void);
void BSP_IntHandlerCCU80_03 (void);
void BSP_IntHandlerCCU81_00 (void);
void BSP_IntHandlerCCU81_01 (void);
void BSP_IntHandlerCCU81_02 (void);
void BSP_IntHandlerCCU81_03 (void);
void BSP_IntHandlerPOSIF0_00 (void);
void BSP_IntHandlerPOSIF0_01 (void);
void BSP_IntHandlerPOSIF1_00 (void);
void BSP_IntHandlerPOSIF1_01 (void);
void BSP_IntHandlerCAN0_00 (void);
void BSP_IntHandlerCAN0_01 (void);
void BSP_IntHandlerCAN0_02 (void);
void BSP_IntHandlerCAN0_03 (void);
void BSP_IntHandlerCAN0_04 (void);
void BSP_IntHandlerCAN0_05 (void);
void BSP_IntHandlerCAN0_06 (void);
void BSP_IntHandlerCAN0_07 (void);
void BSP_IntHandlerUSIC0_00 (void);
void BSP_IntHandlerUSIC0_01 (void);
void BSP_IntHandlerUSIC0_02 (void);
void BSP_IntHandlerUSIC0_03 (void);
void BSP_IntHandlerUSIC0_04 (void);
void BSP_IntHandlerUSIC0_05 (void);
void BSP_IntHandlerUSIC1_00 (void);
void BSP_IntHandlerUSIC1_01 (void);
void BSP_IntHandlerUSIC1_02 (void);
void BSP_IntHandlerUSIC1_03 (void);
void BSP_IntHandlerUSIC1_04 (void);
void BSP_IntHandlerUSIC1_05 (void);
void BSP_IntHandlerUSIC2_00 (void);
void BSP_IntHandlerUSIC2_01 (void);
void BSP_IntHandlerUSIC2_02 (void);
void BSP_IntHandlerUSIC2_03 (void);
void BSP_IntHandlerUSIC2_04 (void);
void BSP_IntHandlerUSIC2_05 (void);
void BSP_IntHandlerLEDTS0_00 (void);
void BSP_IntHandlerFCE0_00 (void);
void BSP_IntHandlerGPDMA0_00 (void);
void BSP_IntHandlerSDMMC0_00 (void);
void BSP_IntHandlerUSB0_00 (void);
void BSP_IntHandlerETH0_00 (void);
void BSP_IntHandlerGPDMA1_00 (void);

#endif
/*! EOF */
