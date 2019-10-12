################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/CControl/Sources/Algebra.c \
../src/CControl/Sources/Estimation.c \
../src/CControl/Sources/Feedback.c \
../src/CControl/Sources/Identification.c \
../src/CControl/Sources/Tools.c 

OBJS += \
./src/CControl/Sources/Algebra.o \
./src/CControl/Sources/Estimation.o \
./src/CControl/Sources/Feedback.o \
./src/CControl/Sources/Identification.o \
./src/CControl/Sources/Tools.o 

C_DEPS += \
./src/CControl/Sources/Algebra.d \
./src/CControl/Sources/Estimation.d \
./src/CControl/Sources/Feedback.d \
./src/CControl/Sources/Identification.d \
./src/CControl/Sources/Tools.d 


# Each subdirectory must supply rules for building sources it contributes
src/CControl/Sources/%.o: ../src/CControl/Sources/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -std=c11 -O2 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


