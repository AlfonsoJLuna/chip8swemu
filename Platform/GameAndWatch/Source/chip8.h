/*
    CHIP-8 and Super-CHIP interpreter library
    https://github.com/AlfonsoJLuna/chip8swemu
*/


#ifndef CHIP8_H
#define CHIP8_H


#include <stdbool.h>
#include <stdint.h>
#include <string.h>


/**
 * Clears the memory and copies the charset and a rom to it.
 *
 * @param rom A pointer to a chunk of memory containing the rom to be copied.
 * @param rom_size The rom size in bytes. The rom can't be larger than 3584 bytes.
 *
 * @return 0 if the rom was succesfully copied, 1 otherwise.
 */
int chip8ResetMem(uint8_t* rom, size_t rom_size);


/**
 * Resets all registers and buffers.
 */
void chip8ResetCpu();


/**
 * Get the status of a pixel of the screen buffer.
 *
 * @param row The row to be accessed.
 * @param col The column to be accessed.
 *
 * @return 0 if the pixel is clear, 1 if the pixel is set.
 */
bool chip8GetPixel(int row, int col);


/**
 * Steps the cpu.
 *
 * @param cycles The number of instructions to execute.
 *
 * @return 0 if success, 1 if an unsupported opcode has been processed.
 */
bool chip8StepCpu(int cycles);


/**
 * Decreases the timers.
 * This function needs to be called 60 times per second.
 */
void chip8UpdateTimers();


/**
 * Updates a key status.
 * This function needs to be called when a key has been pressed or released.
 *
 * @param key The key pressed or released (from 0x0 to 0xF).
 * @param status 1 if the key has been pressed, 0 if the key has been released.
 */
void chip8UpdateKey(uint8_t key, bool status);


/**
 * Get the buzzer status.
 *
 * @return 0 if the buzzer should be silent, 1 if should beep.
 */
bool chip8GetAudio();


/**
 * Get the screen buffer.
 *
 * @return A pointer to the 128x64 pixels = 1024 bytes screen buffer.
*/
uint8_t* chip8GetScreen();


/**
 * Change 8XY6, 8XYE, FX55, FX65 behavior between
 * CHIP-8 (compatibility mode enabled) or Super-CHIP (disabled).
 *
 * @param Enable or disable the compatibility mode.
 */
void chip8CompatibilityMode(bool enabled);


/**
 * Configure Vertical Wrap
 *
 * @param Enable or disable the vertical wrap.
 */
void chip8VerticalWrap(bool enabled);


#endif  // CHIP8_H
