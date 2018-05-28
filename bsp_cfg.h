/**
 * @file bsp_cfg.h
 *
 * @brief Board Support Package
 */

#ifndef  BSP_CFG_H_
#define  BSP_CFG_H_

/************************************************************** MODULE ENABLE */

#define  BSP_CFG_CAN_EN      0    /* Enable 1, Disable 0 uC/CAN               */
#define  BSP_CFG_CAN_OPEN_EN 0    /* Enable 1, Disable 0 uC/CANopen           */
#define  BSP_CFG_FS_EN       0    /* Enable 1, Disable 0 uC/FS                */
#define  BSP_CFG_GUI_EN      0    /* Enable 1, Disable 0 uC/GUI               */
#define  BSP_CFG_MBM_EN      0    /* Enable 1, Disable 0 uC/Modbus-Master     */
#define  BSP_CFG_MBS_EN      0    /* Enable 1, Disable 0 uC/Modbus-Slave      */
#define  BSP_CFG_NET_EN      0    /* Enable 1, Disable 0 uC/TCP-IP            */
#define  BSP_CFG_OS2_EN      1    /* Enable 1, Disable 0 uC/OS-II             */
#define  BSP_CFG_OS3_EN      0    /* Enable 1, Disable 0 uC/OS-III            */
#define  BSP_CFG_USBD_EN     0    /* Enable 1, Disable 0 uC/USB-D             */
#define  BSP_CFG_USBH_EN     0    /* Enable 1, Disable 0 uC/USB-H             */


/*********************************************************** CLOCK MANAGEMENT */

#define  BSP_CFG_SYS_XTAL_OSC_FREQ_HZ   12000000u
#define  BSP_CFG_SYS_INT_OSC_FI_FREQ_HZ 24000000u


/************************************************************ BOARD SPECIFICS */

#endif
/** EOF */