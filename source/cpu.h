#ifndef CPU_H
#define CPU_H


#include <stdbool.h>
#include <stdint.h>


typedef struct
{
    uint8_t memory[4096];
    uint16_t program_counter;
    uint16_t I;                 // Index register
    uint16_t stack[16];
    uint8_t stack_pointer;
    uint8_t V[16];              // General purpose registers
    uint8_t delay_timer;
    uint8_t sound_timer;

    uint8_t screen[2048];       // Screen has 64 * 32 = 2048 pixels
    bool keyboard[16];

    uint16_t opcode;
    uint8_t opcode_X;
    uint8_t opcode_Y;
    uint8_t opcode_N;
    uint8_t opcode_KK;
    uint16_t opcode_NNN;

    bool rom_loaded;            // When true, a ROM has been loaded into the memory
    bool unknown_opcode;        // When true, an unknown opcode has been processed and the emulation should be stopped

    unsigned int instructions;  // Number of instructions executed since last timers update
} cpu;


void resetMemory(cpu* chip8);

void resetCPU(cpu* chip8);

void fetchInstruction(cpu* chip8);

void executeInstruction(cpu* chip8);

void updateTimers(cpu* chip8);


#endif  // CPU_H

