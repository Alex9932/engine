################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/engine/sound/buffer.cpp \
../src/engine/sound/sound.cpp \
../src/engine/sound/sound_loader.cpp \
../src/engine/sound/source.cpp 

CPP_DEPS += \
./src/engine/sound/buffer.d \
./src/engine/sound/sound.d \
./src/engine/sound/sound_loader.d \
./src/engine/sound/source.d 

OBJS += \
./src/engine/sound/buffer.o \
./src/engine/sound/sound.o \
./src/engine/sound/sound_loader.o \
./src/engine/sound/source.o 


# Each subdirectory must supply rules for building sources it contributes
src/engine/sound/%.o: ../src/engine/sound/%.cpp src/engine/sound/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/mnt/hentai/engine_workspace/engine/include" -I"/mnt/hentai/engine_workspace/engine/imgui" -O3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-engine-2f-sound

clean-src-2f-engine-2f-sound:
	-$(RM) ./src/engine/sound/buffer.d ./src/engine/sound/buffer.o ./src/engine/sound/sound.d ./src/engine/sound/sound.o ./src/engine/sound/sound_loader.d ./src/engine/sound/sound_loader.o ./src/engine/sound/source.d ./src/engine/sound/source.o

.PHONY: clean-src-2f-engine-2f-sound

