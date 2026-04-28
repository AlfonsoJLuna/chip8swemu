# STM32 build system

# Target processor selection (STM32H7B0VBT6)
MCU_GCC      := cortex-m7
MCU_HAL      := STM32H7B0xx
MCU_STARTUP  := stm32h7b0xx
MCU_LDSCRIPT := STM32H7B0VBTx

# Linkerscript
LDSCRIPT := libraries/game-and-watch-base/$(MCU_LDSCRIPT)_FLASH.ld

# Library sources
S_FILES += libraries/cmsis-device-h7/Source/Templates/gcc/startup_$(MCU_STARTUP).s
C_FILES += libraries/cmsis-device-h7/Source/Templates/system_stm32h7xx.c
C_FILES += libraries/stm32h7xx-hal-driver/Src/stm32h7xx_hal_cortex.c
C_FILES += libraries/stm32h7xx-hal-driver/Src/stm32h7xx_hal_ltdc.c
C_FILES += libraries/stm32h7xx-hal-driver/Src/stm32h7xx_hal_ltdc_ex.c
C_FILES += libraries/stm32h7xx-hal-driver/Src/stm32h7xx_hal_rcc.c
C_FILES += libraries/stm32h7xx-hal-driver/Src/stm32h7xx_hal_rcc_ex.c
C_FILES += libraries/stm32h7xx-hal-driver/Src/stm32h7xx_hal_flash.c
C_FILES += libraries/stm32h7xx-hal-driver/Src/stm32h7xx_hal_flash_ex.c
C_FILES += libraries/stm32h7xx-hal-driver/Src/stm32h7xx_hal_gpio.c
C_FILES += libraries/stm32h7xx-hal-driver/Src/stm32h7xx_hal_hsem.c
C_FILES += libraries/stm32h7xx-hal-driver/Src/stm32h7xx_hal_dma.c
C_FILES += libraries/stm32h7xx-hal-driver/Src/stm32h7xx_hal_dma_ex.c
C_FILES += libraries/stm32h7xx-hal-driver/Src/stm32h7xx_hal_mdma.c
C_FILES += libraries/stm32h7xx-hal-driver/Src/stm32h7xx_hal_pwr.c
C_FILES += libraries/stm32h7xx-hal-driver/Src/stm32h7xx_hal_pwr_ex.c
C_FILES += libraries/stm32h7xx-hal-driver/Src/stm32h7xx_hal.c
C_FILES += libraries/stm32h7xx-hal-driver/Src/stm32h7xx_hal_i2c.c
C_FILES += libraries/stm32h7xx-hal-driver/Src/stm32h7xx_hal_i2c_ex.c
C_FILES += libraries/stm32h7xx-hal-driver/Src/stm32h7xx_hal_exti.c
C_FILES += libraries/stm32h7xx-hal-driver/Src/stm32h7xx_hal_tim.c
C_FILES += libraries/stm32h7xx-hal-driver/Src/stm32h7xx_hal_tim_ex.c
C_FILES += libraries/stm32h7xx-hal-driver/Src/stm32h7xx_hal_spi.c
C_FILES += libraries/stm32h7xx-hal-driver/Src/stm32h7xx_hal_spi_ex.c
C_FILES += libraries/stm32h7xx-hal-driver/Src/stm32h7xx_hal_ospi.c
C_FILES += libraries/stm32h7xx-hal-driver/Src/stm32h7xx_hal_sai.c
C_FILES += libraries/stm32h7xx-hal-driver/Src/stm32h7xx_hal_sai_ex.c
C_FILES += libraries/game-and-watch-base/Core/Src/buttons.c
C_FILES += libraries/game-and-watch-base/Core/Src/flash.c
C_FILES += libraries/game-and-watch-base/Core/Src/lcd.c
C_FILES += libraries/game-and-watch-base/Core/Src/stm32h7xx_hal_msp.c
C_FILES += libraries/game-and-watch-base/Core/Src/stm32h7xx_it.c

# Applicaton sources
C_FILES += ../../core/source/chip8.c
C_FILES += $(wildcard source/*.c)

# Needed objects
OBJECTS += $(addprefix objects/,$(notdir $(C_FILES:.c=.o)))
OBJECTS += $(addprefix objects/,$(notdir $(S_FILES:.s=.o)))

# Include search paths
INCS += -Ilibraries/cmsis-core/Include
INCS += -Ilibraries/cmsis-device-h7/Include
INCS += -Ilibraries/stm32h7xx-hal-driver/Inc
INCS += -Ilibraries/game-and-watch-base/Core/Inc
INCS += -I../../core/source
INCS += -I../../core/assets/logo_rom
INCS += -Isource

# Compiler flags
CFLAGS += -mcpu=$(MCU_GCC) -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard
CFLAGS += -std=c99 -ffunction-sections -fdata-sections -Os
CFLAGS += -Wall -Wextra -Wno-unused-parameter -Wstack-usage=256
CFLAGS += -Wno-error=implicit-function-declaration
DEFINE += -D$(MCU_HAL) -DUSE_HAL_DRIVER

# Linker flags
LFLAGS += -Wl,--gc-sections -Wl,-Map=build/binary.map -specs=nano.specs -specs=nosys.specs
LFLAGS += -Wl,-T$(LDSCRIPT)

# Rules
all: build/binary.hex build/binary.bin

build/binary.hex: build/binary.elf
	arm-none-eabi-objcopy -O ihex build/binary.elf build/binary.hex

build/binary.bin: build/binary.elf
	arm-none-eabi-objcopy -O binary -S build/binary.elf build/binary.bin

build/binary.elf: $(OBJECTS)
	arm-none-eabi-gcc $^ $(CFLAGS) $(LFLAGS) -o $@

objects/%.o: libraries/cmsis-device-h7/Source/Templates/gcc/%.s
	arm-none-eabi-gcc $(CFLAGS) -MMD -c $(DEFINE) $(INCS) $< -o $@

objects/%.o: libraries/cmsis-device-h7/Source/Templates/%.c
	arm-none-eabi-gcc $(CFLAGS) -MMD -c $(DEFINE) $(INCS) $< -o $@

objects/%.o: libraries/stm32h7xx-hal-driver/Src/%.c
	arm-none-eabi-gcc $(CFLAGS) -MMD -c $(DEFINE) $(INCS) $< -o $@

objects/%.o: ../../core/source/%.c
	arm-none-eabi-gcc $(CFLAGS) -MMD -c $(DEFINE) $(INCS) $< -o $@

objects/%.o: libraries/game-and-watch-base/Core/Src/%.c
	arm-none-eabi-gcc $(CFLAGS) -MMD -c $(DEFINE) $(INCS) $< -o $@

objects/%.o: source/%.c
	arm-none-eabi-gcc $(CFLAGS) -MMD -c $(DEFINE) $(INCS) $< -o $@

-include $(OBJECTS:.o=.d)

flash: all
	dd if=build/binary.bin of=build/binary_flash.bin bs=1024 count=128
	openocd -f interface/stlink.cfg -c "transport select hla_swd" -f "target/stm32h7x.cfg" -c "reset_config none; program build/binary_flash.bin 0x08000000 verify reset exit"

debug: all
	openocd -f interface/stlink.cfg -f "target/stm32h7x.cfg"
	arm-none-eabi-gdb -ex "target remote localhost:3333" ./build/binary.elf
