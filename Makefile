################################################################################
# Makefile for XMC4500 RelaxKit using uCOS-III
# v1, 02/2015
# Martin Horauer, UAS Technikum Wien
#
# Supported: Windows, Linux, OSX
# Requirements:
# * GCC ARM https://launchpad.net/gcc-arm-embedded/+download
# * SEGGER JLINK https://www.segger.com/jlink-software.html
# * DOXYGEN http://www.stack.nl/~dimitri/doxygen/

################################################################################
# USAGE
# -----
# make           .... build the program image
# make debug     .... build the program image and invoke gdb
# make flash     .... build an flash the application
# make erase     .... erase the target device
# make doc       .... run doxygen - output will be in > doc
# make clean     .... remove intermediate and generated files

################################################################################
# define the name of the generated output file
#
TARGET        = main

################################################################################
# below only edit with care
#
VENDOR        = Infineon

################################################################################
# define the following symbol -D JLINK_RTT to enable JLINK_RTT tracing
#                             -D SEMI_HOSTING to enable semi hosted tracing
#                             comment the line to disable tracing
TRACE         = -D SEMI_HOSTING

################################################################################
# DIRECTORIES
SRCDIR        = .
BIN           = ./bin
DOC           = ./doc
SYS           = ../CMSIS
XMCLIB        = ../XMCLIB
OS            = ../UCOS3
BSP           = $(SRCDIR)/BSP
CMSIS         = $(SYS)/CMSIS
CMSIS_INCDIR  = $(CMSIS)/Include
CMSIS_LIBDIR  = $(CMSIS)/Lib/GCC
INF_INCDIR    = $(CMSIS)/$(VENDOR)/Include
INF_LIBDIR    = $(CMSIS)/$(VENDOR)/Lib
XMC_INCDIR    = $(CMSIS)/$(VENDOR)/$(UC)_series/Include
XMC_LIBDIR    = $(CMSIS)/$(VENDOR)/$(UC)_series/Lib
XMC_SRCDIR    = $(CMSIS)/$(VENDOR)/$(UC)_series/Source
XMC_GCCDIR    = $(CMSIS)/$(VENDOR)/$(UC)_series/Source/GCC
XMC_LIBINCDIR = $(XMCLIB)/inc
XMC_LIBSRCDIR = $(XMCLIB)/src
XMC_USBINCDIR = $(XMCLIB)/inc/USB
XMC_USBLIBDIR = $(XMCLIB)/src/USB

################################################################################
# TOOLS & ARGS
#
TERMINAL      = gnome-terminal
TOOLCHAIN     = arm-none-eabi
AS            = $(TOOLCHAIN)-as
CC            = $(TOOLCHAIN)-gcc
CP            = $(TOOLCHAIN)-objcopy
OD            = $(TOOLCHAIN)-objdump
GDB           = $(TOOLCHAIN)-gdb
SIZE          = $(TOOLCHAIN)-size
# DETERMINE OS
ifdef SystemRoot
  RM = del /Q
  FixPath = $(subst /,\,$1)
else
  RM = rm -rf
  FixPath = $1
endif

UC            = XMC4500
UC_ID         = 4503
CPU           = cortex-m4
FPU           = fpv4-sp-d16
FABI          = softfp  #soft, softfp, hard
LIBS          = -larm_cortexM4_mathL_2
LIBS         += -lxmclibcstubs
LIBS         += -lm
GDB_ARGS      = -ex "target remote :2331"
GDB_ARGS     += -ex "monitor reset"
GDB_ARGS     += -ex "load"
GDB_ARGS     += -ex "monitor reset"

################################################################################
# SEMI_HOSTED DEBUGGING
GDB_ARGS     += -ex "monitor SWO EnableTarget 16000000 0 1 0"

# RTT OPTION
#GDB_ARGS     += -ex "monitor exec SetRTTAddr 0x20000000"
#GDB_ARGS     += -ex "monitor exec SetRTTSearchRanges 0x20000000 0x1000"

################################################################################
# OS Source
SRC  = $(wildcard *.c)
SRC += $(OS)/uC-CPU/cpu_core.c
SRC += $(OS)/uC-CPU/ARM-Cortex-M4/GNU/cpu_c.c
SRC += $(wildcard $(OS)/uC-LIB/*.c)
SRC += $(wildcard $(OS)/uCOS-III/Source/*.c)
SRC += $(OS)/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU/os_cpu_c.c
SRC += $(wildcard $(BSP)/*.c)

################################################################################
# SYSTEM SOURCES
SRC += $(INF_LIBDIR)/System_LibcStubs.c
SRC += $(XMC_SRCDIR)/System_XMC4500.c
SRC += $(wildcard $(XMC_LIBSRCDIR)/*.c)
SRC += $(wildcard $(XMC_USBLIBDIR)/*.c)

################################################################################
# ASSEMBLER SOURCES
SRCASM = $(BSP)/startup.asm
SRCASM += $(OS)/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU/os_cpu_a.asm
SRCASM += $(OS)/uC-CPU/ARM-Cortex-M4/GNU/cpu_a.asm
SRCASM += $(OS)/uC-LIB/Ports/ARM-Cortex-M4/GNU/lib_mem_a.asm
SRCASM += $(XMC_GCCDIR)/startup_XMC4500.asm

################################################################################
# LINKER_FILE
LINKER_FILE = $(XMC_GCCDIR)/xmc4500_ucos.ld

################################################################################
# INCLUDE DIRECTORIES
OS_INCDIR += -I$(OS)
OS_INCDIR += -I$(OS)/uC-CPU
OS_INCDIR += -I$(OS)/uC-CPU/ARM-Cortex-M4/GNU
OS_INCDIR += -I$(OS)/uC-LIB
OS_INCDIR += -I$(OS)/uCOS-III/Source
OS_INCDIR += -I$(OS)/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU
OS_INCDIR += -I$(BSP)

INC_DIR = -I$(SRCDIR)
INC_DIR+= -I$(SYS)
INC_DIR+= -I$(CMSIS_INCDIR)
INC_DIR+= -I$(INF_INCDIR)
INC_DIR+= -I$(XMC_INCDIR)
INC_DIR+= -I$(OS_INCDIR)
INC_DIR+= -I$(XMC_LIBINCDIR)
INC_DIR+= -I$(XMC_USBINCDIR)

################################################################################
# LIBRARY DIRECTORIES
LIBS_DIR  = -L$(SYS)
LIBS_DIR += -L$(CMSIS_LIBDIR)
LIBS_DIR += -L$(INF_LIBDIR)
LIBS_DIR += -L$(XMC_LIBDIR)

################################################################################
# OBJECT FILES
OBJS = $(SRC:.c=.o)
OBJS+= $(SRCASM:.asm=.o)

################################################################################
# DEPENDENCY FILES
DEPS = $(SRC:.c=.d)
DEPS += $(SRCASM:.asm=.d)

################################################################################
# STACK USAGE FILES
SU = $(SRC:.c=.su)

################################################################################
# COMPILER, ASSEMBLER OPTIONS
CFLAGS = -mthumb
CFLAGS+= -mcpu=$(CPU)
#CFLAGS+= -mfpu=$(FPU)
#CFLAGS+= -mfloat-abi=$(FABI)
CFLAGS+= -O0
CFLAGS+= -ffunction-sections -fdata-sections -fsigned-char -fstack-usage
CFLAGS+= -MD -std=c99 -Wall -fms-extensions
CFLAGS+= -DUC_ID=$(UC_ID) -DARM_MATH_CM4 -DXMC4500_F144x1024
CFLAGS+= -g3 -fmessage-length=0 
AFLAGS = -x assembler-with-cpp
LFLAGS = -nostartfiles $(LIBS_DIR) -Wl,--gc-sections -Wl,-Map=bin/$(TARGET).map
CPFLAGS = -Obinary
ODFLAGS = -S

################################################################################
# BUILD RULES
all: $(OBJS) $(TARGET).elf $(TARGET)

%.o: %.asm
	@echo "----------------------------------------------------------------------"
	@echo "Assembly of $<:"
	@echo ""
	$(CC) -c $(CFLAGS) $(INC_DIR) $(AFLAGS) $< -o $@
	@echo ""

%.o: %.c
	@echo "----------------------------------------------------------------------"
	@echo "Compilation of $<:"
	@echo ""
	$(CC) -c $(CFLAGS) $(INC_DIR) $< -o $@
	@echo ""

$(TARGET).elf: $(OBJS)
	@echo "----------------------------------------------------------------------"
	@echo "Linking:"
	@echo ""
	$(CC) -T $(LINKER_FILE) $(LFLAGS) $(CFLAGS) -o $(BIN)/$(TARGET).elf $(OBJS) $(LIBS)
	@echo ""

$(TARGET): $(TARGET).elf
	@echo "----------------------------------------------------------------------"
	@echo "Creation of Binary:"
	@echo ""
	$(CP) $(CPFLAGS) $(BIN)/$(TARGET).elf $(BIN)/$(TARGET).bin
	@echo "----------------------------------------------------------------------"
	@echo "Create Listing File:"
	@echo ""
	$(OD) $(ODFLAGS) $(BIN)/$(TARGET).elf > $(BIN)/$(TARGET).lst
	@echo "----------------------------------------------------------------------"
	@echo "Create Static Usage Analysis:"
	@echo ""
	$(SIZE) $(BIN)/$(TARGET).elf

################################################################################
# DEBUG RULES
debug: $(TARGET)
ifdef SystemRoot
	@call start JLinkGDBServer -Device XMC4500-1024 -if SWD
else
	$(TERMINAL) -e "JLinkGDBServer -Device XMC4500-1024 -if SWD" &
	sleep 1 && $(TERMINAL) -e "telnet localhost 2333" & 
endif
	$(GDB) -q $(BIN)/$(TARGET).elf $(GDB_ARGS)

################################################################################
# FLASH RULES
flash: $(TARGET)
	echo -e 'speed 4000\nconnect\nh\nloadbin bin/$(TARGET).bin,0xC000000\nr\ng\nq' | JLinkExe -Device XMC4500-1024 -if SWD

################################################################################
# ERASE DEVICE
erase:
	echo -e 'speed 4000\nconnect\nerase\nr\nq' | JLinkExe -Device XMC4500-1024 -if SWD

################################################################################
# DOCUMENTATION RULES
doc: $(TARGET)
	doxygen

################################################################################
# CLEAN RULES
clean:
	$(RM) $(call FixPath, ${OBJS} ${DEPS} ${SU})
	$(RM) $(call FixPath, ${BIN}/*)
	$(RM) $(call FixPath, ${DOC}/html/*)

################################################################################
# EOF
################################################################################
