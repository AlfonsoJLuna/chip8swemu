/*
    CHIP-8 and Super-CHIP interpreter library
    https://github.com/AlfonsoJLuna/chip8swemu
*/


#include "chip8.h"


#include <stdlib.h>
#include <time.h>


typedef struct
{
    uint8_t data[4096];
    bool    rom_loaded;
} memory_t;


static memory_t memory;


int chip8ResetMem(uint8_t* rom, size_t rom_size)
{
    // Clear the memory
    memory = (memory_t){0};

    // The charset: sprites representing characters
    const uint8_t charset[180] = {
        // Low-res sprites
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
        0xF0, 0x80, 0xF0, 0x80, 0x80, // "F"

        // High-res sprites
        0x3C, 0x7E, 0xE7, 0xC3, 0xC3, 0xC3, 0xC3, 0xE7, 0x7E, 0x3C, // "0"
        0x18, 0x38, 0x58, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, // "1"
        0x3E, 0x7F, 0xC3, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xFF, 0xFF, // "2"
        0x3C, 0x7E, 0xC3, 0x03, 0x0E, 0x0E, 0x03, 0xC3, 0x7E, 0x3C, // "3"
        0x06, 0x0E, 0x1E, 0x36, 0x66, 0xC6, 0xFF, 0xFF, 0x06, 0x06, // "4"
        0xFF, 0xFF, 0xC0, 0xC0, 0xFC, 0xFE, 0x03, 0xC3, 0x7E, 0x3C, // "5"
        0x3E, 0x7C, 0xC0, 0xC0, 0xFC, 0xFE, 0xC3, 0xC3, 0x7E, 0x3C, // "6"
        0xFF, 0xFF, 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x60, 0x60, // "7"
        0x3C, 0x7E, 0xC3, 0xC3, 0x7E, 0x7E, 0xC3, 0xC3, 0x7E, 0x3C, // "8"
        0x3C, 0x7E, 0xC3, 0xC3, 0x7F, 0x3F, 0x03, 0x03, 0x3E, 0x7C  // "9"
    };

    // Copy the charset into memory starting at memory address 0x50
    memcpy(&memory.data[0x50], charset, 180);

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
    uint8_t  R[8];          // User editable registers
    uint8_t  SP;            // Stack pointer
    uint16_t stack[16];
    uint8_t  screen[1024];  // Screen buffer needs 128x64 pixels = 8192 bits = 1024 bytes
    bool     extended;      // Extended screen (128x64 mode)
    bool     keyboard[16];

    uint16_t opcode;

    uint8_t  delay_timer;
    uint8_t  sound_timer;

    bool compatibility_mode;
    bool vertical_wrap;
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
    if (row >= 0 && row <= 63 && col >= 0 && col <= 127)
        return (cpu.screen[(128 * row + col) / 8] >> (7 - (col % 8))) & 1;
    else
        return 0;
}


static inline void setPixel(int row, int col)
{
    if (row >= 0 && row <= 63 && col >= 0 && col <= 127)
        cpu.screen[(128 * row + col) / 8] |= (1 << (7 - (col % 8)));
}


static inline void clearPixel(int row, int col)
{
    if (row >= 0 && row <= 63 && col >= 0 && col <= 127)
        cpu.screen[(128 * row + col) / 8] &= ~(1 << (7 - (col % 8)));
}


// 00BN: Scroll display N lines up
static inline void exec00BN()
{
    memmove(cpu.screen, &cpu.screen[16 * OPCODE_N], 1024 - 16 * OPCODE_N);
    memset(&cpu.screen[1023 - 16 * OPCODE_N], 0, 16 * OPCODE_N);
    cpu.PC += 2;
}


// 00CN: Scroll display N lines down
static inline void exec00CN()
{
    memmove(&cpu.screen[16 * OPCODE_N], cpu.screen, 1024 - 16 * OPCODE_N);
    memset(cpu.screen, 0, 16 * OPCODE_N);
    cpu.PC += 2;
}


// 00E0: Clear the display
static inline void exec00E0()
{
    memset(cpu.screen, 0, 1024);
    cpu.PC += 2;
}


// 00EE: Return from a subroutine
static inline void exec00EE()
{
    cpu.SP--;
    cpu.PC = cpu.stack[cpu.SP];
    cpu.PC += 2;
}


// 00FB: Scroll display 4 pixels right
static inline void exec00FB()
{
    for (int row = 0; row < 64; row++)
        for (int col = 15; col >= 0; col--)
        {
            cpu.screen[16 * row + col] >>= 4;
            if (col > 0)
                cpu.screen[16 * row + col] |= (cpu.screen[16 * row + col - 1] & 0x0F) << 4;
        }
    cpu.PC += 2;
}


// 00FC: Scroll display 4 pixels left
static inline void exec00FC()
{
    for (int row = 0; row < 64; row++)
        for (int col = 0; col < 16; col++)
        {
            cpu.screen[16 * row + col] <<= 4;
            if (col < 15)
                cpu.screen[16 * row + col] |= (cpu.screen[16 * row + col + 1] & 0xF0) >> 4;
        }
    cpu.PC += 2;
}


// 00FD: Exit CHIP interpreter
static inline void exec00FD()
{
    // Stop incrementing the PC
}


// 00FE: Disable extended screen mode
static inline void exec00FE()
{
    cpu.extended = false;
    cpu.PC += 2;
}


// 00FF: Enable extended screen mode
static inline void exec00FF()
{
    cpu.extended = true;
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
    cpu.V[0xF] = (cpu.V[OPCODE_X] + cpu.V[OPCODE_Y]) > 0xFF;
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
// Note: The original implementation stores the value of register VY shifted right one bit in register VX.
static inline void exec8XY6()
{
    if (cpu.compatibility_mode)
    {
        cpu.V[0xF] = cpu.V[OPCODE_Y] & 0x01;
        cpu.V[OPCODE_X] = cpu.V[OPCODE_Y] >> 1;
    }
    else
    {
        cpu.V[0xF] = cpu.V[OPCODE_X] & 0x01;
        cpu.V[OPCODE_X] >>= 1;
    }

    cpu.PC += 2;
}


// 8XY7: Set VX = VY - VX, set VF = NOT borrow
static inline void exec8XY7()
{
    // Borrow occurred if VX > VY, so NOT borrow occured if VY >= VX
    cpu.V[0xF] = cpu.V[OPCODE_Y] >= cpu.V[OPCODE_X];
    // VX = VY - VX
    cpu.V[OPCODE_X] = cpu.V[OPCODE_Y] - cpu.V[OPCODE_X];
    cpu.PC += 2;
}


// 8XYE: Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift.
// Note: The original implementation stores the value of register VY shifted left one bit in register VX.
static inline void exec8XYE()
{
    if (cpu.compatibility_mode)
    {
        cpu.V[0xF] = cpu.V[OPCODE_Y] >> 7;
        cpu.V[OPCODE_X] = cpu.V[OPCODE_Y] << 1;
    }
    else
    {
        cpu.V[0xF] = cpu.V[OPCODE_X] >> 7;
        cpu.V[OPCODE_X] <<= 1;
    }

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


// DXY0: Draw 16x16 sprite starting at memory location I at (VX, VY), set VF = collision
static inline void execDXY0()
{
    cpu.V[0xF] = 0;

    for (int row = 0; row < 16; row++)
    {
        for (int col = 0; col < 16; col++)
        {
            uint16_t sprite_row = memory.data[cpu.I + 2 * row] << 8 | memory.data[cpu.I + 2 * row + 1];

            int pixel = (sprite_row >> (15 - col)) & 1;

            int screen_x = (cpu.V[OPCODE_X] + col) % 128;

            int screen_y;
            if (cpu.vertical_wrap)
            {
                screen_y = (cpu.V[OPCODE_Y] + row) % 64;
            }
            else
            {
                screen_y = (cpu.V[OPCODE_Y] + row);
            }

            cpu.V[0xF] |= chip8GetPixel(screen_y, screen_x) & pixel;

            if (chip8GetPixel(screen_y, screen_x) ^ pixel)
                setPixel(screen_y, screen_x);
            else
                clearPixel(screen_y, screen_x);
        }
    }
    cpu.PC += 2;
}


// DXYN: Draw 8xN sprite starting at memory location I at (VX, VY), set VF = collision
static inline void execDXYN()
{
    cpu.V[0xF] = 0;

    for (int row = 0; row < OPCODE_N; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            int pixel = (memory.data[cpu.I + row] >> (7 - col)) & 1;

            int screen_x = (cpu.V[OPCODE_X] + col) % (cpu.extended ? 128 : 64);

            int screen_y;
            if (cpu.vertical_wrap)
            {
                screen_y = (cpu.V[OPCODE_Y] + row) % (cpu.extended ? 64  : 32);
            }
            else
            {
                screen_y = (cpu.V[OPCODE_Y] + row);
            }

            if (cpu.extended)
                cpu.V[0xF] |= chip8GetPixel(screen_y, screen_x) & pixel;
            else
                cpu.V[0xF] |= chip8GetPixel(2 * screen_y, 2 * screen_x) & pixel;

            if (cpu.extended)
            {
                if (chip8GetPixel(screen_y, screen_x) ^ pixel)
                    setPixel(screen_y, screen_x);
                else
                    clearPixel(screen_y, screen_x);
            }
            else
            {
                if (chip8GetPixel(2 * screen_y, 2 * screen_x) ^ pixel)
                {
                    setPixel(2 * screen_y, 2 * screen_x);
                    setPixel(2 * screen_y, 2 * screen_x + 1);
                    setPixel(2 * screen_y + 1, 2 * screen_x);
                    setPixel(2 * screen_y + 1, 2 * screen_x + 1);
                }
                else
                {
                    clearPixel(2 * screen_y, 2 * screen_x);
                    clearPixel(2 * screen_y, 2 * screen_x + 1);
                    clearPixel(2 * screen_y + 1, 2 * screen_x);
                    clearPixel(2 * screen_y + 1, 2 * screen_x + 1);
                }
            }
        }
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
    cpu.V[0xF] = cpu.I >= 0x1000;
    cpu.PC += 2;
}


// FX29: Set I = location of low-res sprite for digit VX
static inline void execFX29()
{
    cpu.I = 0x50 + cpu.V[OPCODE_X] * 5;
    cpu.PC += 2;
}


// FX30: Set I = location of high-res sprite for digit VX
static inline void execFX30()
{
    cpu.I = 0xA0 + cpu.V[OPCODE_X] * 10;
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
// Note: The original implementation also increments I by the number of registers written.
static inline void execFX55()
{
    memcpy(&memory.data[cpu.I], cpu.V, OPCODE_X + 1);

    if (cpu.compatibility_mode)
        cpu.I += (OPCODE_X + 1);

    cpu.PC += 2;
}


// FX65: Read registers V0 through VX from memory starting at location I
// Note: The original implementation also increments I by the number of registers read.
static inline void execFX65()
{
    memcpy(cpu.V, &memory.data[cpu.I], OPCODE_X + 1);

    if (cpu.compatibility_mode)
        cpu.I += (OPCODE_X + 1);

    cpu.PC += 2;
}


// FX75: Store V0..VX in RPL user flags (X <= 7)
static inline void execFX75()
{
    if (OPCODE_X <= 7)
        memcpy(cpu.R, cpu.V, OPCODE_X + 1);
    cpu.PC += 2;
}


// FX85: Read V0..VX from RPL user flags (X <= 7)
static inline void execFX85()
{
    if (OPCODE_X <= 7)
        memcpy(cpu.V, cpu.R, OPCODE_X + 1);
    cpu.PC += 2;
}


static inline bool decodeInstruction()
{
    bool unknown_opcode = false;

    switch (cpu.opcode & 0xF000)
    {
        case 0x0000:
            switch (cpu.opcode & 0xFFF0)
            {
                case 0x00B0:            exec00BN();         break;
                case 0x00C0:            exec00CN();         break;
                case 0x00E0:
                    switch (cpu.opcode)
                    {
                        case 0x00E0:    exec00E0();         break;
                        case 0x00EE:    exec00EE();         break;
                        default:    unknown_opcode = true;  break;
                    }
                break;
                case 0X00F0:
                    switch (cpu.opcode)
                    {
                        case 0x00FB:    exec00FB();         break;
                        case 0x00FC:    exec00FC();         break;
                        case 0x00FD:    exec00FD();         break;
                        case 0x00FE:    exec00FE();         break;
                        case 0x00FF:    exec00FF();         break;
                        default:    unknown_opcode = true;  break;
                    }
                break;
                default:            unknown_opcode = true;  break;
            }
        break;
        case 0x1000:                    exec1NNN();         break;
        case 0x2000:                    exec2NNN();         break;
        case 0x3000:                    exec3XKK();         break;
        case 0x4000:                    exec4XKK();         break;
        case 0x5000:                    exec5XY0();         break;
        case 0x6000:                    exec6XKK();         break;
        case 0x7000:                    exec7XKK();         break;
        case 0x8000:
            switch (cpu.opcode & 0x000F)
            {
                case 0x0000:            exec8XY0();         break;
                case 0x0001:            exec8XY1();         break;
                case 0x0002:            exec8XY2();         break;
                case 0x0003:            exec8XY3();         break;
                case 0x0004:            exec8XY4();         break;
                case 0x0005:            exec8XY5();         break;
                case 0x0006:            exec8XY6();         break;
                case 0x0007:            exec8XY7();         break;
                case 0x000E:            exec8XYE();         break;
                default:            unknown_opcode = true;  break;
            }
        break;
        case 0x9000:                    exec9XY0();         break;
        case 0xA000:                    execANNN();         break;
        case 0xB000:                    execBNNN();         break;
        case 0xC000:                    execCXKK();         break;
        case 0xD000:
            switch(cpu.opcode & 0x000F)
            {
                case 0x0000:            execDXY0();         break;
                default:                execDXYN();         break;
            }
        break;
        case 0xE000:
            switch (cpu.opcode & 0x00FF)
            {
                case 0x009E:            execEX9E();         break;
                case 0x00A1:            execEXA1();         break;
                default:            unknown_opcode = true;  break;
            }
        break;
        case 0xF000:
            switch (cpu.opcode & 0x00FF)
            {
                case 0x0007:            execFX07();         break;
                case 0x000A:            execFX0A();         break;
                case 0x0015:            execFX15();         break;
                case 0x0018:            execFX18();         break;
                case 0x001E:            execFX1E();         break;
                case 0x0029:            execFX29();         break;
                case 0x0030:            execFX30();         break;
                case 0x0033:            execFX33();         break;
                case 0x0055:            execFX55();         break;
                case 0x0065:            execFX65();         break;
                case 0x0075:            execFX75();         break;
                case 0x0085:            execFX85();         break;
                default:            unknown_opcode = true;  break;
            }
        break;
        default:                    unknown_opcode = true;  break;
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


uint8_t* chip8GetScreen()
{
    return cpu.screen;
}


void chip8CompatibilityMode(bool enabled)
{
    cpu.compatibility_mode = enabled;
}


void chip8VerticalWrap(bool enabled)
{
    cpu.vertical_wrap = enabled;
}
