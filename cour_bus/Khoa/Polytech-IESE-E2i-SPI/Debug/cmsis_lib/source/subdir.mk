################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../cmsis_lib/source/misc.c \
../cmsis_lib/source/stm32f4xx_adc.c \
../cmsis_lib/source/stm32f4xx_crc.c \
../cmsis_lib/source/stm32f4xx_dma.c \
../cmsis_lib/source/stm32f4xx_exti.c \
../cmsis_lib/source/stm32f4xx_gpio.c \
../cmsis_lib/source/stm32f4xx_i2c.c \
../cmsis_lib/source/stm32f4xx_pwr.c \
../cmsis_lib/source/stm32f4xx_rcc.c \
../cmsis_lib/source/stm32f4xx_spi.c \
../cmsis_lib/source/stm32f4xx_syscfg.c \
../cmsis_lib/source/stm32f4xx_tim.c 

OBJS += \
./cmsis_lib/source/misc.o \
./cmsis_lib/source/stm32f4xx_adc.o \
./cmsis_lib/source/stm32f4xx_crc.o \
./cmsis_lib/source/stm32f4xx_dma.o \
./cmsis_lib/source/stm32f4xx_exti.o \
./cmsis_lib/source/stm32f4xx_gpio.o \
./cmsis_lib/source/stm32f4xx_i2c.o \
./cmsis_lib/source/stm32f4xx_pwr.o \
./cmsis_lib/source/stm32f4xx_rcc.o \
./cmsis_lib/source/stm32f4xx_spi.o \
./cmsis_lib/source/stm32f4xx_syscfg.o \
./cmsis_lib/source/stm32f4xx_tim.o 

C_DEPS += \
./cmsis_lib/source/misc.d \
./cmsis_lib/source/stm32f4xx_adc.d \
./cmsis_lib/source/stm32f4xx_crc.d \
./cmsis_lib/source/stm32f4xx_dma.d \
./cmsis_lib/source/stm32f4xx_exti.d \
./cmsis_lib/source/stm32f4xx_gpio.d \
./cmsis_lib/source/stm32f4xx_i2c.d \
./cmsis_lib/source/stm32f4xx_pwr.d \
./cmsis_lib/source/stm32f4xx_rcc.d \
./cmsis_lib/source/stm32f4xx_spi.d \
./cmsis_lib/source/stm32f4xx_syscfg.d \
./cmsis_lib/source/stm32f4xx_tim.d 


# Each subdirectory must supply rules for building sources it contributes
cmsis_lib/source/%.o: ../cmsis_lib/source/%.c
	arm-none-eabi-gcc -c "$<" -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -std=gnu11 -D__FPU_PRESENT -DSTM32F446xx -I"../src" -I"../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../Drivers/CMSIS/Include" -I"../src" -I"../cmsis_lib/include" -I"../Polytech-library/inc" -O0 -ffunction-sections -fdata-sections -g -fstack-usage -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -specs=nano.specs -o "$@"

