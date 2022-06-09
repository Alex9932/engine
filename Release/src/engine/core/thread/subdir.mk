################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/engine/core/thread/thread.cpp \
../src/engine/core/thread/worker.cpp 

CPP_DEPS += \
./src/engine/core/thread/thread.d \
./src/engine/core/thread/worker.d 

OBJS += \
./src/engine/core/thread/thread.o \
./src/engine/core/thread/worker.o 


# Each subdirectory must supply rules for building sources it contributes
src/engine/core/thread/%.o: ../src/engine/core/thread/%.cpp src/engine/core/thread/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/mnt/hentai/engine_workspace/engine/include" -I"/mnt/hentai/engine_workspace/engine/imgui" -O3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-engine-2f-core-2f-thread

clean-src-2f-engine-2f-core-2f-thread:
	-$(RM) ./src/engine/core/thread/thread.d ./src/engine/core/thread/thread.o ./src/engine/core/thread/worker.d ./src/engine/core/thread/worker.o

.PHONY: clean-src-2f-engine-2f-core-2f-thread

