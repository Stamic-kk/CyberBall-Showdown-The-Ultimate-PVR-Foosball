################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c \
../src/stm32f0xx_it.c \
../src/syscalls.c \
../src/system_stm32f0xx.c 

OBJS += \
./src/main.o \
./src/stm32f0xx_it.o \
./src/syscalls.o \
./src/system_stm32f0xx.o 

C_DEPS += \
./src/main.d \
./src/stm32f0xx_it.d \
./src/syscalls.d \
./src/system_stm32f0xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F0 -DSTM32F091RCTx -DDEBUG -DSTM32F091xC -DUSE_HAL_DRIVER -I"U:/foosball/CyberBall-Showdown-The-Ultimate-PVR-Foosball/foosball/HAL_Driver/Inc/Legacy" -I"C:/ac6/SystemWorkbench/plugins/STM32F0xx_StdPeriph_Lib_V1.5.0" -I"U:/foosball/CyberBall-Showdown-The-Ultimate-PVR-Foosball/foosball/inc" -I"U:/foosball/CyberBall-Showdown-The-Ultimate-PVR-Foosball/foosball/CMSIS/device" -I"U:/foosball/CyberBall-Showdown-The-Ultimate-PVR-Foosball/foosball/CMSIS/core" -I"U:/foosball/CyberBall-Showdown-The-Ultimate-PVR-Foosball/foosball/HAL_Driver/Inc" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


