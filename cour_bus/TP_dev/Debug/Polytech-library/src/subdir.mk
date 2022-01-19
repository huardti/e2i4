################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Polytech-library/src/Adc.c \
../Polytech-library/src/Clocks.c \
../Polytech-library/src/Port_conf.c \
../Polytech-library/src/Spi.c \
../Polytech-library/src/Timer.c \
../Polytech-library/src/Uart.c \
../Polytech-library/src/code_eleve.c 

OBJS += \
./Polytech-library/src/Adc.o \
./Polytech-library/src/Clocks.o \
./Polytech-library/src/Port_conf.o \
./Polytech-library/src/Spi.o \
./Polytech-library/src/Timer.o \
./Polytech-library/src/Uart.o \
./Polytech-library/src/code_eleve.o 

C_DEPS += \
./Polytech-library/src/Adc.d \
./Polytech-library/src/Clocks.d \
./Polytech-library/src/Port_conf.d \
./Polytech-library/src/Spi.d \
./Polytech-library/src/Timer.d \
./Polytech-library/src/Uart.d \
./Polytech-library/src/code_eleve.d 


# Each subdirectory must supply rules for building sources it contributes
Polytech-library/src/%.o: ../Polytech-library/src/%.c Polytech-library/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -D__FPU_PRESENT -DSTM32F446xx -c -I../src -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../src -I../cmsis_lib/include -I../Polytech-library/inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Polytech-2d-library-2f-src

clean-Polytech-2d-library-2f-src:
	-$(RM) ./Polytech-library/src/Adc.d ./Polytech-library/src/Adc.o ./Polytech-library/src/Clocks.d ./Polytech-library/src/Clocks.o ./Polytech-library/src/Port_conf.d ./Polytech-library/src/Port_conf.o ./Polytech-library/src/Spi.d ./Polytech-library/src/Spi.o ./Polytech-library/src/Timer.d ./Polytech-library/src/Timer.o ./Polytech-library/src/Uart.d ./Polytech-library/src/Uart.o ./Polytech-library/src/code_eleve.d ./Polytech-library/src/code_eleve.o

.PHONY: clean-Polytech-2d-library-2f-src

