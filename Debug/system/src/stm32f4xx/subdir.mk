################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/src/stm32f4xx/stm32f4xx_hal.c \
../system/src/stm32f4xx/stm32f4xx_hal_cortex.c \
../system/src/stm32f4xx/stm32f4xx_hal_gpio.c \
../system/src/stm32f4xx/stm32f4xx_hal_rcc.c 

OBJS += \
./system/src/stm32f4xx/stm32f4xx_hal.o \
./system/src/stm32f4xx/stm32f4xx_hal_cortex.o \
./system/src/stm32f4xx/stm32f4xx_hal_gpio.o \
./system/src/stm32f4xx/stm32f4xx_hal_rcc.o 

C_DEPS += \
./system/src/stm32f4xx/stm32f4xx_hal.d \
./system/src/stm32f4xx/stm32f4xx_hal_cortex.d \
./system/src/stm32f4xx/stm32f4xx_hal_gpio.d \
./system/src/stm32f4xx/stm32f4xx_hal_rcc.d 


# Each subdirectory must supply rules for building sources it contributes
system/src/stm32f4xx/%.o: ../system/src/stm32f4xx/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DSTM32F407xx -DTRACE -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4xx" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


