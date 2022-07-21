################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/engine/render_vk/framebuffer.cpp \
../src/engine/render_vk/pipeline.cpp \
../src/engine/render_vk/renderer.cpp \
../src/engine/render_vk/shader.cpp \
../src/engine/render_vk/texture.cpp \
../src/engine/render_vk/vertexbuffer.cpp 

CPP_DEPS += \
./src/engine/render_vk/framebuffer.d \
./src/engine/render_vk/pipeline.d \
./src/engine/render_vk/renderer.d \
./src/engine/render_vk/shader.d \
./src/engine/render_vk/texture.d \
./src/engine/render_vk/vertexbuffer.d 

OBJS += \
./src/engine/render_vk/framebuffer.o \
./src/engine/render_vk/pipeline.o \
./src/engine/render_vk/renderer.o \
./src/engine/render_vk/shader.o \
./src/engine/render_vk/texture.o \
./src/engine/render_vk/vertexbuffer.o 


# Each subdirectory must supply rules for building sources it contributes
src/engine/render_vk/%.o: ../src/engine/render_vk/%.cpp src/engine/render_vk/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/mnt/hentai/engine_workspace/engine/include" -I"/mnt/hentai/engine_workspace/engine/imgui" -O3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-engine-2f-render_vk

clean-src-2f-engine-2f-render_vk:
	-$(RM) ./src/engine/render_vk/framebuffer.d ./src/engine/render_vk/framebuffer.o ./src/engine/render_vk/pipeline.d ./src/engine/render_vk/pipeline.o ./src/engine/render_vk/renderer.d ./src/engine/render_vk/renderer.o ./src/engine/render_vk/shader.d ./src/engine/render_vk/shader.o ./src/engine/render_vk/texture.d ./src/engine/render_vk/texture.o ./src/engine/render_vk/vertexbuffer.d ./src/engine/render_vk/vertexbuffer.o

.PHONY: clean-src-2f-engine-2f-render_vk

