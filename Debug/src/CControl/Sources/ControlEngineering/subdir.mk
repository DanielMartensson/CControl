################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/CControl/Sources/ControlEngineering/autotuning.c \
../src/CControl/Sources/ControlEngineering/cab.c \
../src/CControl/Sources/ControlEngineering/gpc.c \
../src/CControl/Sources/ControlEngineering/integral.c \
../src/CControl/Sources/ControlEngineering/kalman.c \
../src/CControl/Sources/ControlEngineering/lqi.c \
../src/CControl/Sources/ControlEngineering/mpc.c \
../src/CControl/Sources/ControlEngineering/mrac.c \
../src/CControl/Sources/ControlEngineering/obsv.c \
../src/CControl/Sources/ControlEngineering/stability.c \
../src/CControl/Sources/ControlEngineering/theta2ss.c 

OBJS += \
./src/CControl/Sources/ControlEngineering/autotuning.o \
./src/CControl/Sources/ControlEngineering/cab.o \
./src/CControl/Sources/ControlEngineering/gpc.o \
./src/CControl/Sources/ControlEngineering/integral.o \
./src/CControl/Sources/ControlEngineering/kalman.o \
./src/CControl/Sources/ControlEngineering/lqi.o \
./src/CControl/Sources/ControlEngineering/mpc.o \
./src/CControl/Sources/ControlEngineering/mrac.o \
./src/CControl/Sources/ControlEngineering/obsv.o \
./src/CControl/Sources/ControlEngineering/stability.o \
./src/CControl/Sources/ControlEngineering/theta2ss.o 

C_DEPS += \
./src/CControl/Sources/ControlEngineering/autotuning.d \
./src/CControl/Sources/ControlEngineering/cab.d \
./src/CControl/Sources/ControlEngineering/gpc.d \
./src/CControl/Sources/ControlEngineering/integral.d \
./src/CControl/Sources/ControlEngineering/kalman.d \
./src/CControl/Sources/ControlEngineering/lqi.d \
./src/CControl/Sources/ControlEngineering/mpc.d \
./src/CControl/Sources/ControlEngineering/mrac.d \
./src/CControl/Sources/ControlEngineering/obsv.d \
./src/CControl/Sources/ControlEngineering/stability.d \
./src/CControl/Sources/ControlEngineering/theta2ss.d 


# Each subdirectory must supply rules for building sources it contributes
src/CControl/Sources/ControlEngineering/%.o: ../src/CControl/Sources/ControlEngineering/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


