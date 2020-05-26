################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/CControl/Sources/ArtificialIntelligence/Astar.c 

OBJS += \
./src/CControl/Sources/ArtificialIntelligence/Astar.o 

C_DEPS += \
./src/CControl/Sources/ArtificialIntelligence/Astar.d 


# Each subdirectory must supply rules for building sources it contributes
src/CControl/Sources/ArtificialIntelligence/%.o: ../src/CControl/Sources/ArtificialIntelligence/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


