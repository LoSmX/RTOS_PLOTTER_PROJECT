/**
 * @file bsp.h
 * @brief Main board support package for uCOS-III targeting the RelaxKit board.
 *
 * @author Martin Horauer, UAS Technikum Wien
 * @revision 0.1
 * @date 02-2015
 */

#ifndef  BSP_H_
#define  BSP_H_

#ifdef   BSP_MODULE
#define  BSP_EXT
#else
#define  BSP_EXT  extern
#endif


/******************************************************************* INCLUDES */
#include  <stdio.h>
#include  <stdarg.h>

#include  <cpu.h>
#include  <cpu_core.h>

#include  <lib_def.h>
#include  <lib_ascii.h>

/****************************************************************** CONSTANTS */

/*********************************************************** REGISTER DEFINES */

/***************************************************************** DATA TYPES */

/************************************************************* EXPORT GLOBALS */

/******************************************************************** MACRO'S */

/******************************************************** FUNCTION PROTOTYPES */
void  BSP_Init (void);

#endif
/*! EOF */
