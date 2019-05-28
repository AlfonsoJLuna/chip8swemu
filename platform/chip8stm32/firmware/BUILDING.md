## Building

1. Add your games to the `games.h` header file. You can convert your games from binary format to a C array using xxd: `xxd -i game.ch8`.

**How to build:**

2. [Install](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads) the GNU ARM Embedded Toolchain compiler. Be sure you check `Add to PATH` during installation.
3. You will need make. On Windows you can [install](http://tdm-gcc.tdragon.net/download) the TDM-GCC compiler suite which includes mingw32-make. Be sure you check `Add to PATH` during installation.
4. [Install](http://www.st.com/en/embedded-software/stsw-link004.html) the STM32 ST-LINK Utility. You need to add its folder to PATH manually.
5. Clone or download this repository.
6. Open a command prompt in `chip8stm32/firmware/` and type `mingw32-make` for building.

**How to flash:**

7. Connect the board to the computer using a ST-LINK Programmer and type `mingw32-make flash` for flashing.
