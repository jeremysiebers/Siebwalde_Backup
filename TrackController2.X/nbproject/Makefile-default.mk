#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/TrackController2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/TrackController2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=--mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=modbus/General.c modbus/Init.c modbus/Interrupts.c modbus/PetitModbus.c modbus/PetitModbusPort.c modbus/PetitRegConstant.c Peripherals/config.c Peripherals/interrupt_manager.c Peripherals/pin_manager.c Peripherals/tmr2.c Peripherals/tmr1.c main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/modbus/General.p1 ${OBJECTDIR}/modbus/Init.p1 ${OBJECTDIR}/modbus/Interrupts.p1 ${OBJECTDIR}/modbus/PetitModbus.p1 ${OBJECTDIR}/modbus/PetitModbusPort.p1 ${OBJECTDIR}/modbus/PetitRegConstant.p1 ${OBJECTDIR}/Peripherals/config.p1 ${OBJECTDIR}/Peripherals/interrupt_manager.p1 ${OBJECTDIR}/Peripherals/pin_manager.p1 ${OBJECTDIR}/Peripherals/tmr2.p1 ${OBJECTDIR}/Peripherals/tmr1.p1 ${OBJECTDIR}/main.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/modbus/General.p1.d ${OBJECTDIR}/modbus/Init.p1.d ${OBJECTDIR}/modbus/Interrupts.p1.d ${OBJECTDIR}/modbus/PetitModbus.p1.d ${OBJECTDIR}/modbus/PetitModbusPort.p1.d ${OBJECTDIR}/modbus/PetitRegConstant.p1.d ${OBJECTDIR}/Peripherals/config.p1.d ${OBJECTDIR}/Peripherals/interrupt_manager.p1.d ${OBJECTDIR}/Peripherals/pin_manager.p1.d ${OBJECTDIR}/Peripherals/tmr2.p1.d ${OBJECTDIR}/Peripherals/tmr1.p1.d ${OBJECTDIR}/main.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/modbus/General.p1 ${OBJECTDIR}/modbus/Init.p1 ${OBJECTDIR}/modbus/Interrupts.p1 ${OBJECTDIR}/modbus/PetitModbus.p1 ${OBJECTDIR}/modbus/PetitModbusPort.p1 ${OBJECTDIR}/modbus/PetitRegConstant.p1 ${OBJECTDIR}/Peripherals/config.p1 ${OBJECTDIR}/Peripherals/interrupt_manager.p1 ${OBJECTDIR}/Peripherals/pin_manager.p1 ${OBJECTDIR}/Peripherals/tmr2.p1 ${OBJECTDIR}/Peripherals/tmr1.p1 ${OBJECTDIR}/main.p1

# Source Files
SOURCEFILES=modbus/General.c modbus/Init.c modbus/Interrupts.c modbus/PetitModbus.c modbus/PetitModbusPort.c modbus/PetitRegConstant.c Peripherals/config.c Peripherals/interrupt_manager.c Peripherals/pin_manager.c Peripherals/tmr2.c Peripherals/tmr1.c main.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/TrackController2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=16F777
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/modbus/General.p1: modbus/General.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/modbus" 
	@${RM} ${OBJECTDIR}/modbus/General.p1.d 
	@${RM} ${OBJECTDIR}/modbus/General.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/modbus/General.p1  modbus/General.c 
	@-${MV} ${OBJECTDIR}/modbus/General.d ${OBJECTDIR}/modbus/General.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/modbus/General.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/modbus/Init.p1: modbus/Init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/modbus" 
	@${RM} ${OBJECTDIR}/modbus/Init.p1.d 
	@${RM} ${OBJECTDIR}/modbus/Init.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/modbus/Init.p1  modbus/Init.c 
	@-${MV} ${OBJECTDIR}/modbus/Init.d ${OBJECTDIR}/modbus/Init.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/modbus/Init.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/modbus/Interrupts.p1: modbus/Interrupts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/modbus" 
	@${RM} ${OBJECTDIR}/modbus/Interrupts.p1.d 
	@${RM} ${OBJECTDIR}/modbus/Interrupts.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/modbus/Interrupts.p1  modbus/Interrupts.c 
	@-${MV} ${OBJECTDIR}/modbus/Interrupts.d ${OBJECTDIR}/modbus/Interrupts.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/modbus/Interrupts.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/modbus/PetitModbus.p1: modbus/PetitModbus.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/modbus" 
	@${RM} ${OBJECTDIR}/modbus/PetitModbus.p1.d 
	@${RM} ${OBJECTDIR}/modbus/PetitModbus.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/modbus/PetitModbus.p1  modbus/PetitModbus.c 
	@-${MV} ${OBJECTDIR}/modbus/PetitModbus.d ${OBJECTDIR}/modbus/PetitModbus.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/modbus/PetitModbus.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/modbus/PetitModbusPort.p1: modbus/PetitModbusPort.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/modbus" 
	@${RM} ${OBJECTDIR}/modbus/PetitModbusPort.p1.d 
	@${RM} ${OBJECTDIR}/modbus/PetitModbusPort.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/modbus/PetitModbusPort.p1  modbus/PetitModbusPort.c 
	@-${MV} ${OBJECTDIR}/modbus/PetitModbusPort.d ${OBJECTDIR}/modbus/PetitModbusPort.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/modbus/PetitModbusPort.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/modbus/PetitRegConstant.p1: modbus/PetitRegConstant.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/modbus" 
	@${RM} ${OBJECTDIR}/modbus/PetitRegConstant.p1.d 
	@${RM} ${OBJECTDIR}/modbus/PetitRegConstant.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/modbus/PetitRegConstant.p1  modbus/PetitRegConstant.c 
	@-${MV} ${OBJECTDIR}/modbus/PetitRegConstant.d ${OBJECTDIR}/modbus/PetitRegConstant.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/modbus/PetitRegConstant.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Peripherals/config.p1: Peripherals/config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Peripherals" 
	@${RM} ${OBJECTDIR}/Peripherals/config.p1.d 
	@${RM} ${OBJECTDIR}/Peripherals/config.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Peripherals/config.p1  Peripherals/config.c 
	@-${MV} ${OBJECTDIR}/Peripherals/config.d ${OBJECTDIR}/Peripherals/config.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Peripherals/config.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Peripherals/interrupt_manager.p1: Peripherals/interrupt_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Peripherals" 
	@${RM} ${OBJECTDIR}/Peripherals/interrupt_manager.p1.d 
	@${RM} ${OBJECTDIR}/Peripherals/interrupt_manager.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Peripherals/interrupt_manager.p1  Peripherals/interrupt_manager.c 
	@-${MV} ${OBJECTDIR}/Peripherals/interrupt_manager.d ${OBJECTDIR}/Peripherals/interrupt_manager.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Peripherals/interrupt_manager.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Peripherals/pin_manager.p1: Peripherals/pin_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Peripherals" 
	@${RM} ${OBJECTDIR}/Peripherals/pin_manager.p1.d 
	@${RM} ${OBJECTDIR}/Peripherals/pin_manager.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Peripherals/pin_manager.p1  Peripherals/pin_manager.c 
	@-${MV} ${OBJECTDIR}/Peripherals/pin_manager.d ${OBJECTDIR}/Peripherals/pin_manager.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Peripherals/pin_manager.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Peripherals/tmr2.p1: Peripherals/tmr2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Peripherals" 
	@${RM} ${OBJECTDIR}/Peripherals/tmr2.p1.d 
	@${RM} ${OBJECTDIR}/Peripherals/tmr2.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Peripherals/tmr2.p1  Peripherals/tmr2.c 
	@-${MV} ${OBJECTDIR}/Peripherals/tmr2.d ${OBJECTDIR}/Peripherals/tmr2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Peripherals/tmr2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Peripherals/tmr1.p1: Peripherals/tmr1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Peripherals" 
	@${RM} ${OBJECTDIR}/Peripherals/tmr1.p1.d 
	@${RM} ${OBJECTDIR}/Peripherals/tmr1.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Peripherals/tmr1.p1  Peripherals/tmr1.c 
	@-${MV} ${OBJECTDIR}/Peripherals/tmr1.d ${OBJECTDIR}/Peripherals/tmr1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Peripherals/tmr1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/main.p1: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.p1.d 
	@${RM} ${OBJECTDIR}/main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/main.p1  main.c 
	@-${MV} ${OBJECTDIR}/main.d ${OBJECTDIR}/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/modbus/General.p1: modbus/General.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/modbus" 
	@${RM} ${OBJECTDIR}/modbus/General.p1.d 
	@${RM} ${OBJECTDIR}/modbus/General.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/modbus/General.p1  modbus/General.c 
	@-${MV} ${OBJECTDIR}/modbus/General.d ${OBJECTDIR}/modbus/General.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/modbus/General.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/modbus/Init.p1: modbus/Init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/modbus" 
	@${RM} ${OBJECTDIR}/modbus/Init.p1.d 
	@${RM} ${OBJECTDIR}/modbus/Init.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/modbus/Init.p1  modbus/Init.c 
	@-${MV} ${OBJECTDIR}/modbus/Init.d ${OBJECTDIR}/modbus/Init.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/modbus/Init.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/modbus/Interrupts.p1: modbus/Interrupts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/modbus" 
	@${RM} ${OBJECTDIR}/modbus/Interrupts.p1.d 
	@${RM} ${OBJECTDIR}/modbus/Interrupts.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/modbus/Interrupts.p1  modbus/Interrupts.c 
	@-${MV} ${OBJECTDIR}/modbus/Interrupts.d ${OBJECTDIR}/modbus/Interrupts.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/modbus/Interrupts.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/modbus/PetitModbus.p1: modbus/PetitModbus.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/modbus" 
	@${RM} ${OBJECTDIR}/modbus/PetitModbus.p1.d 
	@${RM} ${OBJECTDIR}/modbus/PetitModbus.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/modbus/PetitModbus.p1  modbus/PetitModbus.c 
	@-${MV} ${OBJECTDIR}/modbus/PetitModbus.d ${OBJECTDIR}/modbus/PetitModbus.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/modbus/PetitModbus.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/modbus/PetitModbusPort.p1: modbus/PetitModbusPort.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/modbus" 
	@${RM} ${OBJECTDIR}/modbus/PetitModbusPort.p1.d 
	@${RM} ${OBJECTDIR}/modbus/PetitModbusPort.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/modbus/PetitModbusPort.p1  modbus/PetitModbusPort.c 
	@-${MV} ${OBJECTDIR}/modbus/PetitModbusPort.d ${OBJECTDIR}/modbus/PetitModbusPort.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/modbus/PetitModbusPort.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/modbus/PetitRegConstant.p1: modbus/PetitRegConstant.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/modbus" 
	@${RM} ${OBJECTDIR}/modbus/PetitRegConstant.p1.d 
	@${RM} ${OBJECTDIR}/modbus/PetitRegConstant.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/modbus/PetitRegConstant.p1  modbus/PetitRegConstant.c 
	@-${MV} ${OBJECTDIR}/modbus/PetitRegConstant.d ${OBJECTDIR}/modbus/PetitRegConstant.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/modbus/PetitRegConstant.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Peripherals/config.p1: Peripherals/config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Peripherals" 
	@${RM} ${OBJECTDIR}/Peripherals/config.p1.d 
	@${RM} ${OBJECTDIR}/Peripherals/config.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Peripherals/config.p1  Peripherals/config.c 
	@-${MV} ${OBJECTDIR}/Peripherals/config.d ${OBJECTDIR}/Peripherals/config.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Peripherals/config.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Peripherals/interrupt_manager.p1: Peripherals/interrupt_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Peripherals" 
	@${RM} ${OBJECTDIR}/Peripherals/interrupt_manager.p1.d 
	@${RM} ${OBJECTDIR}/Peripherals/interrupt_manager.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Peripherals/interrupt_manager.p1  Peripherals/interrupt_manager.c 
	@-${MV} ${OBJECTDIR}/Peripherals/interrupt_manager.d ${OBJECTDIR}/Peripherals/interrupt_manager.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Peripherals/interrupt_manager.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Peripherals/pin_manager.p1: Peripherals/pin_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Peripherals" 
	@${RM} ${OBJECTDIR}/Peripherals/pin_manager.p1.d 
	@${RM} ${OBJECTDIR}/Peripherals/pin_manager.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Peripherals/pin_manager.p1  Peripherals/pin_manager.c 
	@-${MV} ${OBJECTDIR}/Peripherals/pin_manager.d ${OBJECTDIR}/Peripherals/pin_manager.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Peripherals/pin_manager.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Peripherals/tmr2.p1: Peripherals/tmr2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Peripherals" 
	@${RM} ${OBJECTDIR}/Peripherals/tmr2.p1.d 
	@${RM} ${OBJECTDIR}/Peripherals/tmr2.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Peripherals/tmr2.p1  Peripherals/tmr2.c 
	@-${MV} ${OBJECTDIR}/Peripherals/tmr2.d ${OBJECTDIR}/Peripherals/tmr2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Peripherals/tmr2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Peripherals/tmr1.p1: Peripherals/tmr1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Peripherals" 
	@${RM} ${OBJECTDIR}/Peripherals/tmr1.p1.d 
	@${RM} ${OBJECTDIR}/Peripherals/tmr1.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/Peripherals/tmr1.p1  Peripherals/tmr1.c 
	@-${MV} ${OBJECTDIR}/Peripherals/tmr1.d ${OBJECTDIR}/Peripherals/tmr1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Peripherals/tmr1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/main.p1: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.p1.d 
	@${RM} ${OBJECTDIR}/main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/main.p1  main.c 
	@-${MV} ${OBJECTDIR}/main.d ${OBJECTDIR}/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/TrackController2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) --chip=$(MP_PROCESSOR_OPTION) -G -mdist/${CND_CONF}/${IMAGE_TYPE}/TrackController2.X.${IMAGE_TYPE}.map  -D__DEBUG=1 --debugger=pickit3  --double=24 --float=24 --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     --rom=default,-1f00-1fff --ram=default,-0-0,-70-70,-80-80,-f0-f0,-100-100,-165-170,-180-180,-1f0-1f0  $(COMPARISON_BUILD) --memorysummary dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -odist/${CND_CONF}/${IMAGE_TYPE}/TrackController2.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} dist/${CND_CONF}/${IMAGE_TYPE}/TrackController2.X.${IMAGE_TYPE}.hex 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/TrackController2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) --chip=$(MP_PROCESSOR_OPTION) -G -mdist/${CND_CONF}/${IMAGE_TYPE}/TrackController2.X.${IMAGE_TYPE}.map  --double=24 --float=24 --opt=+asm,+asmfile,+speed,-space,-debug,-local --addrqual=ignore --mode=pro -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,-download,-stackcall,+clib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     $(COMPARISON_BUILD) --memorysummary dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -odist/${CND_CONF}/${IMAGE_TYPE}/TrackController2.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
