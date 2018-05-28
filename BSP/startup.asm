@*
@********************************************************************************************************
@                                    EXCEPTION VECTORS & STARTUP CODE
@
@ File      : cstartup.s
@ For       : ARMv7M Cortex-M4
@ Mode      : Thumb2
@ Toolchain : Atollic TrueSTUDIO(R)
@********************************************************************************************************
@


@********************************************************************************************************
@                                      CODE GENERATION DIRECTIVES
@********************************************************************************************************
    .syntax unified
    .cpu cortex-m4
    .fpu softvfp
    .thumb


@********************************************************************************************************
@                                           PUBLIC FUNCTIONS
@********************************************************************************************************

    .global  g_pfnVectors
    .global  Default_Handler


@********************************************************************************************************
@                                       LINKER SCRIPT DEFINITIONS
@********************************************************************************************************

    .word  _sidata                          @ start addr for the init values of the .data section.
    .word  _sdata                           @ start addr for the .data section.
    .word  _edata                           @ end address for the .data section.
    .word  _sbss                            @ start address for the .bss section.
    .word  _ebss                            @ end address for the .bss section.


@********************************************************************************************************
@                                               EQUATES
@********************************************************************************************************

.equ  PREF_PCON,      0x58004000
.equ  SCU_GCU_PEEN,   0x5000413C
.equ  SCU_GCU_PEFLAG, 0x50004150


@PAGE
@********************************************************************************************************
@                                      void Reset_Handler (void)
@
@ Note(s) : 1) This code gets called after a reset event.
@
@           2) Reset_Handler() MUST:
@              a) Copy .data section from ROM to RAM.
@              b) Clear .bss section (Zero init).
@              c) Call low level initialzation routine.
@              d) Run static constructors.
@              e) Enter main.
@              f) Loop forever if returning from main.
@********************************************************************************************************

    .section  .text.Reset_Handler
    .weak  Reset_Handler
    .type  Reset_Handler, %function
Reset_Handler:
                                            @ Disable Branch prediction.
    ldr r0,=PREF_PCON
    ldr r1,[r0]
    orr r1,r1,#0x00010000
    str r1,[r0]

                                            @ Clear existing parity errors if any.
    ldr r0,=SCU_GCU_PEFLAG
    ldr r1,=0xFFFFFFFF
    str r1,[r0]

                                            @ Disable parity.
    ldr r0,=SCU_GCU_PEEN
    mov r1,#0
    str r1,[R0]

#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
                                            @ Enable Floating Point Support at reset for FPU
    LDR.W   R0, =0xE000ED88                 @ Load address of CPACR register
    LDR     R1, [R0]                        @ Read value at CPACR
    ORR     R1,  R1, #(0xF <<20)            @ Set bits 20-23 to enable CP10 and CP11 coprocessors
                                            @ Write back the modified CPACR value
    STR     R1, [R0]                        @ Wait for store to complete
    DSB

                                            @ Disable automatic FP register content
                                            @ Disable lazy context switch
    LDR.W   R0, =0xE000EF34                 @ Load address to FPCCR register
    LDR     R1, [R0]
    AND     R1,  R1, #(0x3FFFFFFF)          @ Clear the LSPEN and ASPEN bits
    STR     R1, [R0]
    ISB                                     @ Reset pipeline now the FPU is enabled
#endif

    LDR     R1, =0xE000ED14                 @ Load address to CCR register
    LDR.W   R0, [R1, #0x0]
    BIC     R0,  R0, #0x8                   @ Enable unaligned memory access
    STR.W   R0, [R1, #0x0]

    ldr sp, =_estack                        @ set stack pointer.

                                            @ a) copy .data section (Copy from ROM to RAM).
    movs r1, #0
    b LoopCopyDataInit

CopyDataInit:
    ldr r3, =_sidata
    ldr r3, [r3, r1]
    str r3, [r0, r1]
    adds r1, r1, #4

LoopCopyDataInit:
    ldr r0, =_sdata
    ldr r3, =_edata
    adds r2, r0, r1
    cmp r2, r3
    bcc CopyDataInit
    ldr r2, =_sbss
    b LoopFillZerobss

                                            @ b) Clear .bss section (Zero init).
FillZerobss:
    movs r3, #0
    str r3, [r2], #4

LoopFillZerobss:
    ldr r3, = _ebss
    cmp r2, r3
    bcc FillZerobss

    bl BSP_LowLevelInit                     @ c) Call system initialzation routine.

    bl __libc_init_array                    @ d) Run static constructors.

    bl main                                 @ e) Enter main.

                                            @ f) Loop forever if returning from main.
LoopForever:
    b LoopForever


.size	Reset_Handler, .-Reset_Handler


@********************************************************************************************************
@                                      EXCEPTION & INTERRUPT VECTORS
@********************************************************************************************************

    .section  .isr_vector,"a",%progbits
    .type  g_pfnVectors, %object
    .size  g_pfnVectors, .-g_pfnVectors

    .extern  BSP_IntHandlerSCU
    .extern  BSP_IntHandlerERU0_00
    .extern  BSP_IntHandlerERU0_01
    .extern  BSP_IntHandlerERU0_02
    .extern  BSP_IntHandlerERU0_03
    .extern  BSP_IntHandlerERU1_00
    .extern  BSP_IntHandlerERU1_01
    .extern  BSP_IntHandlerERU1_02
    .extern  BSP_IntHandlerERU1_03
    .extern  BSP_IntHandlerWDT_00
    .extern  BSP_IntHandlerPMU0_00
    .extern  BSP_IntHandlerPMU0_01
    .extern  BSP_IntHandlerVADC0_C0_00
    .extern  BSP_IntHandlerVADC0_C0_01
    .extern  BSP_IntHandlerVADC0_C0_02
    .extern  BSP_IntHandlerVADC0_C0_03
    .extern  BSP_IntHandlerVADC0_G0_00
    .extern  BSP_IntHandlerVADC0_G0_01
    .extern  BSP_IntHandlerVADC0_G0_02
    .extern  BSP_IntHandlerVADC0_G0_03
    .extern  BSP_IntHandlerVADC0_G1_00
    .extern  BSP_IntHandlerVADC0_G1_01
    .extern  BSP_IntHandlerVADC0_G1_02
    .extern  BSP_IntHandlerVADC0_G1_03
    .extern  BSP_IntHandlerVADC0_G2_00
    .extern  BSP_IntHandlerVADC0_G2_01
    .extern  BSP_IntHandlerVADC0_G2_02
    .extern  BSP_IntHandlerVADC0_G2_03
    .extern  BSP_IntHandlerVADC0_G3_00
    .extern  BSP_IntHandlerVADC0_G3_01
    .extern  BSP_IntHandlerVADC0_G3_02
    .extern  BSP_IntHandlerVADC0_G3_03
    .extern  BSP_IntHandlerDSD0_00
    .extern  BSP_IntHandlerDSD0_01
    .extern  BSP_IntHandlerDSD0_02
    .extern  BSP_IntHandlerDSD0_03
    .extern  BSP_IntHandlerDSD0_04
    .extern  BSP_IntHandlerDSD0_05
    .extern  BSP_IntHandlerDSD0_06
    .extern  BSP_IntHandlerDSD0_07
    .extern  BSP_IntHandlerDAC0_00
    .extern  BSP_IntHandlerDAC0_01
    .extern  BSP_IntHandlerCCU40_00
    .extern  BSP_IntHandlerCCU40_01
    .extern  BSP_IntHandlerCCU40_02
    .extern  BSP_IntHandlerCCU40_03
    .extern  BSP_IntHandlerCCU41_00
    .extern  BSP_IntHandlerCCU41_01
    .extern  BSP_IntHandlerCCU41_02
    .extern  BSP_IntHandlerCCU41_03
    .extern  BSP_IntHandlerCCU42_00
    .extern  BSP_IntHandlerCCU42_01
    .extern  BSP_IntHandlerCCU42_02
    .extern  BSP_IntHandlerCCU42_03
    .extern  BSP_IntHandlerCCU43_00
    .extern  BSP_IntHandlerCCU43_01
    .extern  BSP_IntHandlerCCU43_02
    .extern  BSP_IntHandlerCCU43_03
    .extern  BSP_IntHandlerCCU80_00
    .extern  BSP_IntHandlerCCU80_01
    .extern  BSP_IntHandlerCCU80_02
    .extern  BSP_IntHandlerCCU80_03
    .extern  BSP_IntHandlerCCU81_00
    .extern  BSP_IntHandlerCCU81_01
    .extern  BSP_IntHandlerCCU81_02
    .extern  BSP_IntHandlerCCU81_03
    .extern  BSP_IntHandlerPOSIF0_00
    .extern  BSP_IntHandlerPOSIF0_01
    .extern  BSP_IntHandlerPOSIF1_00
    .extern  BSP_IntHandlerPOSIF1_01

    .extern  BSP_IntHandlerCAN0_00
    .extern  BSP_IntHandlerCAN0_01
    .extern  BSP_IntHandlerCAN0_02
    .extern  BSP_IntHandlerCAN0_03
    .extern  BSP_IntHandlerCAN0_04
    .extern  BSP_IntHandlerCAN0_05
    .extern  BSP_IntHandlerCAN0_06
    .extern  BSP_IntHandlerCAN0_07
    .extern  BSP_IntHandlerUSIC0_00
    .extern  BSP_IntHandlerUSIC0_01
    .extern  BSP_IntHandlerUSIC0_02
    .extern  BSP_IntHandlerUSIC0_03
    .extern  BSP_IntHandlerUSIC0_04
    .extern  BSP_IntHandlerUSIC0_05
    .extern  BSP_IntHandlerUSIC1_00
    .extern  BSP_IntHandlerUSIC1_01
    .extern  BSP_IntHandlerUSIC1_02
    .extern  BSP_IntHandlerUSIC1_03
    .extern  BSP_IntHandlerUSIC1_04
    .extern  BSP_IntHandlerUSIC1_05
    .extern  BSP_IntHandlerUSIC2_00
    .extern  BSP_IntHandlerUSIC2_01
    .extern  BSP_IntHandlerUSIC2_02
    .extern  BSP_IntHandlerUSIC2_03
    .extern  BSP_IntHandlerUSIC2_04
    .extern  BSP_IntHandlerUSIC2_05
    .extern  BSP_IntHandlerLEDTS0_00
    .extern  BSP_IntHandlerFCE0_00
    .extern  BSP_IntHandlerGPDMA0_00
    .extern  BSP_IntHandlerSDMMC0_00
    .extern  BSP_IntHandlerUSB0_00
    .extern  BSP_IntHandlerETH0_00
    .extern  BSP_IntHandlerGPDMA1_00

    .extern  OS_CPU_PendSVHandler
    .extern  OS_CPU_SysTickHandler


g_pfnVectors:
                                            @ Processor exception vectors
    .word     _estack                       @ Top of Stack
    .word     Reset_Handler                 @ Reset Handler
    .word     App_NMI_ISR                   @ NMI Handler
    .word     App_Fault_ISR                 @ Hard Fault Handler
    .word     App_MemFault_ISR              @ MPU Fault Handler
    .word     App_BusFault_ISR              @ Bus Fault Handler
    .word     App_UsageFault_ISR            @ Usage Fault Handler
    .word     App_Spurious_ISR              @ Reserved
    .word     App_Spurious_ISR              @ Reserved
    .word     App_Spurious_ISR              @ Reserved
    .word     App_Spurious_ISR              @ Reserved
    .word     App_Spurious_ISR              @ SVCall Handler
    .word     App_Spurious_ISR              @ Debug Monitor Handler
    .word     App_Spurious_ISR              @ Reserved
    .word     OS_CPU_PendSVHandler          @ PendSV Handler
    .word     OS_CPU_SysTickHandler         @ SysTick Handler

                                            @  External Interrupts
    .word     BSP_IntHandlerSCU             @  16, INTISR[  0]  SCU Interrupt.
    .word     BSP_IntHandlerERU0_00         @  17, INTISR[  1]  ERU0 SR0 Interrupt.
    .word     BSP_IntHandlerERU0_01         @  18, INTISR[  2]  ERU0 SR0 Interrupt.
    .word     BSP_IntHandlerERU0_02         @  19, INTISR[  3]  ERU0 SR0 Interrupt.
    .word     BSP_IntHandlerERU0_03         @  20, INTISR[  4]  ERU0 SR0 Interrupt.
    .word     BSP_IntHandlerERU1_00         @  21, INTISR[  5]  ERU1 SR0 Interrupt.
    .word     BSP_IntHandlerERU1_01         @  22, INTISR[  6]  ERU1 SR1 Interrupt.
    .word     BSP_IntHandlerERU1_02         @  23, INTISR[  7]  ERU1 SR2 Interrupt.
    .word     BSP_IntHandlerERU1_03         @  24, INTISR[  8]  ERU1 SR3 Interrupt.

    .word     App_Spurious_ISR              @  25, INTISR[  9]  Reserved.
    .word     App_Spurious_ISR              @  26, INTISR[ 10]  Reserved.
    .word     App_Spurious_ISR              @  27, INTISR[ 11]  Reserved.
    .word     BSP_IntHandlerPMU0_00         @  28, INTISR[ 12]  Program Management Unit SR0 Interrupt.
    .word     App_Spurious_ISR              @  29, INTISR[ 13]  Reserved.

    .word     BSP_IntHandlerVADC0_C0_00     @  30, INTISR[ 14]  Analog to digital converter common block 0 SR0 Int.
    .word     BSP_IntHandlerVADC0_C0_01     @  31, INTISR[ 15]  Analog to digital converter common block 0 SR1 Int.
    .word     BSP_IntHandlerVADC0_C0_02     @  32, INTISR[ 16]  Analog to digital converter common block 0 SR2 Int.
    .word     BSP_IntHandlerVADC0_C0_03     @  33, INTISR[ 17]  Analog to digital converter common block 0 SR3 Int.

    .word     BSP_IntHandlerVADC0_G0_00     @  34, INTISR[ 18]  Analog to digital converter group 0 SR0 Interrupt.
    .word     BSP_IntHandlerVADC0_G0_01     @  35, INTISR[ 19]  Analog to digital converter group 0 SR1 Interrupt.
    .word     BSP_IntHandlerVADC0_G0_02     @  36, INTISR[ 20]  Analog to digital converter group 0 SR2 Interrupt.
    .word     BSP_IntHandlerVADC0_G0_03     @  37, INTISR[ 21]  Analog to digital converter group 0 SR3 Interrupt.
    .word     BSP_IntHandlerVADC0_G1_00     @  38, INTISR[ 22]  Analog to digital converter group 1 SR0 Interrupt.
    .word     BSP_IntHandlerVADC0_G1_01     @  39, INTISR[ 23]  Analog to digital converter group 1 SR1 Interrupt.
    .word     BSP_IntHandlerVADC0_G1_02     @  40, INTISR[ 24]  Analog to digital converter group 1 SR2 Interrupt.
    .word     BSP_IntHandlerVADC0_G1_03     @  41, INTISR[ 25]  Analog to digital converter group 1 SR3 Interrupt.
    .word     BSP_IntHandlerVADC0_G2_00     @  42, INTISR[ 26]  Analog to digital converter group 2 SR0 Interrupt.
    .word     BSP_IntHandlerVADC0_G2_01     @  43, INTISR[ 27]  Analog to digital converter group 2 SR1 Interrupt.
    .word     BSP_IntHandlerVADC0_G2_02     @  44, INTISR[ 28]  Analog to digital converter group 2 SR2 Interrupt.
    .word     BSP_IntHandlerVADC0_G2_03     @  45, INTISR[ 29]  Analog to digital converter group 2 SR3 Interrupt.
    .word     BSP_IntHandlerVADC0_G3_00     @  46, INTISR[ 30]  Analog to digital converter group 3 SR0 Interrupt.
    .word     BSP_IntHandlerVADC0_G3_01     @  47, INTISR[ 31]  Analog to digital converter group 3 SR1 Interrupt.
    .word     BSP_IntHandlerVADC0_G3_02     @  48, INTISR[ 32]  Analog to digital converter group 3 SR2 Interrupt.
    .word     BSP_IntHandlerVADC0_G3_03     @  49, INTISR[ 33]  Analog to digital converter group 3 SR3 Interrupt.

    .word     BSP_IntHandlerDSD0_00         @  50, INTISR[ 34]  Delta Sigma Demodulator SR0 Interrupt.
    .word     BSP_IntHandlerDSD0_01         @  51, INTISR[ 35]  Delta Sigma Demodulator SR1 Interrupt.
    .word     BSP_IntHandlerDSD0_02         @  52, INTISR[ 36]  Delta Sigma Demodulator SR2 Interrupt.
    .word     BSP_IntHandlerDSD0_03         @  53, INTISR[ 37]  Delta Sigma Demodulator SR3 Interrupt.
    .word     BSP_IntHandlerDSD0_04         @  54, INTISR[ 38]  Delta Sigma Demodulator SR4 Interrupt.
    .word     BSP_IntHandlerDSD0_05         @  55, INTISR[ 39]  Delta Sigma Demodulator SR5 Interrupt.
    .word     BSP_IntHandlerDSD0_06         @  56, INTISR[ 40]  Delta Sigma Demodulator SR6 Interrupt.
    .word     BSP_IntHandlerDSD0_07         @  57, INTISR[ 41]  Delta Sigma Demodulator SR7 Interrupt.

    .word     BSP_IntHandlerDAC0_00         @  58, INTISR[ 42]  Digital to anolog converter SR0 Interrupt.
    .word     BSP_IntHandlerDAC0_01         @  59, INTISR[ 43]  Digital to anolog converter SR1 Interrupt.

    .word     BSP_IntHandlerCCU40_00        @  60, INTISR[ 44]  Capture compare Unit 4 (Module 0) SR 0 Interrupt.
    .word     BSP_IntHandlerCCU40_01        @  61, INTISR[ 45]  Capture compare Unit 4 (Module 0) SR 1 Interrupt.
    .word     BSP_IntHandlerCCU40_02        @  62, INTISR[ 46]  Capture compare Unit 4 (Module 0) SR 2 Interrupt.
    .word     BSP_IntHandlerCCU40_03        @  63, INTISR[ 47]  Capture compare Unit 4 (Module 0) SR 3 Interrupt.
    .word     BSP_IntHandlerCCU41_00        @  64, INTISR[ 48]  Capture compare Unit 4 (Module 1) SR 0 Interrupt.
    .word     BSP_IntHandlerCCU41_01        @  65, INTISR[ 49]  Capture compare Unit 4 (Module 1) SR 1 Interrupt.
    .word     BSP_IntHandlerCCU41_02        @  66, INTISR[ 50]  Capture compare Unit 4 (Module 1) SR 2 Interrupt.
    .word     BSP_IntHandlerCCU41_03        @  67, INTISR[ 51]  Capture compare Unit 4 (Module 1) SR 3 Interrupt.
    .word     BSP_IntHandlerCCU42_00        @  68, INTISR[ 52]  Capture compare Unit 4 (Module 2) SR 0 Interrupt.
    .word     BSP_IntHandlerCCU42_01        @  69, INTISR[ 53]  Capture compare Unit 4 (Module 2) SR 1 Interrupt.
    .word     BSP_IntHandlerCCU42_02        @  70, INTISR[ 54]  Capture compare Unit 4 (Module 2) SR 2 Interrupt.
    .word     BSP_IntHandlerCCU42_03        @  71, INTISR[ 55]  Capture compare Unit 4 (Module 2) SR 3 Interrupt.
    .word     BSP_IntHandlerCCU43_00        @  72, INTISR[ 56]  Capture compare Unit 4 (Module 3) SR 0 Interrupt.
    .word     BSP_IntHandlerCCU43_01        @  73, INTISR[ 57]  Capture compare Unit 4 (Module 3) SR 1 Interrupt.
    .word     BSP_IntHandlerCCU43_02        @  74, INTISR[ 58]  Capture compare Unit 4 (Module 3) SR 2 Interrupt.
    .word     BSP_IntHandlerCCU43_03        @  75, INTISR[ 59]  Capture compare Unit 4 (Module 3) SR 3 Interrupt.

    .word     BSP_IntHandlerCCU80_00        @  76, INTISR[ 60]  Capture compare Unit 8 (Module 0) SR 0 Interrupt.
    .word     BSP_IntHandlerCCU80_01        @  77, INTISR[ 61]  Capture compare Unit 8 (Module 0) SR 1 Interrupt.
    .word     BSP_IntHandlerCCU80_02        @  78, INTISR[ 62]  Capture compare Unit 8 (Module 0) SR 2 Interrupt.
    .word     BSP_IntHandlerCCU80_03        @  79, INTISR[ 63]  Capture compare Unit 8 (Module 0) SR 3 Interrupt.
    .word     BSP_IntHandlerCCU81_00        @  80, INTISR[ 64]  Capture compare Unit 8 (Module 1) SR 0 Interrupt.
    .word     BSP_IntHandlerCCU81_01        @  81, INTISR[ 65]  Capture compare Unit 8 (Module 1) SR 1 Interrupt.
    .word     BSP_IntHandlerCCU81_02        @  82, INTISR[ 66]  Capture compare Unit 8 (Module 1) SR 2 Interrupt.
    .word     BSP_IntHandlerCCU81_03        @  83, INTISR[ 67]  Capture compare Unit 8 (Module 1) SR 3 Interrupt.

    .word     BSP_IntHandlerPOSIF0_00       @  84, INTISR[ 68]  Position interface (Module 0) SR0 Interrupt.
    .word     BSP_IntHandlerPOSIF0_01       @  85, INTISR[ 69]  Position interface (Module 0) SR1 Interrupt.
    .word     BSP_IntHandlerPOSIF1_00       @  86, INTISR[ 70]  Position interface (Module 1) SR0 Interrupt.
    .word     BSP_IntHandlerPOSIF1_01       @  87, INTISR[ 71]  Position interface (Module 1) SR1 Interrupt.
    .word     App_Spurious_ISR              @  88, INTISR[ 72]  Reserved.
    .word     App_Spurious_ISR              @  89, INTISR[ 73]  Reserved.
    .word     App_Spurious_ISR              @  90, INTISR[ 74]  Reserved.
    .word     App_Spurious_ISR              @  91, INTISR[ 75]  Reserved.

    .word     BSP_IntHandlerCAN0_00         @  92, INTISR[ 76]  MultiCAN SR0 Interrupt.
    .word     BSP_IntHandlerCAN0_01         @  93, INTISR[ 77]  MultiCAN SR1 Interrupt.
    .word     BSP_IntHandlerCAN0_02         @  94, INTISR[ 78]  MultiCAN SR2 Interrupt.
    .word     BSP_IntHandlerCAN0_03         @  95, INTISR[ 79]  MultiCAN SR3 Interrupt.
    .word     BSP_IntHandlerCAN0_04         @  96, INTISR[ 80]  MultiCAN SR4 Interrupt.
    .word     BSP_IntHandlerCAN0_05         @  97, INTISR[ 81]  MultiCAN SR5 Interrupt.
    .word     BSP_IntHandlerCAN0_06         @  98, INTISR[ 82]  MultiCAN SR6 Interrupt.
    .word     BSP_IntHandlerCAN0_07         @  99, INTISR[ 83]  MultiCAN SR7 Interrupt.
    .word     BSP_IntHandlerUSIC0_00        @ 100, INTISR[ 84]  Universal serial interface channel(Module 0) SR0 Int.
    .word     BSP_IntHandlerUSIC0_01        @ 101, INTISR[ 85]  Universal serial interface channel(Module 0) SR1 Int.
    .word     BSP_IntHandlerUSIC0_02        @ 102, INTISR[ 86]  Universal serial interface channel(Module 0) SR2 Int.
    .word     BSP_IntHandlerUSIC0_03        @ 103, INTISR[ 87]  Universal serial interface channel(Module 0) SR3 Int.
    .word     BSP_IntHandlerUSIC0_04        @ 104, INTISR[ 88]  Universal serial interface channel(Module 0) SR4 Int.
    .word     BSP_IntHandlerUSIC0_05        @ 105, INTISR[ 89]  Universal serial interface channel(Module 0) SR5 Int.
    .word     BSP_IntHandlerUSIC1_00        @ 106, INTISR[ 90]  Universal serial interface channel(Module 1) SR0 Int.
    .word     BSP_IntHandlerUSIC1_01        @ 107, INTISR[ 91]  Universal serial interface channel(Module 1) SR1 Int.
    .word     BSP_IntHandlerUSIC1_02        @ 108, INTISR[ 92]  Universal serial interface channel(Module 1) SR2 Int.
    .word     BSP_IntHandlerUSIC1_03        @ 109, INTISR[ 93]  Universal serial interface channel(Module 1) SR3 Int.
    .word     BSP_IntHandlerUSIC1_04        @ 110, INTISR[ 94]  Universal serial interface channel(Module 1) SR4 Int.
    .word     BSP_IntHandlerUSIC1_05        @ 111, INTISR[ 95]  Universal serial interface channel(Module 1) SR5 Int.
    .word     BSP_IntHandlerUSIC2_00        @ 112, INTISR[ 96]  Universal serial interface channel(Module 2) SR0 Int.
    .word     BSP_IntHandlerUSIC2_01        @ 113, INTISR[ 97]  Universal serial interface channel(Module 2) SR1 Int.
    .word     BSP_IntHandlerUSIC2_02        @ 114, INTISR[ 98]  Universal serial interface channel(Module 2) SR2 Int.
    .word     BSP_IntHandlerUSIC2_03        @ 115, INTISR[ 99]  Universal serial interface channel(Module 2) SR3 Int.
    .word     BSP_IntHandlerUSIC2_04        @ 116, INTISR[100]  Universal serial interface channel(Module 2) SR4 Int.
    .word     BSP_IntHandlerUSIC2_05        @ 117, INTISR[101]  Universal serial interface channel(Module 2) SR5 Int.

    .word     BSP_IntHandlerLEDTS0_00       @ 118, INTISR[102]  LED and touch sense control unit (Module 0) SR0 Int.
    .word     App_Spurious_ISR              @ 119, INTISR[103]  Reserved.
    .word     BSP_IntHandlerFCE0_00         @ 120, INTISR[104]  Flexible CRC Engine SR0 Interrupt.
    .word     BSP_IntHandlerGPDMA0_00       @ 121, INTISR[105]  General purpose DMA unit 0 SR0 Interrupt.
    .word     BSP_IntHandlerSDMMC0_00       @ 122, INTISR[106]  Secure Digital/MultiMediaCard interface SR0 Int.
    .word     BSP_IntHandlerUSB0_00         @ 123, INTISR[107]  EUSB SR0 Interrupt.
    .word     BSP_IntHandlerETH0_00         @ 124, INTISR[108]  Ethernet (Module 0) SR0 Interrupt.
    .word     App_Spurious_ISR              @ 125, INTISR[109]  Reserved.
    .word     BSP_IntHandlerGPDMA1_00       @ 126, INTISR[110]  General purpose DMA unit 1 SR0 Interrupt.
    .word     App_Spurious_ISR              @ 127, INTISR[111]  Reserved.


@ Dummy Exception Handlers (infinite loops which can be modified)

    .section	.text.App_NMI_ISR
App_NMI_ISR:
	b	App_NMI_ISR
	.size	App_NMI_ISR, .-App_NMI_ISR


    .section	.text.App_Fault_ISR
App_Fault_ISR:
	b	App_Fault_ISR
	.size	App_Fault_ISR, .-App_Fault_ISR


    .section	.text.App_MemFault_ISR
App_MemFault_ISR:
	b	App_MemFault_ISR
	.size	App_MemFault_ISR, .-App_MemFault_ISR


    .section	.text.App_BusFault_ISR
App_BusFault_ISR:
	b	App_BusFault_ISR
	.size	App_BusFault_ISR, .-App_BusFault_ISR


    .section	.text.App_UsageFault_ISR
App_UsageFault_ISR:
	b	App_UsageFault_ISR
	.size	App_UsageFault_ISR, .-App_UsageFault_ISR

    .section	.text.App_Spurious_ISR
App_Spurious_ISR:
	b	App_Spurious_ISR
	.size	App_Spurious_ISR, .-App_Spurious_ISR


    .section	.text.App_Reserved_ISR
App_Reserved_ISR:
	b	App_Reserved_ISR
	.size	App_Reserved_ISR, .-App_Reserved_ISR

.end
