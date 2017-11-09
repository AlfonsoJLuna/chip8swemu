# chip8swemu

Multiplatform CHIP-8 and Super-CHIP emulator written in C.

Get the latest release: https://github.com/AlfonsoJLuna/chip8swemu/releases

![frontend-logo](/images/frontend-logo.png)

## Features

* Compatible with CHIP-8 and Super-CHIP programs.
* Cross-platform support through [SDL2](http://libsdl.org).
* Cross-platform GUI using [ImGui](https://github.com/ocornut/imgui).
* Native file selection dialog on Windows.
* Roms can be passed as an argument or dropped to the window.
* Configurable instructions per second rate, 840Hz by default.
* Configurable color palette.
* Compatibility settings.

The emulator core (`chip8.h`, `chip8.c`) is written as a platform-independent library with no other dependencies than the standard C99 libraries, so it can run even on small microcontrollers:

**ATmega2560 (Arduino Mega) + KS0108 GLCD**

![core-arduino](/images/core-arduino.jpg)

**STM32F103 (Blue Pill) + SH1106 OLED Display**

![core-stm32](/images/core-stm32.jpg)

## Compatibility

Some games require special settings to run properly, you need to change the options before loading the game to take effect.

Known games that require the `Compatibility Mode` *enabled*:
* Animal Race [Brian Astle].ch8

Known games that require the `Vertical Wrap` *disabled*:
* Blitz [David Winter].ch8
* Mines! - The minehunter [David Winter, 1997].ch8

There are some other weird quirks not considered because they also differ between original interpreters and no known game depends on them. These are well documented [here](https://github.com/Chromatophore/HP48-Superchip).

## Building

The only required dependencies are SDL 2.0.5 and ImGui 1.52. I have tested on Windows, but should work on Linux and other OSs if you modify the Makefile as needed.

Instructions for building on Windows (64-bit):

1. [Install](https://git-scm.com/downloads) git and clone this repository.
2. [Install](http://tdm-gcc.tdragon.net/download) the TDM64-GCC compiler suite. Be sure you check `Add to PATH` during installation.
3. [Download](https://www.libsdl.org/download-2.0.php) SDL 2.0.5 libraries for MinGW and extract them to `chip8swemu/frontend/libraries/SDL2-2.0.5/...`
4. [Download](https://github.com/ocornut/imgui/releases) ImGui 1.52 source code and extract it to `chip8swemu/frontend/libraries/imgui-1.52/...`
6. Open a command prompt in `chip8swemu/frontend/` and type: `mingw32-make`.

## References

- [CHIP-8 article on Wikipedia](https://en.wikipedia.org/wiki/CHIP-8)
- [Mastering CHIP-8](http://mattmik.com/files/chip8/mastering/chip8.html)
- [Cowgod's CHIP-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
- [How to write an emulator (CHIP-8 interpreter)](http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/)
- [Mastering SuperChip](https://github.com/JohnEarnest/Octo/blob/gh-pages/docs/SuperChip.md)
- [Super-CHIP Documentation](https://github.com/Chromatophore/HP48-Superchip)
- [EmuDev on Reddit](https://www.reddit.com/r/EmuDev/)
