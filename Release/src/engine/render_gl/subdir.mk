################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/engine/render_gl/framebuffer.cpp \
../src/engine/render_gl/pipeline.cpp \
../src/engine/render_gl/renderer.cpp \
../src/engine/render_gl/shader.cpp \
../src/engine/render_gl/texture.cpp \
../src/engine/render_gl/vertexbuffer.cpp 

CPP_DEPS += \
./src/engine/render_gl/framebuffer.d \
./src/engine/render_gl/pipeline.d \
./src/engine/render_gl/renderer.d \
./src/engine/render_gl/shader.d \
./src/engine/render_gl/texture.d \
./src/engine/render_gl/vertexbuffer.d 

OBJS += \
./src/engine/render_gl/framebuffer.o \
./src/engine/render_gl/pipeline.o \
./src/engine/render_gl/renderer.o \
./src/engine/render_gl/shader.o \
./src/engine/render_gl/texture.o \
./src/engine/render_gl/vertexbuffer.o 


# Each subdirectory must supply rules for building sources it contributes
src/engine/render_gl/%.o: ../src/engine/render_gl/%.cpp src/engine/render_gl/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/mnt/hentai/engine_workspace/engine/include" -I"/mnt/hentai/engine_workspace/engine/imgui" -O3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-engine-2f-render_gl

clean-src-2f-engine-2f-render_gl:
	-$(RM) ./src/engine/render_gl/framebuffer.d ./src/engine/render_gl/framebuffer.o ./src/engine/render_gl/pipeline.d ./src/engine/render_gl/pipeline.o ./src/engine/render_gl/renderer.d ./src/engine/render_gl/renderer.o ./src/engine/render_gl/shader.d ./src/engine/render_gl/shader.o ./src/engine/render_gl/texture.d ./src/engine/render_gl/texture.o ./src/engine/render_gl/vertexbuffer.d ./src/engine/render_gl/vertexbuffer.o

.PHONY: clean-src-2f-engine-2f-render_gl

