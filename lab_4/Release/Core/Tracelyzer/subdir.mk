################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Tracelyzer/trcKernelPort.c \
../Core/Tracelyzer/trcSnapshotRecorder.c \
../Core/Tracelyzer/trcStreamingRecorder.c 

OBJS += \
./Core/Tracelyzer/trcKernelPort.o \
./Core/Tracelyzer/trcSnapshotRecorder.o \
./Core/Tracelyzer/trcStreamingRecorder.o 

C_DEPS += \
./Core/Tracelyzer/trcKernelPort.d \
./Core/Tracelyzer/trcSnapshotRecorder.d \
./Core/Tracelyzer/trcStreamingRecorder.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Tracelyzer/%.o Core/Tracelyzer/%.su: ../Core/Tracelyzer/%.c Core/Tracelyzer/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Tracelyzer

clean-Core-2f-Tracelyzer:
	-$(RM) ./Core/Tracelyzer/trcKernelPort.d ./Core/Tracelyzer/trcKernelPort.o ./Core/Tracelyzer/trcKernelPort.su ./Core/Tracelyzer/trcSnapshotRecorder.d ./Core/Tracelyzer/trcSnapshotRecorder.o ./Core/Tracelyzer/trcSnapshotRecorder.su ./Core/Tracelyzer/trcStreamingRecorder.d ./Core/Tracelyzer/trcStreamingRecorder.o ./Core/Tracelyzer/trcStreamingRecorder.su

.PHONY: clean-Core-2f-Tracelyzer

