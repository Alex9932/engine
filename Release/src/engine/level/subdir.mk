################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/engine/level/camera.cpp \
../src/engine/level/entity.cpp \
../src/engine/level/level.cpp \
../src/engine/level/object.cpp 

CPP_DEPS += \
./src/engine/level/camera.d \
./src/engine/level/entity.d \
./src/engine/level/level.d \
./src/engine/level/object.d 

OBJS += \
./src/engine/level/camera.o \
./src/engine/level/entity.o \
./src/engine/level/level.o \
./src/engine/level/object.o 


# Each subdirectory must supply rules for building sources it contributes
src/engine/level/%.o: ../src/engine/level/%.cpp src/engine/level/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/mnt/hentai/engine_workspace/engine/include" -I"/mnt/hentai/engine_workspace/engine/imgui" -O3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-engine-2f-level

clean-src-2f-engine-2f-level:
	-$(RM) ./src/engine/level/camera.d ./src/engine/level/camera.o ./src/engine/level/entity.d ./src/engine/level/entity.o ./src/engine/level/level.d ./src/engine/level/level.o ./src/engine/level/object.d ./src/engine/level/object.o

.PHONY: clean-src-2f-engine-2f-level

