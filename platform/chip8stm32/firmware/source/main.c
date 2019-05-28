#include "system.h"
#include "splash.h"
#include "menu.h"
#include "games.h"
#include "chip8.h"
#include "input.h"
#include "buzzer.h"
#include "display.h"


int _gettimeofday (struct timeval* tp, struct timezone* tzp)
{
    return 0;
}


int main()
{
    systemInit();
    splashScreenShow();
    unsigned int game = menuSelectGame();
    chip8ResetMem(game_data[game], game_size[game]);
    chip8ResetCpu();

    unsigned long current_millis;
    unsigned long last_millis = 0;

    while (1)
    {
        current_millis = systemMillis();

        if ((current_millis - last_millis) >= (1000 / 60))
        {
            last_millis = current_millis;

            chip8UpdateKey(0x0, inputRead(0x0));
            chip8UpdateKey(0x1, inputRead(0x1));
            chip8UpdateKey(0x2, inputRead(0x2));
            chip8UpdateKey(0x3, inputRead(0x3));
            chip8UpdateKey(0x4, inputRead(0x4));
            chip8UpdateKey(0x5, inputRead(0x5));
            chip8UpdateKey(0x6, inputRead(0x6));
            chip8UpdateKey(0x7, inputRead(0x7));
            chip8UpdateKey(0x8, inputRead(0x8));
            chip8UpdateKey(0x9, inputRead(0x9));
            chip8UpdateKey(0xA, inputRead(0xA));
            chip8UpdateKey(0xB, inputRead(0xB));
            chip8UpdateKey(0xC, inputRead(0xC));
            chip8UpdateKey(0xD, inputRead(0xD));
            chip8UpdateKey(0xE, inputRead(0xE));
            chip8UpdateKey(0xF, inputRead(0xF));

            chip8StepCpu(game_frequency[game] / 60);

            if (chip8GetAudio())
            {
                buzzerBeep();
            }
            else
            {
                buzzerSilent();
            }

            chip8UpdateTimers();

            displayRefresh(chip8GetScreen());
        }
    }

    return 0;
}
