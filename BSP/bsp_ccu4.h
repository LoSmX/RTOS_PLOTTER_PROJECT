/*
 * bsp_ccu4.h
 *
 *  Created on: 24.04.2018
 *      Author: Lo5mX
 */

#ifndef APP_TEST_BSP_BSP_CCU4_H_
#define APP_TEST_BSP_BSP_CCU4_H_

#include <xmc_ccu4.h>
#include <stdio.h>
#include <stdbool.h>


#define SLICE_NUMBER      (1U)
#define SLICE_NUMBER2     (2U)
#define SLICE_PTR         CCU40_CC41
#define SLICE_PTR2        CCU40_CC42
#define MODULE_PTR        CCU40
#define MODULE_NUMBER     (0U)
#define CAPCOM_MASK       (SCU_GENERAL_CCUCON_GSC40_Msk) /**< Only CCU42 */
#define PERIODE		 	  100000U
#define CAPTURE1		  5000U

int percent;
int percent2;

_Bool BSP_CCU4_Init (void);
void CCU40_0_SetCapture(int dc);
void CCU40_1_SetCapture(int dc);
#endif /* APP_TEST_BSP_BSP_CCU4_H_ */
