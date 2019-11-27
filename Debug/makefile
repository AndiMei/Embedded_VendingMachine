################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Vending_Machine.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Vending_Machine.list \


# All Target
all: Vending_Machine.elf secondary-outputs

# Tool invocations
Vending_Machine.elf: $(OBJS) $(USER_OBJS) C:\Users\Andi\STM32CubeIDE\workspace_1.0.2\Vending_Machine\STM32F103C8TX_FLASH.ld
	arm-none-eabi-gcc -o "Vending_Machine.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"C:\Users\Andi\STM32CubeIDE\workspace_1.0.2\Vending_Machine\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Vending_Machine.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Vending_Machine.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Vending_Machine.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
