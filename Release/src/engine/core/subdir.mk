################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/engine/core/allocator.cpp \
../src/engine/core/ccdik.cpp \
../src/engine/core/event.cpp \
../src/engine/core/filesystem.cpp \
../src/engine/core/geometry.cpp \
../src/engine/core/input.cpp \
../src/engine/core/loader.cpp \
../src/engine/core/logger.cpp \
../src/engine/core/profiler.cpp \
../src/engine/core/skeleton.cpp \
../src/engine/core/strings.cpp \
../src/engine/core/timer.cpp \
../src/engine/core/utf8.cpp \
../src/engine/core/utils.cpp 

CPP_DEPS += \
./src/engine/core/allocator.d \
./src/engine/core/ccdik.d \
./src/engine/core/event.d \
./src/engine/core/filesystem.d \
./src/engine/core/geometry.d \
./src/engine/core/input.d \
./src/engine/core/loader.d \
./src/engine/core/logger.d \
./src/engine/core/profiler.d \
./src/engine/core/skeleton.d \
./src/engine/core/strings.d \
./src/engine/core/timer.d \
./src/engine/core/utf8.d \
./src/engine/core/utils.d 

OBJS += \
./src/engine/core/allocator.o \
./src/engine/core/ccdik.o \
./src/engine/core/event.o \
./src/engine/core/filesystem.o \
./src/engine/core/geometry.o \
./src/engine/core/input.o \
./src/engine/core/loader.o \
./src/engine/core/logger.o \
./src/engine/core/profiler.o \
./src/engine/core/skeleton.o \
./src/engine/core/strings.o \
./src/engine/core/timer.o \
./src/engine/core/utf8.o \
./src/engine/core/utils.o 


# Each subdirectory must supply rules for building sources it contributes
src/engine/core/%.o: ../src/engine/core/%.cpp src/engine/core/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/mnt/hentai/engine_workspace/engine/include" -I"/mnt/hentai/engine_workspace/engine/imgui" -O3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-engine-2f-core

clean-src-2f-engine-2f-core:
	-$(RM) ./src/engine/core/allocator.d ./src/engine/core/allocator.o ./src/engine/core/ccdik.d ./src/engine/core/ccdik.o ./src/engine/core/event.d ./src/engine/core/event.o ./src/engine/core/filesystem.d ./src/engine/core/filesystem.o ./src/engine/core/geometry.d ./src/engine/core/geometry.o ./src/engine/core/input.d ./src/engine/core/input.o ./src/engine/core/loader.d ./src/engine/core/loader.o ./src/engine/core/logger.d ./src/engine/core/logger.o ./src/engine/core/profiler.d ./src/engine/core/profiler.o ./src/engine/core/skeleton.d ./src/engine/core/skeleton.o ./src/engine/core/strings.d ./src/engine/core/strings.o ./src/engine/core/timer.d ./src/engine/core/timer.o ./src/engine/core/utf8.d ./src/engine/core/utf8.o ./src/engine/core/utils.d ./src/engine/core/utils.o

.PHONY: clean-src-2f-engine-2f-core

