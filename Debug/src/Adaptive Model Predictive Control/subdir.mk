################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Adaptive\ Model\ Predictive\ Control/Adaptive_Model_Predictive_Control.c 

OBJS += \
./src/Adaptive\ Model\ Predictive\ Control/Adaptive_Model_Predictive_Control.o 

C_DEPS += \
./src/Adaptive\ Model\ Predictive\ Control/Adaptive_Model_Predictive_Control.d 


# Each subdirectory must supply rules for building sources it contributes
src/Adaptive\ Model\ Predictive\ Control/Adaptive_Model_Predictive_Control.o: ../src/Adaptive\ Model\ Predictive\ Control/Adaptive_Model_Predictive_Control.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -std=c11 -O2 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/Adaptive Model Predictive Control/Adaptive_Model_Predictive_Control.d" -MT"src/Adaptive\ Model\ Predictive\ Control/Adaptive_Model_Predictive_Control.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


