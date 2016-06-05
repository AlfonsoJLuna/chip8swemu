#ifndef CPU_H
#define CPU_H


#include <stdbool.h>
#include <stdint.h>


struct cpu
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
    uint8_t keyboard[16];

    uint16_t opcode;
    uint8_t opcode_X;
    uint8_t opcode_Y;
    uint8_t opcode_N;
    uint8_t opcode_KK;
    uint16_t opcode_NNN;

    bool rom_loaded;            // When true, a ROM has been loaded into the memory
    bool screen_updated;        // When true, the screen has been updated and needs to be rendered again
    bool unknown_opcode;        // When true, an unknown opcode has been processed and the emulation should be stopped

    unsigned int instructions;  // Number of instructions executed since last timers update
};


// Clears the memory and loads the charset into memory
void resetMemory(struct cpu* chip8);

// Initializes the CPU, setting the program counter and clearing all registers
void resetCPU(struct cpu* chip8);

// Fetches the next instruction, storing it in opcode
void fetchInstruction(struct cpu* chip8);

// Executes the current opcode
void executeInstruction(struct cpu* chip8);

// Decreases delay_timer and sound_timer if is required
void updateTimers(struct cpu* chip8);


#endif  // CPU_H
