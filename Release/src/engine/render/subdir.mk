################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/engine/render/console.cpp \
../src/engine/render/font.cpp \
../src/engine/render/fontrenderer.cpp \
../src/engine/render/material.cpp \
../src/engine/render/mesh.cpp \
../src/engine/render/model.cpp \
../src/engine/render/overlay.cpp \
../src/engine/render/render.cpp \
../src/engine/render/render2d.cpp \
../src/engine/render/shader.cpp \
../src/engine/render/window.cpp 

CPP_DEPS += \
./src/engine/render/console.d \
./src/engine/render/font.d \
./src/engine/render/fontrenderer.d \
./src/engine/render/material.d \
./src/engine/render/mesh.d \
./src/engine/render/model.d \
./src/engine/render/overlay.d \
./src/engine/render/render.d \
./src/engine/render/render2d.d \
./src/engine/render/shader.d \
./src/engine/render/window.d 

OBJS += \
./src/engine/render/console.o \
./src/engine/render/font.o \
./src/engine/render/fontrenderer.o \
./src/engine/render/material.o \
./src/engine/render/mesh.o \
./src/engine/render/model.o \
./src/engine/render/overlay.o \
./src/engine/render/render.o \
./src/engine/render/render2d.o \
./src/engine/render/shader.o \
./src/engine/render/window.o 


# Each subdirectory must supply rules for building sources it contributes
src/engine/render/%.o: ../src/engine/render/%.cpp src/engine/render/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/mnt/hentai/engine_workspace/engine/include" -I"/mnt/hentai/engine_workspace/engine/imgui" -O3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-engine-2f-render

clean-src-2f-engine-2f-render:
	-$(RM) ./src/engine/render/console.d ./src/engine/render/console.o ./src/engine/render/font.d ./src/engine/render/font.o ./src/engine/render/fontrenderer.d ./src/engine/render/fontrenderer.o ./src/engine/render/material.d ./src/engine/render/material.o ./src/engine/render/mesh.d ./src/engine/render/mesh.o ./src/engine/render/model.d ./src/engine/render/model.o ./src/engine/render/overlay.d ./src/engine/render/overlay.o ./src/engine/render/render.d ./src/engine/render/render.o ./src/engine/render/render2d.d ./src/engine/render/render2d.o ./src/engine/render/shader.d ./src/engine/render/shader.o ./src/engine/render/window.d ./src/engine/render/window.o

.PHONY: clean-src-2f-engine-2f-render

