################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/engine/core/math/math.cpp \
../src/engine/core/math/matrix.cpp \
../src/engine/core/math/vector.cpp 

CPP_DEPS += \
./src/engine/core/math/math.d \
./src/engine/core/math/matrix.d \
./src/engine/core/math/vector.d 

OBJS += \
./src/engine/core/math/math.o \
./src/engine/core/math/matrix.o \
./src/engine/core/math/vector.o 


# Each subdirectory must supply rules for building sources it contributes
src/engine/core/math/%.o: ../src/engine/core/math/%.cpp src/engine/core/math/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/mnt/hentai/engine_workspace/engine/include" -I"/mnt/hentai/engine_workspace/engine/imgui" -O3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-engine-2f-core-2f-math

clean-src-2f-engine-2f-core-2f-math:
	-$(RM) ./src/engine/core/math/math.d ./src/engine/core/math/math.o ./src/engine/core/math/matrix.d ./src/engine/core/math/matrix.o ./src/engine/core/math/vector.d ./src/engine/core/math/vector.o

.PHONY: clean-src-2f-engine-2f-core-2f-math

