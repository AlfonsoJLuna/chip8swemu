# chip8swemu

*Multiplatform CHIP-8 and Super-CHIP emulator.*

chip8swemu is a CHIP-8 and Super-CHIP emulator focused on portability. The emulator core is written in standard C99 as a library with no other dependencies. It can be easily included in any project adding the `chip8.h` and `chip8.c` files and can run in almost any machine.

It has a multiplatform frontend for the desktop (Windows/Linux) based on [SDL2](http://libsdl.org) and [ImGui](https://github.com/ocornut/imgui) featuring some fancy configuration options such as changing the color palette or the CPU clock rate (840Hz by default) on the fly.

![frontend-1](/images/frontend-1.png) | ![frontend-2](/images/frontend-2.png) | ![frontend-3](/images/frontend-3.png)
:-----------: | :-------------: | :-------------:
| | | |

It also has a custom PCB called chip8stm32 based on a STM32F103 (ARM Cortex-M3) microcontroller allowing you to play your CHIP-8 games on the go.

![chip8stm32](/images/chip8stm32.jpg)

Video: https://www.youtube.com/watch?v=vA76s3j4H90

## Prototypes

![Core-Arduino](/images/core-arduino.jpg) | ![Core-STM32](/images/core-stm32.jpg)
:-------------: | :-------------:
Arduino Mega (ATmega2560 + KS0108 GLCD) | Blue Pill (STM32F103 + SH1106 OLED Display)

## Game gallery

![Splash screen](/images/1.jpg) | ![Menu](/images/2.jpg) | ![SuperWorm V4](/images/3.jpg)
:-----------: | :-------------: | :-------------:
Splash screen | Menu | SuperWorm V4
![Space Invaders](/images/4.jpg) | ![Car](/images/5.jpg) | ![Blinky](/images/6.jpg)
Space Invaders | Car | Blinky
![Cave](/images/7.jpg) | ![Blitz](/images/8.jpg) | ![Brix](/images/9.jpg)
Cave | Blitz | Brix
![Pong](/images/10.jpg) | ![Super Astro Dodge](/images/11.jpg) | ![Tetris](/images/12.jpg)
Pong | Super Astro Dodge | Tetris

## Compatibility

Some games require special settings to run properly, you need to change the options before loading the game to take effect.

Known games that require the `Compatibility Mode` *enabled*:
* Animal Race [Brian Astle].ch8

Known games that require the `Vertical Wrap` *disabled*:
* Blitz [David Winter].ch8
* Mines! - The minehunter [David Winter, 1997].ch8

There are some other weird quirks not considered because they also differ between original interpreters and no known game depends on them. These are well documented [here](https://github.com/Chromatophore/HP48-Superchip).

## Desktop frontend building

Required dependencies are ImGui, SDL2, OpenGL.

### How to build on Windows

1. [Install](https://git-scm.com/downloads) git and clone this repository.
2. [Install](http://tdm-gcc.tdragon.net/download) the TDM64-GCC (64-bit) or TDM-GCC (32-bit) compiler suite. Be sure you check `Add to PATH` during installation.
3. [Download](https://www.libsdl.org/release/SDL2-devel-2.0.5-mingw.tar.gz) SDL 2.0.5 libraries for MinGW and extract them to `chip8swemu/frontend/libraries/SDL2-2.0.5/...`
4. [Download](https://github.com/ocornut/imgui/releases) ImGui 1.52 source code and extract it to `chip8swemu/frontend/libraries/imgui-1.52/...`
5. Open a command prompt in `chip8swemu/frontend/` and type: `mingw32-make`.

### How to build on Linux (Ubuntu)

1. Install git: `sudo apt-get install git`
2. Clone this repository: `git clone https://github.com/alfonsojluna/chip8swemu`
3. Install SDL2 development libraries: `sudo apt-get install libsdl2-dev`
4. [Download](https://github.com/ocornut/imgui/releases) ImGui 1.52 source code and extract it to `chip8swemu/frontend/libraries/imgui-1.52/...`
5. Open a terminal in `chip8swemu/frontend/` and type: `make`.

## Firmware building for chip8stm32

1. Add your games to the `games.h` header file. You can convert your games from binary format to a C array using xxd: `xxd -i game.ch8`.

**How to build:**

2. [Install](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads) the GNU ARM Embedded Toolchain compiler. Be sure you check `Add to PATH` during installation.
3. You will need make. On Windows you can [install](http://tdm-gcc.tdragon.net/download) the TDM-GCC compiler suite which includes mingw32-make. Be sure you check `Add to PATH` during installation.
4. [Install](http://www.st.com/en/embedded-software/stsw-link004.html) the STM32 ST-LINK Utility. You need to add its folder to PATH manually.
5. Clone or download this repository.
6. Open a command prompt in `chip8stm32/firmware/` and type `mingw32-make` for building.

**How to flash:**

7. Connect the board to the computer using a ST-LINK Programmer and type `mingw32-make flash` for flashing.

## References

Useful resources and documentation for developing a CHIP-8 emulator:

- [CHIP-8 article on Wikipedia](https://en.wikipedia.org/wiki/CHIP-8)
- [Mastering CHIP-8](http://mattmik.com/files/chip8/mastering/chip8.html)
- [Cowgod's CHIP-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
- [How to write an emulator (CHIP-8 interpreter)](http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/)
- [Mastering SuperChip](https://github.com/JohnEarnest/Octo/blob/gh-pages/docs/SuperChip.md)
- [Super-CHIP Documentation](https://github.com/Chromatophore/HP48-Superchip)
- [EmuDev on Reddit](https://www.reddit.com/r/EmuDev/)
