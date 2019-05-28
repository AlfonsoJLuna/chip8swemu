# STM32F1 build system by AlfonsoJLuna

# Target processor selection

CPU := cortex-m3
MCU := STM32F103xB
STARTUP := stm32f103xb
LDSCRIPT := STM32F103XB

# Libraries files

S_FILES  = libraries/CMSIS/Device/ST/STM32F1xx/Source/Templates/gcc/startup_$(STARTUP).s
C_FILES  = $(wildcard libraries/CMSIS/Device/ST/STM32F1xx/Source/Templates/*.c)
C_FILES += $(wildcard libraries/STM32F1xx_HAL_Driver/Src/*.c)

# Source files

C_FILES += $(wildcard source/*.c)

# Needed objects

OBJECTS  = $(addprefix objects/,$(notdir $(C_FILES:.c=.o)))
OBJECTS += $(addprefix objects/,$(notdir $(S_FILES:.s=.o)))

# Include search paths

INCS  = -Ilibraries/CMSIS/Include
INCS += -Ilibraries/CMSIS/Device/ST/STM32F1xx/Include
INCS += -Ilibraries/STM32F1xx_HAL_Driver/Inc
INCS += -Isource

# GCC Flags

CFLAGS  = -mcpu=$(CPU) -mthumb -Wall -Wextra -std=c99 -ffunction-sections -fdata-sections -Os
LFLAGS  = -Wl,--gc-sections -Wl,-Map=build/binary.map
LFLAGS += -Wl,-Tlibraries/CMSIS/Device/ST/STM32F1xx/Source/Templates/gcc/linker/$(LDSCRIPT)_FLASH.ld
DEFINE  = -D$(MCU) -DUSE_HAL_DRIVER

# Rules

all: build/binary.hex

build/binary.hex: build/binary.elf
	arm-none-eabi-objcopy -O ihex build/binary.elf "build/binary.hex"

build/binary.elf: $(OBJECTS)
	arm-none-eabi-gcc $^ $(CFLAGS) $(LFLAGS) -o $@

objects/%.o: libraries/CMSIS/Device/ST/STM32F1xx/Source/Templates/gcc/%.s
	arm-none-eabi-gcc $(CFLAGS) -MMD -c $(DEFINE) $(INCS) $< -o $@

objects/%.o: libraries/CMSIS/Device/ST/STM32F1xx/Source/Templates/%.c
	arm-none-eabi-gcc $(CFLAGS) -MMD -c $(DEFINE) $(INCS) $< -o $@

objects/%.o: libraries/STM32F1xx_HAL_Driver/Src/%.c
	arm-none-eabi-gcc $(CFLAGS) -MMD -c $(DEFINE) $(INCS) $< -o $@

objects/%.o: source/%.c
	arm-none-eabi-gcc $(CFLAGS) -MMD -c $(DEFINE) $(INCS) $< -o $@

-include $(OBJECTS:.o=.d)

flash: all
	ST-LINK_CLI -c -Q -P "build/binary.hex" -V "while_programming" -Run

debug: all
	start st-util
	start arm-none-eabi-gdb build/binary.elf -ex "target extended-remote localhost:4242"
