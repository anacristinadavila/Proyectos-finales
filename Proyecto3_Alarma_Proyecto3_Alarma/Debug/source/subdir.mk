################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/Proyecto3_Alarma_Proyecto3_Alarma.c \
../source/keypad.c \
../source/mtb.c \
../source/semihost_hardfault.c 

OBJS += \
./source/Proyecto3_Alarma_Proyecto3_Alarma.o \
./source/keypad.o \
./source/mtb.o \
./source/semihost_hardfault.o 

C_DEPS += \
./source/Proyecto3_Alarma_Proyecto3_Alarma.d \
./source/keypad.d \
./source/mtb.d \
./source/semihost_hardfault.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL25Z128VLK4_cm0plus -DCPU_MKL25Z128VLK4 -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\Users\User\Desktop\Proyecto3_Alarma_Proyecto3_Alarma\board" -I"C:\Users\User\Desktop\Proyecto3_Alarma_Proyecto3_Alarma\source" -I"C:\Users\User\Desktop\Proyecto3_Alarma_Proyecto3_Alarma" -I"C:\Users\User\Desktop\Proyecto3_Alarma_Proyecto3_Alarma\drivers" -I"C:\Users\User\Desktop\Proyecto3_Alarma_Proyecto3_Alarma\CMSIS" -I"C:\Users\User\Desktop\Proyecto3_Alarma_Proyecto3_Alarma\utilities" -I"C:\Users\User\Desktop\Proyecto3_Alarma_Proyecto3_Alarma\startup" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


