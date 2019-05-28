# chip8swemu

*Multiplatform CHIP-8 and Super-CHIP emulator.*

chip8swemu is a CHIP-8 and Super-CHIP emulator focused on portability. The emulator core is written in standard C99 as a library with no other dependencies. It can be easily included in any project adding the `chip8.h` and `chip8.c` files and can run in almost any machine.

## Platform: Desktop

![frontend-1](/images/frontend-1.png) | ![frontend-2](/images/frontend-2.png) | ![frontend-3](/images/frontend-3.png)
:-----------: | :-------------: | :-------------:
| | | |

### Features

* Cross-platform (Windows XP+/Linux) frontend based on [SDL2](http://libsdl.org) and [ImGui](https://github.com/ocornut/imgui).
* ROMs can be passed as an argument or dropped to the window.
* Native file selection dialog (only available on Windows).
* Configurable CPU clock rate, 840Hz by default.
* Configurable color palette.
* Compatibility settings.

## Platform: STM32

It also has a custom PCB called chip8stm32 based on a STM32F103 (ARM Cortex-M3) microcontroller allowing you to play your CHIP-8 games on the go.

![Core-STM32](/images/core-stm32.jpg) | ![chip8stm32](/images/chip8stm32.jpg)
:-------------: | :-------------:
Blue Pill (STM32F103 + SH1106 OLED Display) | Custom PCB (chip8stm32)

Video: https://www.youtube.com/watch?v=vA76s3j4H90

### Game gallery

![Splash screen](/images/1.jpg) | ![Menu](/images/2.jpg) | ![SuperWorm V4](/images/3.jpg)
:-----------: | :-------------: | :-------------:
Splash screen | Menu | SuperWorm V4
![Space Invaders](/images/4.jpg) | ![Car](/images/5.jpg) | ![Blinky](/images/6.jpg)
Space Invaders | Car | Blinky
![Cave](/images/7.jpg) | ![Blitz](/images/8.jpg) | ![Brix](/images/9.jpg)
Cave | Blitz | Brix
![Pong](/images/10.jpg) | ![Super Astro Dodge](/images/11.jpg) | ![Tetris](/images/12.jpg)
Pong | Super Astro Dodge | Tetris

## Compatibility settings

Some games require special settings to run properly, you need to change the options before loading the game to take effect.

Known games that require the `Compatibility Mode` *enabled*:
* Animal Race [Brian Astle].ch8

Known games that require the `Vertical Wrap` *disabled*:
* Blitz [David Winter].ch8
* Mines! - The minehunter [David Winter, 1997].ch8

There are some other quirks not implemented because they also differ between original interpreters and no known game depends on them. These are well documented [here](https://github.com/Chromatophore/HP48-Superchip).

## References

Useful resources and documentation for developing a CHIP-8 emulator:

- [CHIP-8 article on Wikipedia](https://en.wikipedia.org/wiki/CHIP-8)
- [Mastering CHIP-8](http://mattmik.com/files/chip8/mastering/chip8.html)
- [Cowgod's CHIP-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
- [How to write an emulator (CHIP-8 interpreter)](http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/)
- [Mastering SuperChip](https://github.com/JohnEarnest/Octo/blob/gh-pages/docs/SuperChip.md)
- [Super-CHIP Documentation](https://github.com/Chromatophore/HP48-Superchip)
- [EmuDev on Reddit](https://www.reddit.com/r/EmuDev/)
