################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/computation/computation.c 

OBJS += \
./src/computation/computation.o 

C_DEPS += \
./src/computation/computation.d 


# Each subdirectory must supply rules for building sources it contributes
src/computation/%.o: ../src/computation/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D-DAPL=0 -D LIN=1 -D IBM=0 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


