#include "audio.h"
#include "chip8.h"
#include "config.h"
#include "events.h"
#include "gui.h"
#include "rom.h"
#include "video.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>


int main(int argc, char* argv[])
{
    Config_SetDefaults();
    Config_LoadFromFile();

    if (Video_Init())
    {
        printf("Error: Video context initialization failed.\n");
        return 1;
    }

    if (Audio_Init())
    {
        printf("Error: Audio context initialization failed.\n");
        Video_Quit();
        return 1;
    }

    if (argc > 1)
    {
        ROM_LoadFromPath(argv[1]);
    }
    else
    {
        ROM_LoadDefault();
    }

    bool Quit = false;

    while (!Quit)
    {
        Quit |= Events_Process();
        Quit |= GUI_GetFlagQuit();

        config_t Config = Config_Get();

        chip8CompatibilityMode(Config.CompatibilityMode);
        chip8VerticalWrap(Config.CompatibilityMode);

        if (chip8StepCpu(Config.Frequency / 60))
        {
            GUI_ShowUnknownOp();
        }

        Audio_Update(chip8GetAudio());

        chip8UpdateTimers();

        Video_Render();
    }

    Config_SaveToFile();

    Audio_Quit();
    Video_Quit();

    return 0;
}
