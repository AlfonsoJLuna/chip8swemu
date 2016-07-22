#include "config.h"
#include "cpu.h"
#include "video.h"
#include "audio.h"
#include "fileio.h"
#include "input.h"


int main(int argc, char* argv[])
{
    configuration config;
    cpu chip8;

    if (!initializeSDL(&config))
    {
        if (!initializeAudio())
        {
            setDefaultConfig(&config);

            loadConfig(&config);

            if (!createWindow(&config))
            {
                resetMemory(&chip8);
                resetCPU(&chip8);

                if (argc > 1)
                {
                    chip8.rom_loaded = !loadRomFromPath(argv[1], chip8.memory);
                }

                while (!config.quit)
                {
                    processInput(&config.quit, chip8.keyboard);

                    if (config.load_rom)
                    {
                        if (!getRomPath(config.rom_path))
                        {
                            resetMemory(&chip8);
                            resetCPU(&chip8);
                            chip8.rom_loaded = !loadRomFromPath(config.rom_path, chip8.memory);
                        }
                        config.load_rom = false;
                    }

                    if (config.reset)
                    {
                        resetCPU(&chip8);
                        config.reset = false;
                    }

                    if (chip8.rom_loaded || !chip8.unknown_opcode)
                    {
                        for (int i = 0; i < config.instructions_per_second / config.screen_refresh_rate; i++)
                        {
                            fetchInstruction(&chip8);
                            executeInstruction(&chip8);
                            updateTimers(&chip8);
                            updateAudio(chip8.sound_timer && !config.mute_sound);
                        }
                    }
                    renderScreen(&config, chip8.screen);
                }
                saveConfig(&config);
            }
            finalizeAudio();
        }
        finalizeSDL();
    }
    return 0;
}

