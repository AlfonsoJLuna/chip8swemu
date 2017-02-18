#include "config.h"
#include "chip8.h"
#include "video.h"
#include "audio.h"
#include "fileio.h"
#include "input.h"


int main(int argc, char* argv[])
{
    configuration config;
    bool unknown_opcode = false;
    bool rom_loaded = false;

    if (!initializeSDL(&config))
    {
        if (!initializeAudio())
        {
            setDefaultConfig(&config);

            loadConfig(&config);

            if (!createWindow(&config))
            {
                chip8ResetCpu();

                if (argc > 1)
                {
                    rom_loaded = !loadRomFromPath(argv[1]);
                }

                while (!config.quit)
                {
                    processInput(&config.quit, &config.minimized);

                    if (config.load_rom)
                    {
                        if (!getRomPath(config.rom_path))
                        {
                            chip8ResetCpu();
                            rom_loaded = !loadRomFromPath(config.rom_path);
                            unknown_opcode = false;
                        }
                        config.load_rom = false;
                    }

                    if (config.reset)
                    {
                        chip8ResetCpu();
                        config.reset = false;
                    }

                    if (rom_loaded && !unknown_opcode && !config.minimized)
                    {
                        unknown_opcode = chip8StepCpu(config.instructions_per_second / config.screen_refresh_rate);
                        updateAudio(chip8GetAudio() && !config.mute_sound);
                        chip8UpdateTimers();
                    }

                    renderScreen(&config);
                }
                saveConfig(&config);
            }
            finalizeAudio();
        }
        finalizeSDL();
    }
    return 0;
}
