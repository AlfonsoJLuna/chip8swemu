## Game & Watch

![](assets/gaw-1.jpg)

This is a quick and dirty port of chip8swemu for the [Game & Watch: Super Mario Bros.](https://en.wikipedia.org/wiki/Game_%26_Watch:_Super_Mario_Bros.)

See it in action: https://www.youtube.com/watch?v=n2IKZFr6tSo

## How to use

### Prerequisites

1. Ensure you have the following packages installed on your system and available on the PATH: `git`, `make`, `arm-none-eabi-gcc`, `openocd`.
2. Follow [this guide](https://github.com/ghidraninja/game-and-watch-backup) to backup and prepare your device.
3. Clone this repository, including the submodules: `git clone --recurse-submodules https://github.com/AlfonsoJLuna/chip8swemu`.

### Adding games

Games are stored in the `source/games.h` file. The file already includes some public domain or freely distributed games that can be used for testing the emulator or as an example for adding more games.

If you are looking for more games to test, the [Chip8 Community Archive](https://github.com/JohnEarnest/chip8Archive) includes a selection of modern games released under the Creative Commons 0 "No Rights Reserved" license. For an older selection of freely distributed games you can also check the [Chip-8 Program Pack](https://github.com/dbousamra/scala-js-chip8/tree/master/src/resources/Chip-8%20Pack) by Revival Studios.

Currently, the process of adding new games is done manually. You can convert your games from binary format to a C array using `xxd -i game.ch8` and then you need to manually add them to the `games.h` file. For each game you also need to specify the CPU frequency and the key mapping. This process may be automated in the future.

### Building

1. Open a terminal in `chip8swemu/platform/gameandwatch/`.
2. Type `make` for building.

### Flashing

1. Connect your device to the computer using a ST-Link compatible debugger.
2. Type `make flash` for flashing the binary into your device.
