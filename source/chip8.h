#ifndef CPU_H
#define CPU_H


#include <stdbool.h>
#include <stdint.h>


/*
    Clears the memory and copies the charset and a rom to it.
    rom: A pointer to a chunk of memory containing the rom to be copied.
    rom_size: The rom size in bytes. The rom can't be larger than 3584 bytes.
    Returns 0 if the rom was succesfully copied.
    Returns 1 if the rom was larger than 3584 bytes and hasn't been copied.
*/
bool chip8ResetMem(uint8_t* rom, int rom_size);


/*
    Resets all registers and buffers.
*/
void chip8ResetCpu();


/*
    Get the status of a pixel of the screen buffer.
    Returns 1 if the pixel is set, 0 if the pixel is clear.
*/
bool chip8GetPixel(int row, int col);


/*
    Steps the cpu.
    cycles: The number of instructions to execute.
    Returns 0 if all instructions were executed succesfully.
    Returns 1 if an unsupported opcode has been processed.
*/
bool chip8StepCpu(int cycles);


/*
    Decreases the timers.
    This function needs to be called 60 times per second.
*/
void chip8UpdateTimers();


/*
    Updates a key status.
    This function needs to be called when a key has been pressed or released.
    key: The key pressed or released (from 0x0 to 0xF).
    status: 1 if the key has been pressed, 0 if the key has been released.
*/
void chip8UpdateKey(uint8_t key, bool status);


/*
    Get the buzzer status.
    Returns 1 if the buzzer should beep.
    Returns 0 if the buzzer should be silent.
*/
bool chip8GetAudio();


/*
    Get the screen buffer.
    Returns a pointer to a 128x64 pixels = 8192 bits = 1024 bytes array
    containing the screen buffer (one bit per pixel).
*/
uint8_t* chip8GetScreen();


#endif  // CPU_H