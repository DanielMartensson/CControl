################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/CControl/Sources/SystemIdentification/subdir.mk
-include src/CControl/Sources/Optimization/subdir.mk
-include src/CControl/Sources/Miscellaneous/subdir.mk
-include src/CControl/Sources/LinearAlgebra/subdir.mk
-include src/CControl/Sources/ControlEngineering/subdir.mk
-include src/CControl/Sources/Classification/subdir.mk
-include src/CControl/Sources/ArtificialIntelligence/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: CControl

# Tool invocations
CControl: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc  -o "CControl" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) CControl
	-@echo ' '

.PHONY: all clean dependents

-include ../makefile.targets
