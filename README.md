# chip8swemu

Multiplatform CHIP-8 emulator written in C

## Screenshots

![breakout](/screenshots/breakout.png)

![tetris](/screenshots/tetris.png)

![invaders](/screenshots/invaders.png)

## Resources

- [CHIP-8 article on Wikipedia](https://en.wikipedia.org/wiki/CHIP-8)
- [Cowgod's CHIP-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
- [How to write an emulator (CHIP-8 interpreter)](http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/)
- [Complete collection of all known CHIP-8 games](http://www.chip8.com/?page=109): Some of these programs are included in the release for testing the emulator. Those programs are public domain or freely distributed by their respective authors.

## Dependencies

chip8swemu uses the following libraries:
- [SDL 2.0.4](http://libsdl.org)
- [imgui 1.49](https://github.com/ocornut/imgui)

## Building on Windows

1. Install MinGW and add its bin folder to your `PATH` system variable
2. Clone or download this repository
3. [Download](https://www.libsdl.org/download-2.0.php) SDL2 development libraries for MinGW and extract them to `chip8swemu/libraries/SDL2-2.0.4/...`
4. [Download](https://github.com/ocornut/imgui/releases) imgui source code and place it in `chip8swemu/libraries/imgui/`
6. Open a command prompt and type: `mingw32-make`

