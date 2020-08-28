################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/LCD_ILI9341/LCDCore/LCD_ILI9341.c \
../Core/Src/LCD_ILI9341/LCDCore/TSC2046.c 

OBJS += \
./Core/Src/LCD_ILI9341/LCDCore/LCD_ILI9341.o \
./Core/Src/LCD_ILI9341/LCDCore/TSC2046.o 

C_DEPS += \
./Core/Src/LCD_ILI9341/LCDCore/LCD_ILI9341.d \
./Core/Src/LCD_ILI9341/LCDCore/TSC2046.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/LCD_ILI9341/LCDCore/LCD_ILI9341.o: ../Core/Src/LCD_ILI9341/LCDCore/LCD_ILI9341.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/LCD_ILI9341/LCDCore/LCD_ILI9341.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/LCD_ILI9341/LCDCore/TSC2046.o: ../Core/Src/LCD_ILI9341/LCDCore/TSC2046.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/LCD_ILI9341/LCDCore/TSC2046.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

