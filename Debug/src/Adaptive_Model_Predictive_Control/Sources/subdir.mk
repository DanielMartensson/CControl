################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Adaptive_Model_Predictive_Control/Sources/Algebra.c \
../src/Adaptive_Model_Predictive_Control/Sources/Filtration.c \
../src/Adaptive_Model_Predictive_Control/Sources/Identification.c \
../src/Adaptive_Model_Predictive_Control/Sources/Prediction.c \
../src/Adaptive_Model_Predictive_Control/Sources/Tools.c 

OBJS += \
./src/Adaptive_Model_Predictive_Control/Sources/Algebra.o \
./src/Adaptive_Model_Predictive_Control/Sources/Filtration.o \
./src/Adaptive_Model_Predictive_Control/Sources/Identification.o \
./src/Adaptive_Model_Predictive_Control/Sources/Prediction.o \
./src/Adaptive_Model_Predictive_Control/Sources/Tools.o 

C_DEPS += \
./src/Adaptive_Model_Predictive_Control/Sources/Algebra.d \
./src/Adaptive_Model_Predictive_Control/Sources/Filtration.d \
./src/Adaptive_Model_Predictive_Control/Sources/Identification.d \
./src/Adaptive_Model_Predictive_Control/Sources/Prediction.d \
./src/Adaptive_Model_Predictive_Control/Sources/Tools.d 


# Each subdirectory must supply rules for building sources it contributes
src/Adaptive_Model_Predictive_Control/Sources/%.o: ../src/Adaptive_Model_Predictive_Control/Sources/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -std=c11 -O2 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


