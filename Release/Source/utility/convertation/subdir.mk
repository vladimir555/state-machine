################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Source/utility/convertation/convertation.cpp 

OBJS += \
./Source/utility/convertation/convertation.o 

CPP_DEPS += \
./Source/utility/convertation/convertation.d 


# Each subdirectory must supply rules for building sources it contributes
Source/utility/convertation/%.o: ../Source/utility/convertation/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/volodja/workspace/eclipse/state_machine_demo/Source" -O2 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


