## Game & Watch

![](../../Assets/Images/GameAndWatch-1.jpg)

This is a quick and dirty port of chip8swemu for the [Game & Watch: Super Mario Bros.](https://en.wikipedia.org/wiki/Game_%26_Watch:_Super_Mario_Bros.)

### Adding games

Games are stored in the `games.h` file. The file already includes some public domain games that can be used for testing the emulator or as an example for adding more games.

If you are looking for games to test, the [Chip8 Community Archive](https://github.com/JohnEarnest/chip8Archive) includes a selection of games released under the Creative Commons 0 "No Rights Reserved" license.

Currently, the process of adding new games is very tedious. You can convert your games from binary format to a C array using `xxd -i game.ch8` and then you need to manually add them to the `games.h` file. For each game you also need to specify the CPU frequency and the key mapping. This process may be automated in the future.

### Building steps

1. Follow [this guide](https://github.com/ghidraninja/game-and-watch-backup) to install the required packages and prepare your device.
2. Open the project `gw_base.ioc` with STM32CubeMX and press generate to generate the configuration. It will also provide the correct HAL drivers.
3. Go to the `game-and-watch-base` directory and run `make` for building.
4. Run `make flash` for flashing the binary to your device.
