# STM32 build system

# Target processor selection
CPU := cortex-m3
MCU := STM32F103xB
STARTUP := stm32f103xb
LDSCRIPT := STM32F103XB

# Libraries files
S_FILES  = libraries/cmsis-device-f1/Source/Templates/gcc/startup_$(STARTUP).s
C_FILES  = $(wildcard libraries/cmsis-device-f1/Source/Templates/*.c)
C_FILES += $(filter-out %_template.c, $(wildcard libraries/stm32f1xx-hal-driver/Src/*.c))

# Source files
C_FILES += $(wildcard ../../core/source/*.c)
C_FILES += $(wildcard source/*.c)

# Needed objects
OBJECTS  = $(addprefix objects/,$(notdir $(C_FILES:.c=.o)))
OBJECTS += $(addprefix objects/,$(notdir $(S_FILES:.s=.o)))

# Include search paths
INCS  = -Ilibraries/cmsis-core/CMSIS/Core/Include
INCS += -Ilibraries/cmsis-device-f1/Include
INCS += -Ilibraries/stm32f1xx-hal-driver/Inc
INCS += -I../../core/source
INCS += -Isource

# GCC Flags
CFLAGS  = -mcpu=$(CPU) -mthumb -Wall -Wextra -std=c99 -ffunction-sections -fdata-sections -Os
LFLAGS  = -Wl,--gc-sections -Wl,-Map=build/binary.map
LFLAGS += -Wl,-Tlibraries/cmsis-device-f1/Source/Templates/gcc/linker/$(LDSCRIPT)_FLASH.ld
DEFINE  = -D$(MCU) -DUSE_HAL_DRIVER

# Rules
all: build/binary.hex

build/binary.hex: build/binary.elf
	arm-none-eabi-objcopy -O ihex build/binary.elf "build/binary.hex"

build/binary.elf: $(OBJECTS)
	arm-none-eabi-gcc $^ $(CFLAGS) $(LFLAGS) -o $@

objects/%.o: libraries/cmsis-device-f1/Source/Templates/gcc/%.s
	arm-none-eabi-gcc $(CFLAGS) -MMD -c $(DEFINE) $(INCS) $< -o $@

objects/%.o: libraries/cmsis-device-f1/Source/Templates/%.c
	arm-none-eabi-gcc $(CFLAGS) -MMD -c $(DEFINE) $(INCS) $< -o $@

objects/%.o: libraries/stm32f1xx-hal-driver/Src/%.c
	arm-none-eabi-gcc $(CFLAGS) -MMD -c $(DEFINE) $(INCS) $< -o $@

objects/%.o: ../../core/source/%.c
	arm-none-eabi-gcc $(CFLAGS) -MMD -c $(DEFINE) $(INCS) $< -o $@

objects/%.o: source/%.c
	arm-none-eabi-gcc $(CFLAGS) -MMD -c $(DEFINE) $(INCS) $< -o $@

-include $(OBJECTS:.o=.d)

flash: all
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg -c "program build/binary.hex verify reset exit"

debug: all
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg
	arm-none-eabi-gdb -ex "target remote localhost:3333" ./build/binary.elf
