################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
floorDMX328dimmable.lss \

FLASH_IMAGE += \
floorDMX328dimmable.hex \

EEPROM_IMAGE += \
floorDMX328dimmable.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: floorDMX328dimmable.elf secondary-outputs

# Tool invocations
floorDMX328dimmable.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,floorDMX328dimmable.map -mmcu=atmega328p -o"floorDMX328dimmable.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

floorDMX328dimmable.lss: floorDMX328dimmable.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S floorDMX328dimmable.elf  >"floorDMX328dimmable.lss"
	@echo 'Finished building: $@'
	@echo ' '

floorDMX328dimmable.hex: floorDMX328dimmable.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex floorDMX328dimmable.elf  "floorDMX328dimmable.hex"
	@echo 'Finished building: $@'
	@echo ' '

floorDMX328dimmable.eep: floorDMX328dimmable.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex floorDMX328dimmable.elf  "floorDMX328dimmable.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: floorDMX328dimmable.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328p floorDMX328dimmable.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) floorDMX328dimmable.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
