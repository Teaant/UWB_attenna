################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/DW1000/decadriver/Src/deca_device.c \
../Drivers/DW1000/decadriver/Src/deca_params_init.c 

OBJS += \
./Drivers/DW1000/decadriver/Src/deca_device.o \
./Drivers/DW1000/decadriver/Src/deca_params_init.o 

C_DEPS += \
./Drivers/DW1000/decadriver/Src/deca_device.d \
./Drivers/DW1000/decadriver/Src/deca_params_init.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/DW1000/decadriver/Src/%.o Drivers/DW1000/decadriver/Src/%.su Drivers/DW1000/decadriver/Src/%.cyclo: ../Drivers/DW1000/decadriver/Src/%.c Drivers/DW1000/decadriver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/DW1000/compiler -I../Drivers/DW1000/decadriver/Inc -I../Drivers/DW1000/platform/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-DW1000-2f-decadriver-2f-Src

clean-Drivers-2f-DW1000-2f-decadriver-2f-Src:
	-$(RM) ./Drivers/DW1000/decadriver/Src/deca_device.cyclo ./Drivers/DW1000/decadriver/Src/deca_device.d ./Drivers/DW1000/decadriver/Src/deca_device.o ./Drivers/DW1000/decadriver/Src/deca_device.su ./Drivers/DW1000/decadriver/Src/deca_params_init.cyclo ./Drivers/DW1000/decadriver/Src/deca_params_init.d ./Drivers/DW1000/decadriver/Src/deca_params_init.o ./Drivers/DW1000/decadriver/Src/deca_params_init.su

.PHONY: clean-Drivers-2f-DW1000-2f-decadriver-2f-Src

