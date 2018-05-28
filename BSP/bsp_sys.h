/**
 * @file bsp_sys.h
 *
 * @brief Board Support Functions to initialize the CPU (PLL, Clock, etc.)
 */
#ifndef _BSP_SYS_H_
#define _BSP_SYS_H_

#include  <cpu.h>

/****************************************************************** REGISTERS */

/* ----------------- FLASH REGISTERS ------------------ */
#define  BSP_FLASH0_BASE_ADDR ((CPU_INT32U)(0x58001000u))
#define  BSP_REG_FLASH0_FCON  (*(CPU_REG32 *)(BSP_FLASH0_BASE_ADDR + 0x1014u))

/* -------------- WINDOWS WATCHDOG TIMER -------------- */
#define  BSP_WDT_BASE_ADDR    ((CPU_INT32U)(0x50008000u))
#define  BSP_REG_WDT_CTR      (*(CPU_REG32 *)(BSP_WDT_BASE_ADDR + 0x004u))

/* ----------------- PREFETCH MODULE ------------------ */
#define  BSP_PREF_BASE_ADDR   ((CPU_INT32U)(0x58004000u))
#define  BSP_REG_PREF_PCON    (*(CPU_REG32 *)(BSP_PREF_BASE_ADDR + 0x000u))

/* ------------ SYSTEM CONTROL UNIT MODULE ------------ */
#define  BSP_SCU_BASE_ADDR     ((CPU_INT32U)(0x50004000u))
#define  BSP_REG_SCU_TRAPCLR   (*(CPU_REG32 *)(BSP_SCU_BASE_ADDR + 0x16Cu))
#define  BSP_REG_SCU_SYSCLKCR  (*(CPU_REG32 *)(BSP_SCU_BASE_ADDR + 0x60Cu))
#define  BSP_REG_SCU_CPUCLKCR  (*(CPU_REG32 *)(BSP_SCU_BASE_ADDR + 0x610u))
#define  BSP_REG_SCU_PBCLKCR   (*(CPU_REG32 *)(BSP_SCU_BASE_ADDR + 0x614u))
#define  BSP_REG_SCU_CCUCLKCR  (*(CPU_REG32 *)(BSP_SCU_BASE_ADDR + 0x620u))
#define  BSP_REG_SCU_CLKSET    (*(CPU_REG32 *)(BSP_SCU_BASE_ADDR + 0x604u))
#define  BSP_REG_SCU_OSCHPCTRL (*(CPU_REG32 *)(BSP_SCU_BASE_ADDR + 0x704u))
#define  BSP_REG_SCU_PLLCON0   (*(CPU_REG32 *)(BSP_SCU_BASE_ADDR + 0x714u))
#define  BSP_REG_SCU_PLLCON1   (*(CPU_REG32 *)(BSP_SCU_BASE_ADDR + 0x718u))
#define  BSP_REG_SCU_PLLCON2   (*(CPU_REG32 *)(BSP_SCU_BASE_ADDR + 0x71Cu))
#define  BSP_REG_SCU_PLLSTAT   (*(CPU_REG32 *)(BSP_SCU_BASE_ADDR + 0x710u))

/************************************************************** REGISTER BITS */
#define  BSP_BIT_SYSCLKCR_OSC_FI              DEF_BIT_NONE
#define  BSP_BIT_SYSCLKCR_PLL_CLK             DEF_BIT_MASK(1u, 16u)
#define  BSP_BIT_SYSCLKCR_STANDBY_CLK         DEF_BIT_MASK(2u, 16u)
#define  BSP_BIT_SYSCLKCR_SYSSEL_MASK         DEF_BIT_FIELD(2u, 16u)
#define  BSP_BIT_SYSCLKCR_SYSDIV_MASK         DEF_BIT_FIELD(8u, 0u)

#define  BSP_BIT_PLLCON0_VCOBYP               DEF_BIT_00
#define  BSP_BIT_PLLCON0_VCOPWD               DEF_BIT_01
#define  BSP_BIT_PLLCON0_FINDIS               DEF_BIT_04
#define  BSP_BIT_PLLCON0_OSCDISCDIS           DEF_BIT_06
#define  BSP_BIT_PLLCON0_PLLPWD               DEF_BIT_16
#define  BSP_BIT_PLLCON0_OSCRES               DEF_BIT_17
#define  BSP_BIT_PLLCON0_RESLD                DEF_BIT_18

#define  BSP_BIT_PLLCON2_PINSEL               DEF_BIT_00
#define  BSP_BIT_PLLCON2_K1INSEL              DEF_BIT_08

#define  BSP_BIT_PLLSTAT_VCOLOCK              DEF_BIT_02
#define  BSP_BIT_PLLSTAT_PLLLV                DEF_BIT_07
#define  BSP_BIT_PLLSTAT_PLLHV                DEF_BIT_08
#define  BSP_BIT_PLLSTAT_PLLSP                DEF_BIT_09

#define  BSP_BIT_OSCHPCTRL_MODE_EXT_CRYS      0x0
#define  BSP_BIT_OSCHPCTRL_MODE_OSC_DIS       0x1
#define  BSP_BIT_OSCHPCTRL_MODE_PWR_SAVE      0x2
#define  BSP_BIT_OSCHPCTRL_MODE_MASK          DEF_BIT_FIELD(2u, 4u)
#define  BSP_BIT_OSCHPCTRL_OSCVAL             DEF_BIT_MASK(2u, 16u)
#define  BSP_BIT_OSCHPCTRL_OSCVAL_MASK        DEF_BIT_FIELD(5u, 16u)

#define  BSP_BIT_WDT_CTR_ENB                  DEF_BIT_00

#define  BSP_BIT_PREF_PCON_PBS                DEF_BIT_16

#define  BSP_BIT_CPUCLKCR_CPUDIV              DEF_BIT_00
#define  BSP_BIT_PBCLKCR_PBDIV                DEF_BIT_00
#define  BSP_BIT_CCUCLKCR_CCUDIV              DEF_BIT_00

#define  BSP_BIT_CLKSET_MMCEN                 DEF_BIT_01

/***************************************************************** PROTOTYPES */
void        BSP_LowLevelInit (void);
void        BSP_SysInit (void);
CPU_INT32U  BSP_SysClkFreqGet (void);

#endif

/*! EOF */
