# chip8swemu

Multiplatform CHIP-8 and Super-CHIP emulator written in C.

Get the latest release: https://github.com/AlfonsoJLuna/chip8swemu/releases

## Core

The emulator core (`chip8.h`, `chip8.c`) is written as a platform-independent library that can be easily included in other projects, with no other dependencies than the standard C99 libraries. The [chip8stm32](https://github.com/AlfonsoJLuna/chip8stm32) game console uses it.

The minimum requirements are 6KB of RAM and a 128x64 display so it can run even on small microcontrollers:

**ATmega2560 (Arduino Mega) + KS0108 GLCD**

![core-arduino](/images/core-arduino.jpg)

**STM32F103 (Blue Pill) + SH1106 OLED Display**

![core-stm32](/images/core-stm32.jpg)

## Frontend

I also provide a frontend for the desktop based on [SDL2](http://libsdl.org) and [ImGui](https://github.com/ocornut/imgui). I have tested it on Windows, but should work on Linux and Mac OS X with minor or no changes.

Features:
* Native file selection dialog on Windows.
* Roms can be passed as an argument or dropped to the window.
* Configurable instructions per second rate, 840Hz by default.
* Configurable color palette.
* Compatibility Mode and configurable vertical wrap.

![frontend-logo](/images/frontend-logo.png)

![frontend-invaders](/images/frontend-invaders.png)

![frontend-car](/images/frontend-car.png)

## Building on Windows (64-bit)

1. Clone or download this repository.
2. [Download](http://tdm-gcc.tdragon.net/download) and install the TDM64-GCC compiler suite. Be sure you check `Add to PATH` during installation.
3. [Download](https://www.libsdl.org/download-2.0.php) SDL 2.0.5 development libraries for MinGW and extract them to `chip8swemu/frontend/libraries/SDL2-2.0.5/...`.
4. [Download](https://github.com/ocornut/imgui/releases) ImGui 1.51 source code and place it in `chip8swemu/frontend/libraries/imgui-1.51/...`.
6. Open a command prompt in `chip8swemu/frontend/` and type: `mingw32-make`.

## Documentation

CHIP-8 is an interpreted programming language designed to make easier the writing of simple games for some home computers based on the RCA CDP1802 processor in the late 1970s.

An extension of the CHIP-8 instruction set called Super-CHIP was introduced in 1991 initially for use on the HP48 graphing calculators.

For more information:
- [CHIP-8 article on Wikipedia](https://en.wikipedia.org/wiki/CHIP-8)
- [Mastering CHIP-8](http://mattmik.com/files/chip8/mastering/chip8.html)
- [Cowgod's CHIP-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
- [How to write an emulator (CHIP-8 interpreter)](http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/)
- [Mastering SuperChip](https://github.com/JohnEarnest/Octo/blob/gh-pages/docs/SuperChip.md)
- [Super-CHIP Documentation](https://github.com/Chromatophore/HP48-Superchip)

## Quirks

### Compatibility Mode

8XY6/8XYE and FX55/FX65 instructions behavior in the original Super-CHIP interpreter differs from the original CHIP-8. This emulator uses the Super-CHIP approach by default because is the expected by most games, but you can enable the Compatibility Mode if a game requires the old behavior. You need to change the option before loading the game to take effect.

List of known games that require the Compatibility Mode enabled:
* Animal Race [Brian Astle].ch8

### Vertical Wrap

Enabled by default because is the expected by most games, but there are games that don't work properly if the screen wraps vertically. You need to change the option before loading the game to take effect.

List of known games that require the Vertical Wrap disabled:
* Blitz [David Winter].ch8
* Mines! - The minehunter [David Winter, 1997].ch8

There are some other weird quirks not considered because they also differ between original interpreters and no known game depends on them. These are well documented [here](https://github.com/Chromatophore/HP48-Superchip).
