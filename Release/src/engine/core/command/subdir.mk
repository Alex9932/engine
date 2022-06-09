################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/engine/core/command/command.cpp 

CPP_DEPS += \
./src/engine/core/command/command.d 

OBJS += \
./src/engine/core/command/command.o 


# Each subdirectory must supply rules for building sources it contributes
src/engine/core/command/%.o: ../src/engine/core/command/%.cpp src/engine/core/command/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/mnt/hentai/engine_workspace/engine/include" -I"/mnt/hentai/engine_workspace/engine/imgui" -O3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-engine-2f-core-2f-command

clean-src-2f-engine-2f-core-2f-command:
	-$(RM) ./src/engine/core/command/command.d ./src/engine/core/command/command.o

.PHONY: clean-src-2f-engine-2f-core-2f-command

