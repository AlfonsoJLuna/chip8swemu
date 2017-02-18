#include "chip8.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>


typedef struct
{
    uint8_t data[4096];
    bool    rom_loaded;
} memory_t;


static memory_t memory;


bool chip8ResetMem(uint8_t* rom, int rom_size)
{
    // Clear the memory
    memory = (memory_t){0};

    // The charset: sprites representing characters from 0 to F
    const uint8_t charset[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // "0"
        0x20, 0x60, 0x20, 0x20, 0x70, // "1"
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // "2"
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // "3"
        0x90, 0x90, 0xF0, 0x10, 0x10, // "4"
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // "5"
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // "6"
        0xF0, 0x10, 0x20, 0x40, 0x40, // "7"
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // "8"
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // "9"
        0xF0, 0x90, 0xF0, 0x90, 0x90, // "A"
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // "B"
        0xF0, 0x80, 0x80, 0x80, 0xF0, // "C"
        0xE0, 0x90, 0x90, 0x90, 0xE0, // "D"
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // "E"
        0xF0, 0x80, 0xF0, 0x80, 0x80  // "F"
    };

    // Copy the charset into memory starting at memory address 0x50
    memcpy(&memory.data[0x50], charset, 80);

    // Check if the rom fits the memory
    if (rom_size <= 3584)
    {
        // Then copy the rom into memory starting at memory address 0x200
        memcpy(&memory.data[0x200], rom, rom_size);
        memory.rom_loaded = true;
    }

    return !memory.rom_loaded;
}


typedef struct
{
    uint16_t PC;            // Program counter
    uint16_t I;             // Index register
    uint8_t  V[16];         // General purpose registers
    uint8_t  SP;            // Stack pointer
    uint16_t stack[16];
    uint8_t  screen[1024];  // Screen buffer needs 128x64 pixels = 8192 bits = 1024 bytes
    bool     keyboard[16];

    uint16_t opcode;

    uint8_t  delay_timer;
    uint8_t  sound_timer;
} cpu_t;


static cpu_t cpu;


void chip8ResetCpu()
{
    cpu = (cpu_t){0};

    // Programs start at memory address 0x200
    cpu.PC = 0x200;

    // Initialize random number generator
    srand(time(0));
}


static inline void fetchInstruction()
{
    cpu.opcode = memory.data[cpu.PC] << 8 | memory.data[cpu.PC + 1];
}


#define OPCODE_X ((cpu.opcode & 0x0F00) >> 8)
#define OPCODE_Y ((cpu.opcode & 0x00F0) >> 4)
#define OPCODE_N (cpu.opcode & 0x000F)
#define OPCODE_KK (cpu.opcode & 0x00FF)
#define OPCODE_NNN (cpu.opcode & 0x0FFF)


inline bool chip8GetPixel(int row, int col)
{
    if (row < 64 && col < 128)
        return cpu.screen[(64 * row + col) / 8] & (1 << ((64 * row + col) % 8));
    else
        return 0;
}


static inline void setPixel(int row, int col)
{
    if (row < 64 && col < 128)
        cpu.screen[(64 * row + col) / 8] |= (1 << ((64 * row + col) % 8));
}


static inline void clearPixel(int row, int col)
{
    if (row < 64 && col < 128)
        cpu.screen[(64 * row + col) / 8] &= ~(1 << ((64 * row + col) % 8));
}


// 00E0: Clear the display
static inline void exec00E0()
{
    for (int i = 0; i < 1024; i++)
    {
        cpu.screen[i] = 0;
    }
    cpu.PC += 2;
}


// 00EE: Return from a subroutine
static inline void exec00EE()
{
    cpu.SP--;
    cpu.PC = cpu.stack[cpu.SP];
    cpu.PC += 2;
}


// 1NNN: Jump to location NNN
static inline void exec1NNN()
{
    cpu.PC = OPCODE_NNN;
}


// 2NNN: Call subroutine at NNN
static inline void exec2NNN()
{
    cpu.stack[cpu.SP] = cpu.PC;
    cpu.SP++;
    cpu.PC = OPCODE_NNN;
}
            

// 3XKK: Skip next instruction if VX = KK
static inline void exec3XKK()
{
    if (cpu.V[OPCODE_X] == OPCODE_KK)
        cpu.PC += 4;
    else
        cpu.PC += 2;
}


// 4XKK: Skip next instruction if VX != KK
static inline void exec4XKK()
{
    if (cpu.V[OPCODE_X] != OPCODE_KK)
        cpu.PC += 4;
    else
        cpu.PC += 2;
}


// 5XY0: Skip next instruction if VX = VY
static inline void exec5XY0()
{
    if (cpu.V[OPCODE_X] == cpu.V[OPCODE_Y])
        cpu.PC += 4;
    else
        cpu.PC += 2;
}


// 6XKK: Set VX = KK
static inline void exec6XKK()
{
    cpu.V[OPCODE_X] = OPCODE_KK;
    cpu.PC += 2;
}


// 7XKK: Adds KK to VX
static inline void exec7XKK()
{
    cpu.V[OPCODE_X] += OPCODE_KK;
    cpu.PC += 2;
}


// 8XY0: Set VX = VY
static inline void exec8XY0()
{
    cpu.V[OPCODE_X] = cpu.V[OPCODE_Y];
    cpu.PC += 2;
}


// 8XY1: Set VX = VX OR VY
static inline void exec8XY1()
{
    cpu.V[OPCODE_X] |= cpu.V[OPCODE_Y];
    cpu.PC += 2;
}


// 8XY2: Set VX = VX AND VY
static inline void exec8XY2()
{
    cpu.V[OPCODE_X] &= cpu.V[OPCODE_Y];
    cpu.PC += 2;
}


// 8XY3: Set VX = VX XOR VY
static inline void exec8XY3()
{
    cpu.V[OPCODE_X] ^= cpu.V[OPCODE_Y];
    cpu.PC += 2;
}


// 8XY4: Set VX = VX + VY, set VF = carry
static inline void exec8XY4()
{
    // Overflow occurred if VX > VX + VY
    cpu.V[0xF] = cpu.V[OPCODE_X] > (cpu.V[OPCODE_X] + cpu.V[OPCODE_Y]);
    // VX = VX + VY
    cpu.V[OPCODE_X] += cpu.V[OPCODE_Y];
    cpu.PC += 2;
}


// 8XY5: Set VX = VX - VY, set VF = NOT borrow
static inline void exec8XY5()
{
    // Borrow occurred if VY > VX, so NOT borrow occured if VX >= VY
    cpu.V[0xF] = cpu.V[OPCODE_X] >= cpu.V[OPCODE_Y];
    // VX = VX - VY
    cpu.V[OPCODE_X] -= cpu.V[OPCODE_Y];
    cpu.PC += 2;
}


// 8XY6: Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift.
// Note: VY is ignored on modern implementations
static inline void exec8XY6()
{
    cpu.V[0xF] = cpu.V[OPCODE_X] & 0x01;
    cpu.V[OPCODE_X] >>= 1;
    cpu.PC += 2;
}


// 8XY7: Set VX = VY - VX, set VF = NOT borrow
static inline void exec8XY7()
{
    // Borrow occurred if VX > VY, so NOT borrow occured if VY >= VX
    cpu.V[0xF] = cpu.V[OPCODE_Y] >= cpu.V[OPCODE_X];
    // VX = VY - VX
    cpu.V[OPCODE_Y] -= cpu.V[OPCODE_X];
    cpu.PC += 2;
}


// 8XYE: Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift.
// Note: The original implementation stores the value of register VY shifted left one bit in register VX.
static inline void exec8XYE()
{
    cpu.V[0xF] = cpu.V[OPCODE_X] >> 7;
    cpu.V[OPCODE_X] <<= 1;
    cpu.PC += 2;
}


// 9XY0: Skip next instruction if VX != VY
static inline void exec9XY0()
{
    if (cpu.V[OPCODE_X] != cpu.V[OPCODE_Y])
        cpu.PC += 4;
    else
        cpu.PC += 2;
}


// ANNN: Set I = NNN
static inline void execANNN()
{
    cpu.I = OPCODE_NNN;
    cpu.PC += 2;  
}


// BNNN: Jump to location NNN + V0
static inline void execBNNN()
{
    cpu.PC = OPCODE_NNN + cpu.V[0];
}


// CXKK: Set VX = random byte AND KK
static inline void execCXKK()
{
    cpu.V[OPCODE_X] = (rand() % 255) & OPCODE_KK;
    cpu.PC += 2;
}


// DXYN: Display N-byte sprite starting at memory location I at (VX, VY), set VF = collision
static inline void execDXYN()
{
    uint8_t Y = cpu.V[OPCODE_Y];  // Screen Y coordinate (from 0 to 31)
    uint8_t X;                    // Screen X coordinate (from 0 to 63)
    uint8_t row;                  // Sprite row coordinate (from 0 to N)
    uint8_t column;               // Sprite column coordinate (from 0 to 7)
    uint8_t sprite_row;

    cpu.V[0xF] = 0;

    // For each row of the sprite
    for (row = 0; row < OPCODE_N; row++)
    {
        sprite_row = memory.data[cpu.I + row];

        // Manage vertical wrap
        if (Y > 31) Y %= 32;

        X = cpu.V[OPCODE_X];

        // For each column of the row
        for (column = 0; column < 8; column++)
        {
            // Manage horizontal wrap
            if (X > 63) X %= 64;

            // Check for collision
            cpu.V[0xF] |= chip8GetPixel(Y, X) & ((sprite_row >> (7 - column)) & 0x1);

            // XOR-Copy sprite_row pixel to screen
            if (chip8GetPixel(Y, X) ^ ((sprite_row >> (7 - column)) & 0x1))
            {
                setPixel(Y, X);
            }
            else
            {
                clearPixel(Y, X);
            }

            X++;
        }
        Y++;
    }
    cpu.PC += 2;
}


// EX9E: Skip next instruction if key with the value of VX is pressed
static inline void execEX9E()
{
    if (cpu.keyboard[cpu.V[OPCODE_X]])
        cpu.PC += 4;
    else
        cpu.PC += 2;
}


// EXA1: Skip next instruction if key with the value of VX is not pressed
static inline void execEXA1()
{
    if (!cpu.keyboard[cpu.V[OPCODE_X]])
        cpu.PC += 4;
    else
        cpu.PC += 2;
}


// FX07: Set VX = delay timer value
static inline void execFX07()
{
    cpu.V[OPCODE_X] = cpu.delay_timer;
    cpu.PC += 2;
}


// FX0A: Wait for a key press, store the value of the key in VX
static inline void execFX0A()
{
    // If no key was pressed, repeat the instruction until a key is pressed
    for (int key = 0; key < 16; key++)
    {
        if (cpu.keyboard[key])
        {
            cpu.V[OPCODE_X] = key;
            cpu.PC += 2;
        }
        cpu.keyboard[key] = false;
    }
}


// FX15: Set delay timer = VX
static inline void execFX15()
{
    cpu.delay_timer = cpu.V[OPCODE_X];
    cpu.PC += 2;
}


// FX18: Set sound timer = VX
static inline void execFX18()
{
    cpu.sound_timer = cpu.V[OPCODE_X];
    cpu.PC += 2;
}


// FX1E: Set I = I + VX
static inline void execFX1E()
{
    cpu.I += cpu.V[OPCODE_X];
    cpu.PC += 2;
}


// FX29: Set I = location of sprite for digit VX
static inline void execFX29()
{
    cpu.I = 0x50 + cpu.V[OPCODE_X] * 5;
    cpu.PC += 2;
}


// FX33: Store BCD representation of VX in memory locations I, I+1, and I+2
static inline void execFX33()
{
    memory.data[cpu.I] = cpu.V[OPCODE_X] / 100;
    memory.data[cpu.I + 1] = (cpu.V[OPCODE_X] / 10) % 10;
    memory.data[cpu.I + 2] = cpu.V[OPCODE_X] % 10;
    cpu.PC += 2;
}


// FX55: Store registers V0 through VX in memory starting at location I
static inline void execFX55()
{
    for (int i = 0; i <= OPCODE_X; i++)
    {
        memory.data[cpu.I + i] = cpu.V[i];
    }
    cpu.PC += 2;
}


// FX65: Read registers V0 through VX from memory starting at location I
static inline void execFX65()
{
    for (int i = 0; i <= OPCODE_X; i++)
    {
        cpu.V[i] = memory.data[cpu.I + i];
    }
    cpu.PC += 2;
}


static inline bool decodeInstruction()
{
    bool unknown_opcode = false;

    switch (cpu.opcode & 0xF000)
    {
        case 0x0000:
            switch (cpu.opcode & 0x0FFF)
            {
                case 0x00E0:    exec00E0();         break;
                case 0x00EE:    exec00EE();         break;
                default:    unknown_opcode = true;  break;
            }
        break;
        case 0x1000:            exec1NNN();         break;
        case 0x2000:            exec2NNN();         break;
        case 0x3000:            exec3XKK();         break;
        case 0x4000:            exec4XKK();         break;
        case 0x5000:            exec5XY0();         break;
        case 0x6000:            exec6XKK();         break;
        case 0x7000:            exec7XKK();         break;
        case 0x8000:
            switch (cpu.opcode & 0x000F)
            {
                case 0x0000:    exec8XY0();         break;
                case 0x0001:    exec8XY1();         break;
                case 0x0002:    exec8XY2();         break;
                case 0x0003:    exec8XY3();         break;
                case 0x0004:    exec8XY4();         break;
                case 0x0005:    exec8XY5();         break;
                case 0x0006:    exec8XY6();         break;
                case 0x0007:    exec8XY7();         break;
                case 0x000E:    exec8XYE();         break;
                default:    unknown_opcode = true;  break;
            }
        break;
        case 0x9000:            exec9XY0();         break;
        case 0xA000:            execANNN();         break;
        case 0xB000:            execBNNN();         break;
        case 0xC000:            execCXKK();         break;
        case 0xD000:            execDXYN();         break;
        case 0xE000:
            switch (cpu.opcode & 0x00FF)
            {
                case 0x009E:    execEX9E();         break;
                case 0x00A1:    execEXA1();         break;
                default:    unknown_opcode = true;  break;
            }
        break;
        case 0xF000:
            switch (cpu.opcode & 0x00FF)
            {
                case 0x0007:    execFX07();         break;
                case 0x000A:    execFX0A();         break;
                case 0x0015:    execFX15();         break;
                case 0x0018:    execFX18();         break;
                case 0x001E:    execFX1E();         break;
                case 0x0029:    execFX29();         break;
                case 0x0033:    execFX33();         break;
                case 0x0055:    execFX55();         break;
                case 0x0065:    execFX65();         break;
                default:    unknown_opcode = true;  break;
            }
        break;
        default:            unknown_opcode = true;  break;
    }

    return unknown_opcode;
}


bool chip8StepCpu(int cycles)
{
    bool unknown_opcode = false;

    for (int i = 0; i < cycles; i++)
    {
        fetchInstruction();
        unknown_opcode |= decodeInstruction();
    }

    return unknown_opcode;
}


void chip8UpdateTimers()
{
    if (cpu.delay_timer > 0)
        cpu.delay_timer--;

    if (cpu.sound_timer > 0)
        cpu.sound_timer--;
}


void chip8UpdateKey(uint8_t key, bool status)
{
    if (key <= 0xF)
        cpu.keyboard[key] = status;
}


bool chip8GetAudio()
{
    return cpu.sound_timer > 0;
}
