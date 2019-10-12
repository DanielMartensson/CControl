################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/CControl/Documents/Examples/Feedback/Linear_Quadratic_Integral_Control/Linear_Quadratic_Integral_Control.c 

OBJS += \
./src/CControl/Documents/Examples/Feedback/Linear_Quadratic_Integral_Control/Linear_Quadratic_Integral_Control.o 

C_DEPS += \
./src/CControl/Documents/Examples/Feedback/Linear_Quadratic_Integral_Control/Linear_Quadratic_Integral_Control.d 


# Each subdirectory must supply rules for building sources it contributes
src/CControl/Documents/Examples/Feedback/Linear_Quadratic_Integral_Control/%.o: ../src/CControl/Documents/Examples/Feedback/Linear_Quadratic_Integral_Control/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -std=c11 -O2 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


