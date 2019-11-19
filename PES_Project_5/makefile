############################
# Command for removing files
RM := rm -rf

############################
# Include files
INCLUDES := \
	-I"CMSIS" \
	-I"source" \
	-I"board" \
	-I"drivers" \
	-I"utilities"
	
############################
# Compiler for FB versions
CC := arm-none-eabi-gcc

############################
# Linker for FB versions
LL := arm-none-eabi-gcc

###########################################
# Binary/exectable
EXE := \
  ./debug/PES_Project_5.axf

########################################################
# List of object files for DEBUG_LED and NORMAL versions
OBJS := ./debug/PES_Project_5.o \
	./debug/logger.o \
	./debug/circular_buffer.o \
	./debug/uart.o \
	./debug/systick.o \
	./debug/led.o \
	./debug/startup_mkl25z4.o \
	./debug/system_MKL25Z4.o	\
	./debug/board.o \
	./debug/clock_config.o \
	./debug/peripherals.o \
	./debug/pin_mux.o \
	./debug/fsl_clock.o \
	./debug/fsl_common.o \
	./debug/fsl_flash.o \
	./debug/fsl_gpio.o \
	./debug/fsl_lpsci.o \
	./debug/fsl_smc.o \
	./debug/fsl_uart.o ./debug/fsl_debug_console.o
	
############################
# List of dependency files 
C_DEPS = \
	./debug/PES_Project_5.d \
	./debug/logger.d \
	./debug/circular_buffer.d \
	./debug/uart.d \
	./debug/systick.d \
	./debug/led.d \
	./debug/startup_mkl25z4.d \
	./debug/system_MKL25Z4.d \
	./debug/board.d \
	./debug/clock_config.d \
	./debug/peripherals.d \
	./debug/pin_mux.d \
	./debug/fsl_clock.d \
	./debug/fsl_common.d \
	./debug/fsl_flash.d \
	./debug/fsl_gpio.d \
	./debug/fsl_lpsci.d \
	./debug/fsl_smc.d \
	./debug/fsl_uart.d ./debug/fsl_debug_console.d
 
##############################################################
# Include generated dependency files (only if not clean target)
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

##################################
# Compiler options
CC_OPTIONS := \
	-c \
	-std=gnu99 \
	-O0 \
	-g3 \
	-ffunction-sections \
	-fmessage-length=0\
	-fno-common \
	-fdata-sections \
	-fno-builtin \
	-mcpu=cortex-m0plus \
	-mthumb \
	-Wall \
	-Werror

################################
# Build options
B_OPTIONS := \
	-D__REDLIB__ \
	-DCPU_MKL25Z128VLK4 \
	-DCPU_MKL25Z128VLK4_cm0plus \
	-DSDK_OS_BAREMETAL \
	-DFSL_RTOS_BM \
	-DSDK_DEBUGCONSOLE=1 \
	-DCR_INTEGER_PRINTF \
	-DPRINTF_FLOAT_ENABLE=0 \
	-DSCANF_FLOAT_ENABLE=0 \
	-DPRINTF_ADVANCED_ENABLE=0 \
	-DSCANF_ADVANCED_ENABLE=0 \
	-D__MCUXPRESSO \
	-D__USE_CMSIS \
	-DDEBUG \
	-DFRDM_KL25Z \
	-DFREEDOM \
	-specs=redlib.specs \

################################
# Linker Options for FB versions
LL_OPTIONS := -nostdlib -Xlinker -Map="debug/PES_Project_5.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m0plus -mthumb -T linkerfile.ld -o $(EXE)

###########################################################
# Checking different versions and enabling necessary macros
ifeq ($(MODE), ECHO_POLL)
./debug/%.o: ./source/%.c   
	@echo 'Building file: $<'
	$(CC) $(CC_OPTIONS) $(B_OPTIONS) -DECHO_POLL -DUSE_UART_INTERRUPTS=0 $(INCLUDES) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<" 
	@echo 'Finished building: NORMAL mode $<'
	@echo ' '

else ifeq ($(MODE), ECHO_INT)
./debug/%.o: ./source/%.c   
	@echo 'Building file: $<'
	$(CC) $(CC_OPTIONS) $(B_OPTIONS) -DECHO_INT -DUSE_UART_INTERRUPTS=1 $(INCLUDES) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<" 
	@echo 'Finished building: DEBUG mode $<'
	@echo ' '
	
else ifeq ($(MODE), APP_POLL)
./debug/%.o: ./source/%.c   
	@echo 'Building file: $<'
	$(CC) $(CC_OPTIONS) $(B_OPTIONS) -DAPP_POLL -DAPP -DUSE_UART_INTERRUPTS=0 $(INCLUDES) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<" 
	@echo 'Finished building: DEBUG mode $<'
	@echo ' '
	
else ifeq ($(MODE), APP_INT)
./debug/%.o: ./source/%.c   
	@echo 'Building file: $<'
	$(CC) $(CC_OPTIONS) $(B_OPTIONS) -DAPP_INT -DAPP -DUSE_UART_INTERRUPTS=1 $(INCLUDES) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<" 
	@echo 'Finished building: DEBUG mode $<'
	@echo ' '
	
else ifeq ($(MODE), TEST)	
################################
# List of object files for TEST
OBJS := ./debug/PES_Project_5.o \
	./debug/logger.o \
	./debug/circular_buffer.o \
	./debug/uart.o \
	./debug/systick.o \
	./debug/led.o \
	./debug/unit_test.o \
	./debug/System.o \
	./debug/startup_mkl25z4.o \
	./debug/system_MKL25Z4.o	\
	./debug/board.o \
	./debug/clock_config.o \
	./debug/peripherals.o \
	./debug/pin_mux.o \
	./debug/fsl_clock.o \
	./debug/fsl_common.o \
	./debug/fsl_flash.o \
	./debug/fsl_gpio.o \
	./debug/fsl_lpsci.o \
	./debug/fsl_smc.o \
	./debug/fsl_uart.o ./debug/fsl_debug_console.o
	
############################
# List of dependency files 
C_DEPS = \
	./debug/PES_Project_5.d \
	./debug/logger.d \
	./debug/circular_buffer.d \
	./debug/uart.d \
	./debug/systick.d \
	./debug/led.d \
	./debug/unit_test.d \
	./debug/System.d \
	./debug/startup_mkl25z4.d \
	./debug/system_MKL25Z4.d \
	./debug/board.d \
	./debug/clock_config.d \
	./debug/peripherals.d \
	./debug/pin_mux.d \
	./debug/fsl_clock.d \
	./debug/fsl_common.d \
	./debug/fsl_flash.d \
	./debug/fsl_gpio.d \
	./debug/fsl_lpsci.d \
	./debug/fsl_smc.d \
	./debug/fsl_uart.d ./debug/fsl_debug_console.d
	
./debug/%.o: ./source/%.c   
	@echo 'Building file: $<'
	$(CC) $(CC_OPTIONS) $(B_OPTIONS) -DTEST -DUSE_UART_INTERRUPTS=1 $(INCLUDES) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<" 
	@echo 'Finished building: DEBUG mode $<'
	@echo ' '	
	
endif

############################
# Main (all) target
all: $(EXE)
	@echo "*** finished building ***"

############################
# Clean target
clean:
	-$(RM) $(EXECUTABLES) $(OBJS) $(EXE) 
	-$(RM) ./debug/*.map
	-@echo "*******Cleaned!*******"

############################
# Rule to link the executable
$(EXE): $(OBJS) linkerfile.ld
	@echo 'Building target: $@'
	@echo 'Invoking: Linker'
	$(LL) $(LL_OPTIONS) $(OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

#############################################
# Rule to build the files in the CMSIS folder
./debug/%.o: ./CMSIS/%.c
	@echo 'Building file: $<'
	$(CC) $(CC_OPTIONS) $(B_OPTIONS) $(INCLUDES) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

#############################################
# Rule to build the files in the board folder
./debug/%.o: ./board/%.c
	@echo 'Building file: $<'
	$(CC) $(CC_OPTIONS) $(B_OPTIONS) $(INCLUDES) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
	
###############################################
# Rule to build the files in the drivers folder
./debug/%.o: ./drivers/%.c
	@echo 'Building file: $<'
	$(CC) $(CC_OPTIONS) $(B_OPTIONS) $(INCLUDES) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

################################################
# Rule to build the files in the utilities folder
./debug/%.o: ./utilities/%.c
	@echo 'Building file: $<'
	$(CC) $(CC_OPTIONS) $(B_OPTIONS) $(INCLUDES) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

###############################################
# Rule to build the files in the startup folder
./debug/%.o: ./startup/%.c
	@echo 'Building file: $<'
	$(CC) $(CC_OPTIONS) $(B_OPTIONS) $(INCLUDES) -o $@ $<
	@echo 'Finished building: $<'
	@echo ' '

############################ References ################################
# [1] https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html
########################################################################