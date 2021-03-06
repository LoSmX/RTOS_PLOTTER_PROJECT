/**
 * \file app.c
 *
 * \mainpage Application for UART communication tests
 *
 * Build: make debug OR make flash
 * Connect a TTL USB cable to UART1, launch a terminal program and initiate
 * communication, e.g. (without the quotes):
 *         PC -> uC: "#Hello$"
 *         uC -> PC: "XMC: Hello"
 *
 * @author Beneder Roman, Martin Horauer, UAS Technikum Wien
 * @revision 0.2
 * @date 02-2018
 */

/******************************************************************* INCLUDES */
#include <app_cfg.h>
#include <cpu_core.h>
#include <os.h>
#include <bsp.h>
#include <bsp_sys.h>
#include <bsp_int.h>
#include <bsp_gpio.h>
#include <bsp_ccu4.h>
#include <bsp_spi.h>
#include <mcp23s08_drv.h>

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <GPIO.h>

#include <lib_math.h>

#include <mylib.h>
#include "../XMCLIB/inc/xmc_uart.h"

#if SEMI_HOSTING
#include <debug_lib.h>
#endif

#if JLINK_RTT
#include <SEGGER_RTT.h>
#include <SEGGER_RTT_Conf.h>
#endif

/******************************************************************** DEFINES */
#define ACK  0x6
#define MAX_MSG_LENGTH 20
#define NUM_MSG        2

/********************************************************* FILE LOCAL GLOBALS */
static  CPU_STK  AppStartTaskStk[APP_CFG_TASK_START_STK_SIZE];            // <1>
static  OS_TCB   AppStartTaskTCB;

static  CPU_STK  AppTaskComStk[APP_CFG_TASK_COM_STK_SIZE];
static  CPU_STK  AppTaskEndstopsStk[APP_CFG_TASK_IO_STK_SIZE];
static  OS_TCB   AppTaskComTCB;
static  OS_TCB   AppTaskStepperTCB;

// Memory Block                                                           // <2>
OS_MEM      Mem_Partition;
OS_MEM      Mem_Partition1;
OS_MEM      Mem_Partition2;
CPU_CHAR    MyPartitionStorage[NUM_MSG - 1][MAX_MSG_LENGTH];
CPU_CHAR    MyPartitionStorage1[NUM_MSG - 1][MAX_MSG_LENGTH];
CPU_CHAR    MyPartitionStorage2[NUM_MSG - 1][MAX_MSG_LENGTH];
// Message Queue
OS_Q        UART_ISR;
OS_Q        Q_STEP_1;
OS_Q        Q_STEP_2;

extern int x_c;
extern int y_c;

/****************************************************** FILE LOCAL PROTOTYPES */
static  void AppTaskStart (void  *p_arg);
static  void AppTaskCreate (void);
static  void AppObjCreate (void);
static  void AppTaskCom (void  *p_arg);
void AppTaskStepper (void  *p_arg);
/************************************************************ FUNCTIONS/TASKS */

/*********************************************************************** MAIN */
/**
 * \function main
 * \params none
 * \returns 0 always
 *
 * \brief This is the standard entry point for C code.
 */
int main (void)
{
  OS_ERR  err;

  // Disable all interrupts                                               // <3>
  BSP_IntDisAll();
  // Enable Interrupt UART
  BSP_IntEn (BSP_INT_ID_USIC1_01); //**
  BSP_IntEn (BSP_INT_ID_USIC1_00); //**

// init SEMI Hosting DEBUG Support                                        // <4>
#if SEMI_HOSTING
  initRetargetSwo();
#endif

// init JLINK RTT DEBUG Support
#if JLINK_RTT
  SEGGER_RTT_ConfigDownBuffer (0, NULL, NULL, 0,
             SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);
  SEGGER_RTT_ConfigUpBuffer (0, NULL, NULL, 0,
           SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);
#endif

  // Init uC/OS-III
  OSInit (&err);                                                          // <5>
  if (err != OS_ERR_NONE)
    APP_TRACE_DBG ("Error OSInit: main\n");

  /* Create the start task */                                             // <6>
  OSTaskCreate ( (OS_TCB     *) &AppStartTaskTCB,
           (CPU_CHAR   *) "Startup Task",
           (OS_TASK_PTR) AppTaskStart,
           (void       *) 0,
           (OS_PRIO) APP_CFG_TASK_START_PRIO,
           (CPU_STK    *) &AppStartTaskStk[0],
           (CPU_STK_SIZE) APP_CFG_TASK_START_STK_SIZE / 10u,
           (CPU_STK_SIZE) APP_CFG_TASK_START_STK_SIZE,
           (OS_MSG_QTY) 0u,
           (OS_TICK) 0u,
           (void       *) 0,
           (OS_OPT) (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
           (OS_ERR     *) &err);

  // Start multitasking (i.e., give control to uC/OS-III)
  OSStart (&err);                                                         // <7>
  if (err != OS_ERR_NONE)
    APP_TRACE_DBG ("Error OSStart: main\n");

  while (1) {                                                             // <8>
    APP_TRACE_DBG ("Should never be output! Bug?\n");
  }
  return 0;
}

/*************************************************************** STARTUP TASK */
/**
 * \function AppTaskStart
 * \ params p_arg ... argument passed to AppTaskStart() by
 *                    OSTaskCreate()
 * \returns none
 *
 * \brief Startup (init) task that loads board support functions,
 *        initializes CPU services, the memory, the systick timer,
 *        etc. and finally invokes other application tasks.
 */
static void AppTaskStart (void *p_arg)
{
  CPU_INT32U  cpu_clk_freq;
  CPU_INT32U  cnts;
  OS_ERR      err;

  (void) p_arg;
  // initialize BSP functions
  BSP_Init();                                                             // <9>
  // initialize the uC/CPU services
  CPU_Init();
  // determine SysTick reference frequency
  cpu_clk_freq = BSP_SysClkFreqGet();
  // determine nbr SysTick increments
  cnts = cpu_clk_freq / (CPU_INT32U) OSCfg_TickRate_Hz;
  // init uCOS-III periodic time src (SysTick)
  OS_CPU_SysTickInit (cnts);
  // initialize memory management module
  Mem_Init();
  // initialize mathematical module
  Math_Init();
  //Ta
  pen_up();

  _mcp23s08_Plotter_Init();

  pen_down();

// compute CPU capacity with no task running
#if (OS_CFG_STAT_TASK_EN > 0u)                                           // <10>
  OSStatTaskCPUUsageInit (&err);
  if (err != OS_ERR_NONE)
    APP_TRACE_DBG ("Error OSStatTaskCPUUsageInit: AppTaskStart\n");
#endif

  APP_TRACE_INFO ("Creating Application Objects...\n");                  // <11>
  // create application objects
  AppObjCreate();

  APP_TRACE_INFO ("Creating Application Tasks...\n");                    // <12>
  // create application tasks
  AppTaskCreate();

  while (DEF_TRUE) {                                                     // <13>
    // Suspend current task
    OSTaskSuspend ( (OS_TCB *) 0, &err);
    if (err != OS_ERR_NONE)
      APP_TRACE_DBG ("Error OSTaskSuspend: AppTaskStart\n");
  }
}

/************************************************* Create Application Objects */
/**
 * \function AppObjCreate()
 * \brief Creates application objects.
 * \params none
 * \returns none
 */
static void AppObjCreate (void)
{
	OS_ERR      err;
	// Create Shared Memory
	  OSMemCreate ( (OS_MEM    *) &Mem_Partition,
	          (CPU_CHAR  *) "Mem Partition",
	          (void      *) &MyPartitionStorage[0][0],
	          (OS_MEM_QTY)  NUM_MSG,
	          (OS_MEM_SIZE) MAX_MSG_LENGTH * sizeof (CPU_CHAR),
	          (OS_ERR    *) &err);
	  if (err != OS_ERR_NONE)
	    APP_TRACE_DBG ("Error OSMemCreate: AppObjCreate\n");
	  // Create Shared Memory
	   OSMemCreate ( (OS_MEM    *) &Mem_Partition1,
	           (CPU_CHAR  *) "Mem Partition1",
	           (void      *) &MyPartitionStorage1[0][0],
	           (OS_MEM_QTY)  NUM_MSG,
	           (OS_MEM_SIZE) MAX_MSG_LENGTH * sizeof (CPU_CHAR),
	           (OS_ERR    *) &err);
	   if (err != OS_ERR_NONE)
	     APP_TRACE_DBG ("Error OSMemCreate: AppObjCreate\n");
	   // Create Shared Memory
	   OSMemCreate ( (OS_MEM    *) &Mem_Partition2,
	              (CPU_CHAR  *) "Mem Partition2",
	              (void      *) &MyPartitionStorage2[0][0],
	              (OS_MEM_QTY)  NUM_MSG,
	              (OS_MEM_SIZE) MAX_MSG_LENGTH * sizeof (CPU_CHAR),
	              (OS_ERR    *) &err);
	      if (err != OS_ERR_NONE)
	        APP_TRACE_DBG ("Error OSMemCreate: AppObjCreate\n");
	  // Create Message Queue
	  OSQCreate ( (OS_Q *)     &UART_ISR,
	        (CPU_CHAR *) "ISR Queue",
	        (OS_MSG_QTY) NUM_MSG,
	        (OS_ERR   *) &err);
	  if (err != OS_ERR_NONE)
	    APP_TRACE_DBG ("Error OSQCreate: AppObjCreate\n");

	  // Create Message Queue
	  OSQCreate ( (OS_Q *)     &Q_STEP_1,
	 	              (CPU_CHAR *) "STEP_X Queue",
	 	              (OS_MSG_QTY) NUM_MSG,
	 	              (OS_ERR   *) &err);
	  if (err != OS_ERR_NONE)
		  APP_TRACE_DBG ("Error OSQCreate: AppObjCreate\n");
	    // Create Message Queue
	  OSQCreate ( (OS_Q *)     &Q_STEP_2,
	              (CPU_CHAR *) "STEP_Y Queue",
	              (OS_MSG_QTY) NUM_MSG,
	              (OS_ERR   *) &err);
	  if (err != OS_ERR_NONE)
		  APP_TRACE_DBG ("Error OSQCreate: AppObjCreate\n");

}

/*************************************************** Create Application Tasks */
/**
 * \function AppTaskCreate()
 * \brief Creates one application task.
 * \params none
 * \returns none
 */
static void  AppTaskCreate (void)
{
  OS_ERR      err;

  // create AppTask_COM
  OSTaskCreate ( (OS_TCB     *) &AppTaskComTCB,
           (CPU_CHAR   *) "TaskCOM",
           (OS_TASK_PTR) AppTaskCom,
           (void       *) 0,
           (OS_PRIO) APP_CFG_TASK_COM_PRIO,
           (CPU_STK    *) &AppTaskComStk[0],
           (CPU_STK_SIZE) APP_CFG_TASK_COM_STK_SIZE / 10u,
           (CPU_STK_SIZE) APP_CFG_TASK_COM_STK_SIZE,
           (OS_MSG_QTY) 0u,
           (OS_TICK) 0u,
           (void       *) 0,
           (OS_OPT) (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
           (OS_ERR     *) &err);
  if (err != OS_ERR_NONE)
    APP_TRACE_DBG ("Error OSTaskCreate: AppTaskCreate\n");

  // create AppTask_IO
    OSTaskCreate ( (OS_TCB     *) &AppTaskStepperTCB,
             (CPU_CHAR   *) "TaskStepper",
             (OS_TASK_PTR) AppTaskStepper,
             (void       *) 0,
             (OS_PRIO) APP_CFG_TASK_ENDSTOPS_PRIO,
             (CPU_STK    *) &AppTaskEndstopsStk[0],
             (CPU_STK_SIZE) APP_CFG_TASK_COM_STK_SIZE / 10u,
             (CPU_STK_SIZE) APP_CFG_TASK_COM_STK_SIZE,
             (OS_MSG_QTY) 0u,
             (OS_TICK) 0u,
             (void       *) 0,
             (OS_OPT) (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
             (OS_ERR     *) &err);
    if (err != OS_ERR_NONE)
      APP_TRACE_DBG ("Error OSTaskCreate: AppTaskCreate\n");
}

/*********************************** Communication Application Task */
/**
 * \function AppTaskCom
 * \ params p_arg ... argument passed to AppTaskCom() by
 *                    AppTaskCreate()
 * \returns none
 *
 * \brief Task for Communication between UART_ISR (see BSP/bsp_int.c)
 *        and AppTaskCom. Communication from the ISR to the AppTaskCom is
 *        facilitated using a message queue.
 *
 *        Debug trace mesages are output to the SEGGER J-Link GDB Server.
 *
 *        (1) Debug or Flash the application.
 *        (2) Connect a TTL-USB UART cable: 
 *            GND (BLACK) - GND, TX (GREEN) - P0.0, RX (WHITE) - P0.1
 *        (3) Launch a terminal program and connect with 9600-8N1
 *            Enter strings like: #12345$, #abc$, etc.
 *            The XMC will respond with: XMC: 12345, XMC: abc, etc.
 */
static void AppTaskCom (void *p_arg)
{
  void        	*p_msg;
  OS_ERR      	err;
  OS_MSG_SIZE 	msg_size;
  CPU_TS     	ts;
  CPU_CHAR    	msg[MAX_MSG_LENGTH];
  CPU_CHAR    	x1[MAX_MSG_LENGTH];
  CPU_CHAR    	x2[MAX_MSG_LENGTH];
  CPU_CHAR  	y1[MAX_MSG_LENGTH];
  CPU_CHAR  	y2[MAX_MSG_LENGTH];
  char 			*msg_p=0;
  int 			x1_i;
  int 			y1_i;
  int 			x2_i;
  int 			y2_i;
  int 			i;

  (void) p_arg;

  APP_TRACE_INFO ("Entering AppTaskCom ...\n");

  while (DEF_TRUE) {
	  // empty the message buffer


	  APP_TRACE_INFO ("Pending for uart message ...\n");

	  // wait until a message is received

	  p_msg = OSQPend (&UART_ISR,
			  0,
			  OS_OPT_PEND_BLOCKING,
	          &msg_size,
	          &ts,
	          &err);
	  if (err != OS_ERR_NONE && err != OS_ERR_TIMEOUT)
		  APP_TRACE_DBG ("Error OSQPend: AppTaskCom\n");
	  // obtain message we received
	  APP_TRACE_INFO ("Got uart message ...\n");
	  memset (&msg, 0, MAX_MSG_LENGTH);
	  memcpy (msg, (CPU_CHAR*) p_msg, msg_size - 1);

	  // release the memory partition allocated in the UART service routine
	  OSMemPut (&Mem_Partition, p_msg, &err);
	  if (err != OS_ERR_NONE)
	 	  APP_TRACE_DBG ("Error OSMemPut: AppTaskCom\n");

	  // EXTRACT Coordinates
	  msg_p=&msg[0];
	  memset(&x1,0,MAX_MSG_LENGTH);
	  i=0;
	  while(*msg_p!= ':'){
		  x1[i]=*msg_p;
		  msg_p++;
		  i++;
	  }
	  while(*msg_p== ':'){
		  msg_p++;
	  }

	  memset(&y1,0,MAX_MSG_LENGTH);
	  i=0;
	  while(*msg_p != ':'){
		  y1[i]=*msg_p;
		  msg_p++;
		  i++;
	  }
	  while(*msg_p== ':'){
	  		  msg_p++;
	  }

	  memset(&x2,0,MAX_MSG_LENGTH);
	  i=0;
	  while(*msg_p!= ':'){
		  x2[i]=*msg_p;
		  msg_p++;
		  i++;
	  }
	  while(*msg_p== ':'){
		  msg_p++;
	  }

	  memset(&y2,0,MAX_MSG_LENGTH);
	  i=0;
	  while(*msg_p != '\0'){
		  y2[i]=*msg_p;
		  msg_p++;
		  i++;
	  }
	  x1_i=atoi(x1);
	  y1_i=atoi(y1);
	  x2_i=atoi(x2);
	  y2_i=atoi(y2);

	  x1_i= x1_i - x_c;
	  y1_i= y1_i - y_c;
	  if((x1_i != 0 && (x1_i>1 || x1_i<-1)) || (y1_i != 0 && (y1_i>1 || y1_i<-1))){
		  pen_up();
		  drawline( x1_i, y1_i);
		  pen_down();
	  }else{
		  OSTimeDlyHMSM  (0,
		  				0,
						0,
						100,
						OS_OPT_TIME_HMSM_STRICT ,
						&err);
	  }
	  x2_i= x2_i - x_c;
	  y2_i= y2_i - y_c;
	  drawline( x2_i, y2_i);

	  //send to STEP TASKS
	 /* p_msg = (CPU_CHAR *) OSMemGet (&Mem_Partition2, &err);
	  if (err != OS_ERR_NONE)
		  APP_TRACE_DBG ("Error OSMemGet1: AppTaskCom\n");
	  sprintf(p_msg,"%s\n",msg);
	  APP_TRACE_INFO (p_msg);
	  sprintf(p_msg,"%s\0",msg);

	  OSQPost ( 	(OS_Q      *) &Q_STEP_1,
	  			  (void      *) p_msg,
	  			  (OS_MSG_SIZE) MAX_MSG_LENGTH,
	  			  (OS_OPT)      OS_OPT_POST_FIFO,
	  	  	      (OS_ERR    *) &err);
	  if (err != OS_ERR_NONE)
	  	  APP_TRACE_DBG ("Error OSQPost1: AppTaskCom\n");


	  // Check if dones
	  OSQPend (&Q_STEP_2,
	  		  		0,
	  		  	    OS_OPT_PEND_BLOCKING,
	  		        &msg_size,
	  		 	    &ts,
	  		  		&err);
	  if (err != OS_ERR_NONE)
	  	 APP_TRACE_DBG ("Error OSQPend1: AppSTEPY\n");
	  OSMemPut (&Mem_Partition1, p_msg, &err);
	  */
	 // XMC_UART_CH_Transmit (XMC_UART1_CH1, ACK);
	XMC_UART_CH_Transmit (XMC_UART1_CH1, 'X');                           // <19>
	APP_TRACE_INFO ("Plot Line end\n");
  }
}
/***********************************AppTask_Io*/
void AppTaskStepper (void *p_arg)
{
	void    *q_msg;
	OS_MSG_SIZE msg_size;
	CPU_TS      ts=0;
	OS_ERR      err;
	CPU_CHAR    msg[MAX_MSG_LENGTH];
	int 	i;

	CPU_CHAR xsteps[MAX_MSG_LENGTH];
	CPU_CHAR ysteps[MAX_MSG_LENGTH];
	_Bool xdir= 0;
	_Bool ydir= 0;
	long double factor=0;
	long double ready=0;
	int times=0;
	int xtimes=0;
	int ytimes=0;
	char *msg_p=0;
	APP_TRACE_INFO ("Entering AppTaskStepperY ...\n");

	while(1){
	// Pending for message
		q_msg = OSQPend (&Q_STEP_1,
				0,
		        OS_OPT_PEND_BLOCKING,
		        &msg_size,
		        &ts,
				&err);
		if (err != OS_ERR_NONE)
		      APP_TRACE_DBG ("Error OSQPend1: AppSTEPY\n");
		// obtain message we received
		memset(&msg,0,MAX_MSG_LENGTH);
		memcpy (msg, (CPU_CHAR*) q_msg, msg_size - 1);
		APP_TRACE_INFO("GOT COORDINATES !!! \n");

		// release the memory partition
	    OSMemPut (&Mem_Partition2, q_msg, &err);                              // <18>
	    if (err != OS_ERR_NONE)
	    	APP_TRACE_DBG ("Error OSMemPut1: AppSTEPY\n");
	//DECODE
	    msg_p=&msg[0];
	    memset(&xsteps,0,MAX_MSG_LENGTH);
	    i=0;
	    while(*msg_p!= ':'){
	    	xsteps[i]=*msg_p;
	    	msg_p++;
	       	i++;
	   	}
 	    while(*msg_p== ':'){
 	    	msg_p++;
   	   	}

	    memset(&ysteps,0,MAX_MSG_LENGTH);
	    i=0;
	    while(*msg_p != '\0'){
	    	ysteps[i]=*msg_p;
	   	   	msg_p++;
	       	i++;
	    }
	// PLOTT setup
		xtimes = atoi(xsteps);
		ytimes = atoi(ysteps);
		sprintf(msg,"x: %d, y: %d\n",xtimes,ytimes);
		APP_TRACE_INFO(msg);
		if(xtimes<0){
			xdir=0;
			xtimes=0-xtimes;
		}else{
			xdir=1;
		}
		if(ytimes<0){
			ydir=0;
			ytimes=0-ytimes;
		}else{
			ydir=1;
		}
	//Plott
		if(xtimes==ytimes){ 		// angle of 45 deg
			times=xtimes;
			diagonal(times,xdir,ydir);
		}else if(xtimes==0){		//vertical line
			times=ytimes;
			while(times!=0){
				if(ydir){
					_mcp23s08_step_posy();
				}else{
					_mcp23s08_step_negy();
				}
				times--;
			}
		}else if(ytimes==0){		//horizontal line
			times=xtimes;
			while(times!=0){
				if(xdir){
					_mcp23s08_step_posx();
				}else{
					_mcp23s08_step_negx();
				}
				times--;
			}
		}else if(ytimes < xtimes){		//
			times=xtimes;
			factor=(double)ytimes/(double)xtimes;
			ready=0;
			while(times>0){
				while(ready < 1){
					if(xdir){
						_mcp23s08_step_posx();
					}else{
						_mcp23s08_step_negx();
					}
					ready= ready + factor;
					times--;
				}
				if(ydir){
					_mcp23s08_step_posy();
				}else{
					_mcp23s08_step_negy();
				}
				ready = ready-1;
			}
		}else if(xtimes < ytimes){		//too bee continued
			times=ytimes;
			factor=(double)xtimes/(double)ytimes;
			ready=0;
			while(times>0){
				while(ready < 1){
					if(ydir){
						_mcp23s08_step_posy();
					}else{
						_mcp23s08_step_negy();
					}
					ready= ready + factor;
					times--;
				}
				if(xdir){
					_mcp23s08_step_posx();
				}else{
					_mcp23s08_step_negx();
				}
				ready = ready-1;
			}
		}


	//DONE
		APP_TRACE_INFO("My current position is: \n");
		sprintf(msg,"x: %d , y: %d \n", x_c, y_c);
		APP_TRACE_INFO(msg);


	    q_msg = (CPU_CHAR *) OSMemGet (&Mem_Partition1, &err);
	    if (err != OS_ERR_NONE)
	    	APP_TRACE_DBG ("Error OSMemGet1: TaskSTePY\n");
	    OSQPost ( 	(OS_Q      *) &Q_STEP_2,
	         		(void      *) q_msg,
	    	       	(OS_MSG_SIZE) MAX_MSG_LENGTH,
	    	       	(OS_OPT)      OS_OPT_POST_FIFO,
	    	        (OS_ERR    *) &err);
	   	if (err != OS_ERR_NONE)
	   		APP_TRACE_DBG ("Error OSQPost1: TaskSTePY\n");
	}//Whileend
}
/************************************************************************ EOF */
/******************************************************************************/
