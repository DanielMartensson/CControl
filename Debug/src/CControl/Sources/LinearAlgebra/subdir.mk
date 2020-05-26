################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/CControl/Sources/LinearAlgebra/balanace.c \
../src/CControl/Sources/LinearAlgebra/chol.c \
../src/CControl/Sources/LinearAlgebra/det.c \
../src/CControl/Sources/LinearAlgebra/dlyap.c \
../src/CControl/Sources/LinearAlgebra/eig.c \
../src/CControl/Sources/LinearAlgebra/eig_sym.c \
../src/CControl/Sources/LinearAlgebra/hankel.c \
../src/CControl/Sources/LinearAlgebra/inv.c \
../src/CControl/Sources/LinearAlgebra/linsolve_lower_triangular.c \
../src/CControl/Sources/LinearAlgebra/linsolve_lup.c \
../src/CControl/Sources/LinearAlgebra/linsolve_upper_triangular.c \
../src/CControl/Sources/LinearAlgebra/lup.c \
../src/CControl/Sources/LinearAlgebra/mul.c \
../src/CControl/Sources/LinearAlgebra/pinv.c \
../src/CControl/Sources/LinearAlgebra/qr.c \
../src/CControl/Sources/LinearAlgebra/svd_golub_reinsch.c \
../src/CControl/Sources/LinearAlgebra/svd_jacobi_one_sided.c \
../src/CControl/Sources/LinearAlgebra/tikhonov.c \
../src/CControl/Sources/LinearAlgebra/tran.c \
../src/CControl/Sources/LinearAlgebra/triu.c 

OBJS += \
./src/CControl/Sources/LinearAlgebra/balanace.o \
./src/CControl/Sources/LinearAlgebra/chol.o \
./src/CControl/Sources/LinearAlgebra/det.o \
./src/CControl/Sources/LinearAlgebra/dlyap.o \
./src/CControl/Sources/LinearAlgebra/eig.o \
./src/CControl/Sources/LinearAlgebra/eig_sym.o \
./src/CControl/Sources/LinearAlgebra/hankel.o \
./src/CControl/Sources/LinearAlgebra/inv.o \
./src/CControl/Sources/LinearAlgebra/linsolve_lower_triangular.o \
./src/CControl/Sources/LinearAlgebra/linsolve_lup.o \
./src/CControl/Sources/LinearAlgebra/linsolve_upper_triangular.o \
./src/CControl/Sources/LinearAlgebra/lup.o \
./src/CControl/Sources/LinearAlgebra/mul.o \
./src/CControl/Sources/LinearAlgebra/pinv.o \
./src/CControl/Sources/LinearAlgebra/qr.o \
./src/CControl/Sources/LinearAlgebra/svd_golub_reinsch.o \
./src/CControl/Sources/LinearAlgebra/svd_jacobi_one_sided.o \
./src/CControl/Sources/LinearAlgebra/tikhonov.o \
./src/CControl/Sources/LinearAlgebra/tran.o \
./src/CControl/Sources/LinearAlgebra/triu.o 

C_DEPS += \
./src/CControl/Sources/LinearAlgebra/balanace.d \
./src/CControl/Sources/LinearAlgebra/chol.d \
./src/CControl/Sources/LinearAlgebra/det.d \
./src/CControl/Sources/LinearAlgebra/dlyap.d \
./src/CControl/Sources/LinearAlgebra/eig.d \
./src/CControl/Sources/LinearAlgebra/eig_sym.d \
./src/CControl/Sources/LinearAlgebra/hankel.d \
./src/CControl/Sources/LinearAlgebra/inv.d \
./src/CControl/Sources/LinearAlgebra/linsolve_lower_triangular.d \
./src/CControl/Sources/LinearAlgebra/linsolve_lup.d \
./src/CControl/Sources/LinearAlgebra/linsolve_upper_triangular.d \
./src/CControl/Sources/LinearAlgebra/lup.d \
./src/CControl/Sources/LinearAlgebra/mul.d \
./src/CControl/Sources/LinearAlgebra/pinv.d \
./src/CControl/Sources/LinearAlgebra/qr.d \
./src/CControl/Sources/LinearAlgebra/svd_golub_reinsch.d \
./src/CControl/Sources/LinearAlgebra/svd_jacobi_one_sided.d \
./src/CControl/Sources/LinearAlgebra/tikhonov.d \
./src/CControl/Sources/LinearAlgebra/tran.d \
./src/CControl/Sources/LinearAlgebra/triu.d 


# Each subdirectory must supply rules for building sources it contributes
src/CControl/Sources/LinearAlgebra/%.o: ../src/CControl/Sources/LinearAlgebra/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


