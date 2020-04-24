################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/CControl/Sources/Miscellaneous/cut.c \
../src/CControl/Sources/Miscellaneous/insert.c \
../src/CControl/Sources/Miscellaneous/movingaverage.c \
../src/CControl/Sources/Miscellaneous/print.c \
../src/CControl/Sources/Miscellaneous/saturation.c \
../src/CControl/Sources/Miscellaneous/sign.c 

OBJS += \
./src/CControl/Sources/Miscellaneous/cut.o \
./src/CControl/Sources/Miscellaneous/insert.o \
./src/CControl/Sources/Miscellaneous/movingaverage.o \
./src/CControl/Sources/Miscellaneous/print.o \
./src/CControl/Sources/Miscellaneous/saturation.o \
./src/CControl/Sources/Miscellaneous/sign.o 

C_DEPS += \
./src/CControl/Sources/Miscellaneous/cut.d \
./src/CControl/Sources/Miscellaneous/insert.d \
./src/CControl/Sources/Miscellaneous/movingaverage.d \
./src/CControl/Sources/Miscellaneous/print.d \
./src/CControl/Sources/Miscellaneous/saturation.d \
./src/CControl/Sources/Miscellaneous/sign.d 


# Each subdirectory must supply rules for building sources it contributes
src/CControl/Sources/Miscellaneous/%.o: ../src/CControl/Sources/Miscellaneous/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


