################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/DW1000/platform/Src/deca_mutex.c \
../Drivers/DW1000/platform/Src/deca_range_tables.c \
../Drivers/DW1000/platform/Src/deca_sleep.c \
../Drivers/DW1000/platform/Src/deca_spi.c \
../Drivers/DW1000/platform/Src/port.c 

OBJS += \
./Drivers/DW1000/platform/Src/deca_mutex.o \
./Drivers/DW1000/platform/Src/deca_range_tables.o \
./Drivers/DW1000/platform/Src/deca_sleep.o \
./Drivers/DW1000/platform/Src/deca_spi.o \
./Drivers/DW1000/platform/Src/port.o 

C_DEPS += \
./Drivers/DW1000/platform/Src/deca_mutex.d \
./Drivers/DW1000/platform/Src/deca_range_tables.d \
./Drivers/DW1000/platform/Src/deca_sleep.d \
./Drivers/DW1000/platform/Src/deca_spi.d \
./Drivers/DW1000/platform/Src/port.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/DW1000/platform/Src/%.o Drivers/DW1000/platform/Src/%.su Drivers/DW1000/platform/Src/%.cyclo: ../Drivers/DW1000/platform/Src/%.c Drivers/DW1000/platform/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/DW1000/compiler -I../Drivers/DW1000/decadriver/Inc -I../Drivers/DW1000/platform/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-DW1000-2f-platform-2f-Src

clean-Drivers-2f-DW1000-2f-platform-2f-Src:
	-$(RM) ./Drivers/DW1000/platform/Src/deca_mutex.cyclo ./Drivers/DW1000/platform/Src/deca_mutex.d ./Drivers/DW1000/platform/Src/deca_mutex.o ./Drivers/DW1000/platform/Src/deca_mutex.su ./Drivers/DW1000/platform/Src/deca_range_tables.cyclo ./Drivers/DW1000/platform/Src/deca_range_tables.d ./Drivers/DW1000/platform/Src/deca_range_tables.o ./Drivers/DW1000/platform/Src/deca_range_tables.su ./Drivers/DW1000/platform/Src/deca_sleep.cyclo ./Drivers/DW1000/platform/Src/deca_sleep.d ./Drivers/DW1000/platform/Src/deca_sleep.o ./Drivers/DW1000/platform/Src/deca_sleep.su ./Drivers/DW1000/platform/Src/deca_spi.cyclo ./Drivers/DW1000/platform/Src/deca_spi.d ./Drivers/DW1000/platform/Src/deca_spi.o ./Drivers/DW1000/platform/Src/deca_spi.su ./Drivers/DW1000/platform/Src/port.cyclo ./Drivers/DW1000/platform/Src/port.d ./Drivers/DW1000/platform/Src/port.o ./Drivers/DW1000/platform/Src/port.su

.PHONY: clean-Drivers-2f-DW1000-2f-platform-2f-Src

