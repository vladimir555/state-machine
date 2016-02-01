################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Source/network/service/service.cpp 

OBJS += \
./Source/network/service/service.o 

CPP_DEPS += \
./Source/network/service/service.d 


# Each subdirectory must supply rules for building sources it contributes
Source/network/service/%.o: ../Source/network/service/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/volodja/workspace/eclipse/state_machine_demo/Source" -O2 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


