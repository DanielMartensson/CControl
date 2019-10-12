################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/qpOASES/Source/Bounds.c \
../src/qpOASES/Source/Constraints.c \
../src/qpOASES/Source/Flipper.c \
../src/qpOASES/Source/Indexlist.c \
../src/qpOASES/Source/Matrices.c \
../src/qpOASES/Source/MessageHandling.c \
../src/qpOASES/Source/OQPinterface.c \
../src/qpOASES/Source/Options.c \
../src/qpOASES/Source/QProblem.c \
../src/qpOASES/Source/QProblemB.c \
../src/qpOASES/Source/Utils.c 

OBJS += \
./src/qpOASES/Source/Bounds.o \
./src/qpOASES/Source/Constraints.o \
./src/qpOASES/Source/Flipper.o \
./src/qpOASES/Source/Indexlist.o \
./src/qpOASES/Source/Matrices.o \
./src/qpOASES/Source/MessageHandling.o \
./src/qpOASES/Source/OQPinterface.o \
./src/qpOASES/Source/Options.o \
./src/qpOASES/Source/QProblem.o \
./src/qpOASES/Source/QProblemB.o \
./src/qpOASES/Source/Utils.o 

C_DEPS += \
./src/qpOASES/Source/Bounds.d \
./src/qpOASES/Source/Constraints.d \
./src/qpOASES/Source/Flipper.d \
./src/qpOASES/Source/Indexlist.d \
./src/qpOASES/Source/Matrices.d \
./src/qpOASES/Source/MessageHandling.d \
./src/qpOASES/Source/OQPinterface.d \
./src/qpOASES/Source/Options.d \
./src/qpOASES/Source/QProblem.d \
./src/qpOASES/Source/QProblemB.d \
./src/qpOASES/Source/Utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/qpOASES/Source/%.o: ../src/qpOASES/Source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -std=c11 -O2 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


