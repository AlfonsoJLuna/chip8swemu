#include <stdbool.h>
#include "chip8-lcd.h"
#include "lcd.h"

static inline bool getPixel(uint8_t* chip8_buffer, int row, int col)
{
    return (chip8_buffer[(128 * row + col) / 8] >> (7 - (col % 8))) & 1;
}

void displayRefresh(uint8_t* chip8_buffer)
{
    for (int x = 0; x < 320; x++)
    {
        for (int y = 0; y < 160; y++)
        {
            if (getPixel(chip8_buffer, y * 64 / 160, x * 128 / 320))
                framebuffer[((y+40)*320)+x] = 0xFFFF;
            else
                framebuffer[((y+40)*320)+x] = 0x0000;
      }
    }
}
