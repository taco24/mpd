################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/motionplatform.o 

C_SRCS += \
../src/colomboard.c \
../src/log.c \
../src/modeltest.c \
../src/motionplatform.c \
../src/settings.c \
../src/thread.c \
../src/xplane.c 

OBJS += \
./src/colomboard.o \
./src/log.o \
./src/modeltest.o \
./src/motionplatform.o \
./src/settings.o \
./src/thread.o \
./src/xplane.o 

C_DEPS += \
./src/colomboard.d \
./src/log.d \
./src/modeltest.d \
./src/motionplatform.d \
./src/settings.d \
./src/thread.d \
./src/xplane.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D-DAPL=0 -D LIN=1 -D IBM=0 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


