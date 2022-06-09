################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/engine/engine.cpp 

CPP_DEPS += \
./src/engine/engine.d 

OBJS += \
./src/engine/engine.o 


# Each subdirectory must supply rules for building sources it contributes
src/engine/%.o: ../src/engine/%.cpp src/engine/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/mnt/hentai/engine_workspace/engine/include" -I"/mnt/hentai/engine_workspace/engine/imgui" -O3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-engine

clean-src-2f-engine:
	-$(RM) ./src/engine/engine.d ./src/engine/engine.o

.PHONY: clean-src-2f-engine

