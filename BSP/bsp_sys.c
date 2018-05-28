/**
 * @file bsp_sys.c
 *
 * @brief Board Support Functions to initialize the CPU (PLL, Clock, etc.)
 */

/******************************************************************* INCLUDES */
#include  <bsp_cfg.h>
#include  <lib_def.h>
#include  <bsp_sys.h>

/********************************************************* FILE LOCAL DEFINES */
#define  BSP_REG_VAL_TO                        0x7FFFF

#define  BSP_PLLCON1_K1DIV                     1
#define  BSP_PLLCON1_NDIV                     40
#define  BSP_PLLCON1_PDIV                      1
#define  BSP_PLLCON1_K2DIV_STEP1              20
#define  BSP_PLLCON1_K2DIV_STEP2               8
#define  BSP_PLLCON1_K2DIV_STEP3               5
#define  BSP_PLLCON1_K2DIV_FINAL               4
#define  BSP_PLLCON1_K2DIV_MASK               DEF_BIT_FIELD(7u, 16u)


/******************************************************* FILE LOCAL CONSTANTS */

/*********************************************************** FILE LOCAL TYPES */

/********************************************************* FILE LOCAL GLOBALS */

/****************************************************** FILE LOCAL PROTOTYPES */

/****************************************************************** FUNCTIONS */


/**
 * @function BSP_LowLevelInit()
 * @params none
 * @returns none
 *
 * @brief Board Support Package Low Level Initialization
 *
 * @note Problems with Branch prediction if accessing the flash via the cached
 *       memory interface. In this silicon(step A11), branch prediction has to
 *       be switched off by setting PCON.PBS = 1.
 */
void  BSP_LowLevelInit (void)
{
	CPU_INT32U  reg_val;

	/* Disable Watchdog timer */
	DEF_BIT_CLR (BSP_REG_WDT_CTR, BSP_BIT_WDT_CTR_ENB);
	DEF_BIT_SET (BSP_REG_PREF_PCON, BSP_BIT_PREF_PCON_PBS);

	reg_val = BSP_REG_FLASH0_FCON;
	reg_val = 0x3;
	BSP_REG_FLASH0_FCON = reg_val;

	BSP_SysInit();
}

/**
 * @function BSP_SysInit()
 * @params none
 * @returns none
 *
 * @brief This function should be called early in the BSP initialization process
 *
 * @note Once the Normal Mode is entered, then the intended PLL output target
 *       frequency can be configured by changing only the K2-Divider. This can
 *       result in multiple changes of the K2-Divider to avoid big frequency
 *       changes. Between the update of two K2-divider values 6 cycles of fpll
 *       should be waited.
 */
void  BSP_SysInit (void)
{
	CPU_INT32U  reg_to;
	CPU_INT32U  reg;

	/* Set VCO and PLL power saving mode to normal behavior */
	DEF_BIT_CLR (BSP_REG_SCU_PLLCON0, (BSP_BIT_PLLCON0_VCOPWD |
					   BSP_BIT_PLLCON0_PLLPWD));

	reg = BSP_REG_SCU_OSCHPCTRL;
	/* Set Ext. Crystal mod; osc power saving isn't entered */
	DEF_BIT_CLR (reg, (BSP_BIT_OSCHPCTRL_MODE_MASK  |
			   BSP_BIT_OSCHPCTRL_OSCVAL_MASK));
	/* Cfg. foscref = fosc / (OSCVAL + 1) */
	DEF_BIT_SET (reg, BSP_BIT_OSCHPCTRL_OSCVAL);
	BSP_REG_SCU_OSCHPCTRL = reg;

	/* Set ext. osc as PLL input */
	DEF_BIT_CLR (BSP_REG_SCU_PLLCON2, BSP_BIT_PLLCON2_PINSEL);
	/* Restart osc watchdog of the PLL */
	DEF_BIT_CLR (BSP_REG_SCU_PLLCON0, BSP_BIT_PLLCON0_OSCRES);
	/* Wait for stable OSC frequency */
	reg_to = BSP_REG_VAL_TO;
	while ( (DEF_BIT_IS_CLR (BSP_REG_SCU_PLLSTAT, (BSP_BIT_PLLSTAT_PLLLV |
						       BSP_BIT_PLLSTAT_PLLHV |
						       BSP_BIT_PLLSTAT_PLLSP))) &
		(reg_to > 0u)) {
		reg_to--;
	}
	if (reg_to == 0u) {
		while (DEF_TRUE);
	}
	/* ---------------- PLL CONFIGURATION ----------------- */
	/* Select OSC_FI as system clk */
	BSP_REG_SCU_SYSCLKCR = BSP_BIT_SYSCLKCR_OSC_FI;
	/* PLL is bypassed */
	DEF_BIT_SET (BSP_REG_SCU_PLLCON0, BSP_BIT_PLLCON0_VCOBYP);
	/* Disconnect OSC_HP from PLL */
	DEF_BIT_SET (BSP_REG_SCU_PLLCON0, BSP_BIT_PLLCON0_FINDIS);

	/* fpll = (NDIV  * fosc) / (PDIV * K2DIV))              */
	/*      = (40 * 12MHz) / (1 * 20)) = 24MHz              */
	BSP_REG_SCU_PLLCON1 = ( ( (BSP_PLLCON1_K1DIV       - 1u) <<  0u)
				| ( (BSP_PLLCON1_NDIV        - 1u) <<  8u)
				| ( (BSP_PLLCON1_K2DIV_STEP1 - 1u) << 16u)
				| ( (BSP_PLLCON1_PDIV        - 1u) << 24u));

	/* Keeps OSC clk conn. to VCO in case of Loss-of-lock */
	BSP_REG_SCU_PLLCON0 |= BSP_BIT_PLLCON0_OSCDISCDIS;
	/* Connect OSC_HP to PLL */
	DEF_BIT_CLR (BSP_REG_SCU_PLLCON0, BSP_BIT_PLLCON0_FINDIS);
	/* Restart PLL lock detection */
	DEF_BIT_SET (BSP_REG_SCU_PLLCON0, BSP_BIT_PLLCON0_RESLD);

	/* Wait for PLL Lock */
	reg_to = BSP_REG_VAL_TO;
	while ( (DEF_BIT_IS_CLR (BSP_REG_SCU_PLLSTAT, BSP_BIT_PLLSTAT_VCOLOCK)) &
		(reg_to > 0u)) {
		reg_to--;
	}
	if (reg_to == 0u) {
		while (DEF_TRUE);
	}

	/* Normal opertaion, PLL is not bypassed */
	DEF_BIT_CLR (BSP_REG_SCU_PLLCON0, BSP_BIT_PLLCON0_VCOBYP);
	/* Enable MMC clock */
	DEF_BIT_SET (BSP_REG_SCU_CLKSET, BSP_BIT_CLKSET_MMCEN);
	/* fcpu = fsys */
	DEF_BIT_CLR (BSP_REG_SCU_CPUCLKCR, BSP_BIT_CPUCLKCR_CPUDIV);
	/* fpb  = fsys */
	DEF_BIT_CLR (BSP_REG_SCU_PBCLKCR,  BSP_BIT_PBCLKCR_PBDIV);
	/* fccu = fsys */
	DEF_BIT_CLR (BSP_REG_SCU_CCUCLKCR, BSP_BIT_CCUCLKCR_CCUDIV);
	/* Select PLL clock as system clk */
	BSP_REG_SCU_SYSCLKCR |= BSP_BIT_SYSCLKCR_PLL_CLK;
	/* Wait between K2 divider updates */
	reg_to = BSP_REG_VAL_TO;
	while (reg_to > 0u) {
		reg_to--;
	}
	/* fpll = (NDIV  * fosc) / (PDIV * K2DIV)) */
	/*      = (40 * 12MHz) / (1 * 8)) = 60MHz  */
	reg  = (BSP_REG_SCU_PLLCON1 & ~BSP_PLLCON1_K2DIV_MASK);
	reg |= ( (BSP_PLLCON1_K2DIV_STEP2 - 1u) << 16);
	BSP_REG_SCU_PLLCON1 = reg;
	/* Wait between K2 divider updates */
	reg_to = BSP_REG_VAL_TO;
	while (reg_to > 0u) {
		reg_to--;
	}

	/* fpll = (NDIV  * fosc) / (PDIV * K2DIV)) */
	/*      = (40 * 12MHz) / (1 * 5)) = 96MHz  */
	reg                 = (BSP_REG_SCU_PLLCON1 & ~BSP_PLLCON1_K2DIV_MASK);
	reg                |= ( (BSP_PLLCON1_K2DIV_STEP3 - 1u) << 16);
	BSP_REG_SCU_PLLCON1 = reg;
	/* Wait between K2 divider updates */
	reg_to = BSP_REG_VAL_TO;
	while (reg_to > 0u) {
		reg_to--;
	}
	/* fpll = (NDIV  * fosc) / (PDIV * K2DIV)) */
	/*      = (40 * 12MHz) / (1 * 4)) = 120MHz */
	reg                 = (BSP_REG_SCU_PLLCON1 & ~BSP_PLLCON1_K2DIV_MASK);
	reg                |= ( (BSP_PLLCON1_K2DIV_FINAL - 1u) << 16);
	BSP_REG_SCU_PLLCON1 = reg;
}

/**
 * @function BSP_SysClkFreqGet()
 * @params none
 * @returns System clock frequency in cycles
 *
 * @brief This function is used to retrieve system or CPU clock frequency
 *
 */
CPU_INT32U BSP_SysClkFreqGet (void)
{
	CPU_INT08U  sysclk_src;
	CPU_INT16U  sysclk_div;
	CPU_INT08U  pll_n;
	CPU_INT08U  pll_p;
	CPU_INT08U  pll_k2;
	CPU_INT32U  cpu_freq;


	sysclk_src = (BSP_REG_SCU_SYSCLKCR & DEF_BIT_FIELD (2u, 16u)) >> 16u;
	sysclk_div = (BSP_REG_SCU_SYSCLKCR & DEF_BIT_FIELD (8u, 0u)) + 1u;

	switch (sysclk_src) {
	case 0:
		/* Fast Internal Oscillator selected as system clk */
		cpu_freq = BSP_CFG_SYS_INT_OSC_FI_FREQ_HZ;
		break;

	case 1:
		/* PLL clock is selected as system clk */
		cpu_freq = BSP_CFG_SYS_XTAL_OSC_FREQ_HZ;
		pll_n    = (BSP_REG_SCU_PLLCON1 >>  8u) + 1u;
		pll_p    = (BSP_REG_SCU_PLLCON1 >> 24u) + 1u;
		pll_k2   = (BSP_REG_SCU_PLLCON1 >> 16u) + 1u;
		cpu_freq = ( (cpu_freq / pll_p) * pll_n) / pll_k2;
		break;

	case 2:
	case 3:
	default:
		return ( (CPU_INT32U) 0u);
	}
	cpu_freq /= sysclk_div;
	if (DEF_BIT_IS_SET (BSP_REG_SCU_CPUCLKCR, BSP_BIT_CPUCLKCR_CPUDIV)) {
		cpu_freq /= 2u;
	}
	return (cpu_freq);
}

/*! EOF */
