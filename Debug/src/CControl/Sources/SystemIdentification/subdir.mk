################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/CControl/Sources/SystemIdentification/era.c \
../src/CControl/Sources/SystemIdentification/okid.c \
../src/CControl/Sources/SystemIdentification/rls.c 

OBJS += \
./src/CControl/Sources/SystemIdentification/era.o \
./src/CControl/Sources/SystemIdentification/okid.o \
./src/CControl/Sources/SystemIdentification/rls.o 

C_DEPS += \
./src/CControl/Sources/SystemIdentification/era.d \
./src/CControl/Sources/SystemIdentification/okid.d \
./src/CControl/Sources/SystemIdentification/rls.d 


# Each subdirectory must supply rules for building sources it contributes
src/CControl/Sources/SystemIdentification/%.o: ../src/CControl/Sources/SystemIdentification/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


