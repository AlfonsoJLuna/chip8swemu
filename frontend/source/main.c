#include "audio.h"
#include "chip8.h"
#include "config.h"
#include "events.h"
#include "gui.h"
#include "rom.h"
#include "video.h"


int main(int argc, char* argv[])
{
    bool quit = false;

    configSetDefaults();
    configLoadFromFile();

    if (!videoInitialize())
    {
        if (!audioInitialize())
        {
            if (argc > 1)
            {
                romLoadFromPath(argv[1]);
            }
            else
            {
                romLoadDefault();
            }

            while (!quit)
            {
                quit |= eventsProcess();
                quit |= guiGetFlagQuit();

                config_t config = configGet();

                if (chip8StepCpu(config.frequency / 60))
                    guiShowUnknownOp();

                audioUpdate(chip8GetAudio());

                chip8UpdateTimers();

                videoRender();
            }

            configSaveToFile();

            audioFinalize();
        }

        videoFinalize();
    }

	return 0;
}
