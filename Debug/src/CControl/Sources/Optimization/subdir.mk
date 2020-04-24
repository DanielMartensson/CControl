################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/CControl/Sources/Optimization/linprog.c 

OBJS += \
./src/CControl/Sources/Optimization/linprog.o 

C_DEPS += \
./src/CControl/Sources/Optimization/linprog.d 


# Each subdirectory must supply rules for building sources it contributes
src/CControl/Sources/Optimization/%.o: ../src/CControl/Sources/Optimization/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


