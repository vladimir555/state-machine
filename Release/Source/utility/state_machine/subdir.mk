################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Source/utility/state_machine/state_machine.cpp 

OBJS += \
./Source/utility/state_machine/state_machine.o 

CPP_DEPS += \
./Source/utility/state_machine/state_machine.d 


# Each subdirectory must supply rules for building sources it contributes
Source/utility/state_machine/%.o: ../Source/utility/state_machine/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/volodja/workspace/eclipse/state_machine_demo/Source" -O2 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


