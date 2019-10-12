################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Adaptive\ Model\ Predictive\ Control/Sources/Algebra.c \
../src/Adaptive\ Model\ Predictive\ Control/Sources/Filtration.c \
../src/Adaptive\ Model\ Predictive\ Control/Sources/Identification.c \
../src/Adaptive\ Model\ Predictive\ Control/Sources/Prediction.c 

OBJS += \
./src/Adaptive\ Model\ Predictive\ Control/Sources/Algebra.o \
./src/Adaptive\ Model\ Predictive\ Control/Sources/Filtration.o \
./src/Adaptive\ Model\ Predictive\ Control/Sources/Identification.o \
./src/Adaptive\ Model\ Predictive\ Control/Sources/Prediction.o 

C_DEPS += \
./src/Adaptive\ Model\ Predictive\ Control/Sources/Algebra.d \
./src/Adaptive\ Model\ Predictive\ Control/Sources/Filtration.d \
./src/Adaptive\ Model\ Predictive\ Control/Sources/Identification.d \
./src/Adaptive\ Model\ Predictive\ Control/Sources/Prediction.d 


# Each subdirectory must supply rules for building sources it contributes
src/Adaptive\ Model\ Predictive\ Control/Sources/Algebra.o: ../src/Adaptive\ Model\ Predictive\ Control/Sources/Algebra.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -std=c11 -O2 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/Adaptive Model Predictive Control/Sources/Algebra.d" -MT"src/Adaptive\ Model\ Predictive\ Control/Sources/Algebra.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/Adaptive\ Model\ Predictive\ Control/Sources/Filtration.o: ../src/Adaptive\ Model\ Predictive\ Control/Sources/Filtration.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -std=c11 -O2 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/Adaptive Model Predictive Control/Sources/Filtration.d" -MT"src/Adaptive\ Model\ Predictive\ Control/Sources/Filtration.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/Adaptive\ Model\ Predictive\ Control/Sources/Identification.o: ../src/Adaptive\ Model\ Predictive\ Control/Sources/Identification.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -std=c11 -O2 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/Adaptive Model Predictive Control/Sources/Identification.d" -MT"src/Adaptive\ Model\ Predictive\ Control/Sources/Identification.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/Adaptive\ Model\ Predictive\ Control/Sources/Prediction.o: ../src/Adaptive\ Model\ Predictive\ Control/Sources/Prediction.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -std=c11 -O2 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/Adaptive Model Predictive Control/Sources/Prediction.d" -MT"src/Adaptive\ Model\ Predictive\ Control/Sources/Prediction.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


