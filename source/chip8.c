#include "chip8.h"
#include <stdlib.h>
#include <time.h>


void resetMemory(cpu* chip8)
{
    // Clear memory
    for (int i = 0; i < 4096; i++)
    {
        chip8->memory[i] = 0;
    }

    // Sprites representing characters from 0 to F
    uint8_t charset[5 * 16] = {
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

    // Load the charset into memory
    for (int i = 0; i < 5 * 16; i++)
    {
        chip8->memory[0x50 + i] = charset[i];    // Charset starts at memory location 0x50
    }

    chip8->rom_loaded = false;
}


void resetCPU(cpu* chip8)
{
    // Initialize random number generator
    srand(time(0));

    // Programs start at memory location 0x200
    chip8->program_counter = 0x200;

    // Clear registers
    chip8->I = 0;
    chip8->stack_pointer = 0;
    chip8->delay_timer = 0;
    chip8->sound_timer = 0;

    // Clear opcode
    chip8->opcode = 0;
    chip8->opcode_X = 0;
    chip8->opcode_Y = 0;
    chip8->opcode_N = 0;
    chip8->opcode_KK = 0;
    chip8->opcode_NNN = 0;

    // Clear stack, general registers and keyboard
    for (int i = 0; i < 16; i++)
    {
        chip8->stack[i] = 0;
        chip8->V[i] = 0;
        chip8->keyboard[i] = 0;
    }

    // Clear screen
    for (int i = 0; i < 2048; i++)
    {
        chip8->screen[i] = 0;
    }

    chip8->unknown_opcode = false;
    chip8->instructions = 0;
}


void fetchInstruction(cpu* chip8)
{
    // Each 16-bit opcode takes two 8-bit memory locations, the one indicated by the program_counter and its adjacent
    chip8->opcode = chip8->memory[chip8->program_counter] << 8 | chip8->memory[chip8->program_counter + 1];

    chip8->opcode_X = (chip8->opcode & 0x0F00) >> 8;
    chip8->opcode_Y = (chip8->opcode & 0x00F0) >> 4;
    chip8->opcode_N = chip8->opcode & 0x000F;
    chip8->opcode_KK = chip8->opcode & 0x00FF;
    chip8->opcode_NNN = chip8->opcode & 0x0FFF;
}


void executeInstruction(cpu* chip8)
{
    switch (chip8->opcode & 0xF000)
    {
        case 0x0000:
            switch (chip8->opcode & 0x0FFF)
            {
                case 0x00E0:    // 00E0: Clear the display
                    for (int pixel = 0; pixel < 2048; pixel++)
                    {
                        chip8->screen[pixel] = 0;
                    }
                    chip8->program_counter += 2;
                break;

                case 0x00EE:    // 00EE: Return from a subroutine
                    chip8->stack_pointer--;
                    chip8->program_counter = chip8->stack[chip8->stack_pointer];
                    chip8->program_counter += 2;
                break;

                default:
                    chip8->unknown_opcode = true;
                break;
            }
        break;

        case 0x1000:    // 1NNN: Jump to location NNN
            chip8->program_counter = chip8->opcode_NNN;
        break;

        case 0x2000:    // 2NNN: Call subroutine at NNN
            chip8->stack[chip8->stack_pointer] = chip8->program_counter;
            chip8->stack_pointer++;
            chip8->program_counter = chip8->opcode_NNN;
        break;

        case 0x3000:    // 3XKK: Skip next instruction if VX = KK
            if (chip8->V[chip8->opcode_X] == chip8->opcode_KK)
            {
                chip8->program_counter += 4;
            }
            else
            {
                chip8->program_counter += 2;
            }
        break;

        case 0x4000:    // 4XKK: Skip next instruction if VX != KK
            if (chip8->V[chip8->opcode_X] != chip8->opcode_KK)
            {
                chip8->program_counter += 4;
            }
            else
            {
                chip8->program_counter += 2;
            }
        break;

        case 0x5000:    // 5XY0: Skip next instruction if VX = VY
            if (chip8->V[chip8->opcode_X] == chip8->V[chip8->opcode_Y])
            {
                chip8->program_counter += 4;
            }
            else
            {
                chip8->program_counter += 2;
            }
        break;

        case 0x6000:    // 6XKK: Set VX = KK
            chip8->V[chip8->opcode_X] = chip8->opcode_KK;
            chip8->program_counter += 2;
        break;

        case 0x7000:    // 7XKK: Adds KK to VX
            chip8->V[chip8->opcode_X] += chip8->opcode_KK;
            chip8->program_counter += 2;
        break;

        case 0x8000:
            switch (chip8->opcode & 0x000F)
            {
                case 0x0000:    // 8XY0: Set VX = VY
                    chip8->V[chip8->opcode_X] = chip8->V[chip8->opcode_Y];
                    chip8->program_counter += 2;
                break;

                case 0x0001:    // 8XY1: Set VX = VX OR VY
                    chip8->V[chip8->opcode_X] |= chip8->V[chip8->opcode_Y];
                    chip8->program_counter += 2;
                break;

                case 0x0002:    // 8XY2: Set VX = VX AND VY
                    chip8->V[chip8->opcode_X] &= chip8->V[chip8->opcode_Y];
                    chip8->program_counter += 2;
                break;

                case 0x0003:    // 8XY3: Set VX = VX XOR VY
                    chip8->V[chip8->opcode_X] ^= chip8->V[chip8->opcode_Y];
                    chip8->program_counter += 2;
                break;

                case 0x0004:    // 8XY4: Set VX = VX + VY, set VF = carry
                    // Overflow occurred if VX > VX + VY
                    chip8->V[0xF] = chip8->V[chip8->opcode_X] > (chip8->V[chip8->opcode_X] + chip8->V[chip8->opcode_Y]);
                    // VX = VX + VY
                    chip8->V[chip8->opcode_X] += chip8->V[chip8->opcode_Y];
                    chip8->program_counter += 2;
                break;

                case 0x0005:    // 8XY5: Set VX = VX - VY, set VF = NOT borrow
                    // Borrow occurred if VY > VX, so NOT borrow occured if VX >= VY
                    chip8->V[0xF] = chip8->V[chip8->opcode_X] >= chip8->V[chip8->opcode_Y];
                    // VX = VX - VY
                    chip8->V[chip8->opcode_X] -= chip8->V[chip8->opcode_Y];
                    chip8->program_counter += 2;
                break;

                case 0x0006:    // 8XY6: Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift. VY is ignored on modern implementations
                    chip8->V[0xF] = chip8->V[chip8->opcode_X] & 0x01;
                    chip8->V[chip8->opcode_X] >>= 1;
                    chip8->program_counter += 2;
                break;

                case 0x0007:    // 8XY7: Set VX = VY - VX, set VF = NOT borrow
                    // Borrow occurred if VX > VY, so NOT borrow occured if VY >= VX
                    chip8->V[0xF] = chip8->V[chip8->opcode_Y] >= chip8->V[chip8->opcode_X];
                    // VX = VY - VX
                    chip8->V[chip8->opcode_Y] -= chip8->V[chip8->opcode_X];
                    chip8->program_counter += 2;
                break;

                case 0x000E:    // 8XYE: Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift. VY is ignored on modern implementations
                    chip8->V[0xF] = chip8->V[chip8->opcode_X] >> 7;
                    chip8->V[chip8->opcode_X] <<= 1;
                    chip8->program_counter += 2;
                break;

                default:
                    chip8->unknown_opcode = true;
                break;
            }
        break;

        case 0x9000:    // 9XY0: Skip next instruction if VX != VY
            if (chip8->V[chip8->opcode_X] != chip8->V[chip8->opcode_Y])
            {
                chip8->program_counter += 4;
            }
            else
            {
                chip8->program_counter += 2;
            }
        break;

        case 0xA000:    // ANNN: Set I = NNN
            chip8->I = chip8->opcode_NNN;
            chip8->program_counter += 2;
        break;

        case 0xB000:    // BNNN: Jump to location NNN + V0
            chip8->program_counter = chip8->opcode_NNN + chip8->V[0];
        break;

        case 0xC000:    // CXKK: Set VX = random byte AND KK
            chip8->V[chip8->opcode_X] = (rand() % 255) & chip8->opcode_KK;
            chip8->program_counter += 2;
        break;

        case 0xD000:    // DXYN: Display N-byte sprite starting at memory location I at (VX, VY), set VF = collision
        {
            uint8_t Y = chip8->V[chip8->opcode_Y];  // Screen Y coordinate (from 0 to 31)
            uint8_t X;                              // Screen X coordinate (from 0 to 63)
            uint8_t row;                            // Sprite row coordinate (from 0 to N)
            uint8_t column;                         // Sprite column coordinate (from 0 to 7)
            uint8_t sprite_row;

            chip8->V[0xF] = 0;

            // For each row of the sprite
            for (row = 0; row < chip8->opcode_N; row++)
            {
                sprite_row = chip8->memory[chip8->I + row];

                // Manage vertical wrap
                if (Y > 31) Y %= 32;

                X = chip8->V[chip8->opcode_X];

                // For each column of the row
                for (column = 0; column < 8; column++)
                {
                    // Manage horizontal wrap
                    if (X > 63) X %= 64;

                    // Check for collision
                    chip8->V[0xF] |= chip8->screen[64*Y + X] & ((sprite_row >> (7 - column)) & 0x1);

                    // XOR-Copy sprite_row pixel to screen
                    chip8->screen[64*Y + X] ^= ((sprite_row >> (7 - column)) & 0x1);

                    X++;
                }
                Y++;
            }
            chip8->program_counter += 2;
        break;
        }

        case 0xE000:
            switch (chip8->opcode & 0x00FF)
            {
                case 0x009E:    // EX9E: Skip next instruction if key with the value of VX is pressed
                    if (chip8->keyboard[chip8->V[chip8->opcode_X]])
                    {
                        chip8->program_counter += 4;
                    }
                    else
                    {
                        chip8->program_counter += 2;
                    }
                break;
                case 0x00A1:    // EXA1: Skip next instruction if key with the value of VX is not pressed
                    if (!chip8->keyboard[chip8->V[chip8->opcode_X]])
                    {
                        chip8->program_counter += 4;
                    }
                    else
                    {
                        chip8->program_counter += 2;
                    }
                break;

                default:
                    chip8->unknown_opcode = true;
                break;
            }
        break;

        case 0xF000:
            switch (chip8->opcode & 0x00FF)
            {
                case 0x0007:    // FX07: Set VX = delay timer value
                    chip8->V[chip8->opcode_X] = chip8->delay_timer;
                    chip8->program_counter += 2;
                break;

                case 0x000A:    // FX01: Wait for a key press, store the value of the key in VX
                {
                    // If no key was pressed, repeat the instruction until a key is pressed
                    for (int key = 0; key < 16; key++)
                    {
                        if (chip8->keyboard[key])
                        {
                            chip8->V[chip8->opcode_X] = key;
                            chip8->program_counter += 2;
                        }
                    }
                break;
                }

                case 0x0015:    // FX15: Set delay timer = VX
                    chip8->delay_timer = chip8->V[chip8->opcode_X];
                    chip8->program_counter += 2;
                break;

                case 0x0018:    // FX18: Set sound timer = VX
                    chip8->sound_timer = chip8->V[chip8->opcode_X];
                    chip8->program_counter += 2;
                break;

                case 0x001E:    // FX1E: Set I = I + VX
                    chip8->I += chip8->V[chip8->opcode_X];
                    chip8->program_counter += 2;
                break;

                case 0x0029:    // FX29: Set I = location of sprite for digit VX
                    chip8->I = 0x50 + chip8->V[chip8->opcode_X] * 5;
                    chip8->program_counter += 2;
                break;

                case 0x0033:    // FX33: Store BCD representation of VX in memory locations I, I+1, and I+2
                    chip8->memory[chip8->I] = chip8->V[chip8->opcode_X] / 100;
                    chip8->memory[chip8->I + 1] = (chip8->V[chip8->opcode_X] / 10) % 10;
                    chip8->memory[chip8->I + 2] = chip8->V[chip8->opcode_X] % 10;
                    chip8->program_counter += 2;
                break;

                case 0x0055:    // FX55: Store registers V0 through VX in memory starting at location I
                {
                    for (int i = 0; i <= chip8->opcode_X; i++)
                    {
                        chip8->memory[chip8->I + i] = chip8->V[i];
                    }
                    chip8->program_counter += 2;
                break;
                }

                case 0x0065:    // FX65: Read registers V0 through VX from memory starting at location I
                {
                    for (int i = 0; i <= chip8->opcode_X; i++)
                    {
                        chip8->V[i] = chip8->memory[chip8->I + i];
                    }
                    chip8->program_counter += 2;
                break;
                }

                default:
                    chip8->unknown_opcode = true;
                break;
            }
        break;

        default:
            chip8->unknown_opcode = true;
        break;
    }
}


void updateTimers(cpu* chip8)
{
    chip8->instructions++;

    // The delay timer is decreased each 14 instructions
    if (chip8->instructions >= 14)
    {
        if (chip8->delay_timer > 0)
        {
            chip8->delay_timer--;
        }
        if (chip8->sound_timer > 0)
        {
            chip8->sound_timer--;
        }
        chip8->instructions = 0;
    }
}
