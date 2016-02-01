################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Source/network/socket/socket_client.cpp \
../Source/network/socket/socket_server.cpp 

OBJS += \
./Source/network/socket/socket_client.o \
./Source/network/socket/socket_server.o 

CPP_DEPS += \
./Source/network/socket/socket_client.d \
./Source/network/socket/socket_server.d 


# Each subdirectory must supply rules for building sources it contributes
Source/network/socket/%.o: ../Source/network/socket/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/volodja/workspace/eclipse/state_machine_demo/Source" -O2 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


