################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../debounce.c \
../dmx_controller.c \
../dmx_device_ledbar252.c \
../dmx_tx_driver.c 

OBJS += \
./debounce.o \
./dmx_controller.o \
./dmx_device_ledbar252.o \
./dmx_tx_driver.o 

C_DEPS += \
./debounce.d \
./dmx_controller.d \
./dmx_device_ledbar252.d \
./dmx_tx_driver.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=20000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


