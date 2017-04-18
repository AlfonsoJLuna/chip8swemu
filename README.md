# chip8swemu

Multiplatform CHIP-8 and Super-CHIP emulator written in C.

Get the latest release: https://github.com/AlfonsoJLuna/chip8swemu/releases

## Features

* Compatible with CHIP-8 and Super-CHIP programs.
* Cross-platform support through [SDL2](http://libsdl.org). Tested on Windows, should work on Linux and macOS with minor or no changes.
* Graphical user interface using [imgui](https://github.com/ocornut/imgui).
* Native file selection dialog on Windows. On other platforms, roms can be passed as an argument or dropped to the window.
* Configurable instructions per second rate, 840Hz by default.
* Configurable color palette.

The emulator core (`chip8.h`, `chip8.c`) is written as an easily reusable library that can be included in other projects, with no other dependencies than the standard C99 libraries. The [chip8stm32](https://github.com/AlfonsoJLuna/chip8stm32) game console uses it.

## Screenshots

![logo](/screenshots/logo.png)

![car](/screenshots/car.png)

![ant](/screenshots/ant.png)

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

## Known issues

* 8XY6/8XYE and FX55/FX65 instructions behavior in the Super-CHIP interpreter differs from the original CHIP-8. This emulator uses the Super-CHIP approach because it is the expected by most games, but this should be configurable for better compatibility (Animal Race requires the old behavior in order to display the animal sprites correctly).
* Vertical wrap is always enabled. Some games (Lunar Lander for CHIP-8, for example) require that, but others (Blitz for CHIP-8, Mines for Super-CHIP) don't work properly if the screen wraps vertically. This should be configurable for better compatibility.
* There are some other weird quirks not considered here because they also differ between original interpreters and no known game depends on them. These are well documented [here](https://github.com/Chromatophore/HP48-Superchip).

## Dependencies

chip8swemu uses the following libraries:
- [SDL 2.0.5](http://libsdl.org)
- [imgui 1.49](https://github.com/ocornut/imgui)

## Building on Windows

1. [Download](http://tdm-gcc.tdragon.net/download) and install the TDM-GCC compiler suite. Be sure you check `Add to PATH` during installation.
2. Clone or download this repository
3. [Download](https://www.libsdl.org/download-2.0.php) SDL 2.0.5 development libraries for MinGW and extract them to `chip8swemu/libraries/SDL2-2.0.5/...`
4. [Download](https://github.com/ocornut/imgui/releases) imgui 1.49 source code and place it in `chip8swemu/libraries/imgui-1.49/...`
6. Open a command prompt in `chip8swemu/source/` and type: `mingw32-make`
