################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32.s 

OBJS += \
./startup/startup_stm32.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -I"U:/foosball/CyberBall-Showdown-The-Ultimate-PVR-Foosball/foosball/HAL_Driver/Inc/Legacy" -I"U:/foosball/CyberBall-Showdown-The-Ultimate-PVR-Foosball/foosball/inc" -I"U:/foosball/CyberBall-Showdown-The-Ultimate-PVR-Foosball/foosball/CMSIS/device" -I"U:/foosball/CyberBall-Showdown-The-Ultimate-PVR-Foosball/foosball/CMSIS/core" -I"U:/foosball/CyberBall-Showdown-The-Ultimate-PVR-Foosball/foosball/HAL_Driver/Inc" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


