################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../imgui/ImGuizmo.cpp \
../imgui/imgui.cpp \
../imgui/imgui_draw.cpp \
../imgui/imgui_impl_opengl3.cpp \
../imgui/imgui_impl_sdl.cpp \
../imgui/imgui_impl_vulkan.cpp \
../imgui/imgui_neo_internal.cpp \
../imgui/imgui_neo_sequencer.cpp \
../imgui/imgui_tables.cpp \
../imgui/imgui_widgets.cpp 

CPP_DEPS += \
./imgui/ImGuizmo.d \
./imgui/imgui.d \
./imgui/imgui_draw.d \
./imgui/imgui_impl_opengl3.d \
./imgui/imgui_impl_sdl.d \
./imgui/imgui_impl_vulkan.d \
./imgui/imgui_neo_internal.d \
./imgui/imgui_neo_sequencer.d \
./imgui/imgui_tables.d \
./imgui/imgui_widgets.d 

OBJS += \
./imgui/ImGuizmo.o \
./imgui/imgui.o \
./imgui/imgui_draw.o \
./imgui/imgui_impl_opengl3.o \
./imgui/imgui_impl_sdl.o \
./imgui/imgui_impl_vulkan.o \
./imgui/imgui_neo_internal.o \
./imgui/imgui_neo_sequencer.o \
./imgui/imgui_tables.o \
./imgui/imgui_widgets.o 


# Each subdirectory must supply rules for building sources it contributes
imgui/%.o: ../imgui/%.cpp imgui/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/mnt/hentai/engine_workspace/engine/include" -I"/mnt/hentai/engine_workspace/engine/imgui" -O3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-imgui

clean-imgui:
	-$(RM) ./imgui/ImGuizmo.d ./imgui/ImGuizmo.o ./imgui/imgui.d ./imgui/imgui.o ./imgui/imgui_draw.d ./imgui/imgui_draw.o ./imgui/imgui_impl_opengl3.d ./imgui/imgui_impl_opengl3.o ./imgui/imgui_impl_sdl.d ./imgui/imgui_impl_sdl.o ./imgui/imgui_impl_vulkan.d ./imgui/imgui_impl_vulkan.o ./imgui/imgui_neo_internal.d ./imgui/imgui_neo_internal.o ./imgui/imgui_neo_sequencer.d ./imgui/imgui_neo_sequencer.o ./imgui/imgui_tables.d ./imgui/imgui_tables.o ./imgui/imgui_widgets.d ./imgui/imgui_widgets.o

.PHONY: clean-imgui

