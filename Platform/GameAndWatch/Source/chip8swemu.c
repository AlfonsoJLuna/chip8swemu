#include "chip8swemu.h"

#include "display.h"
#include "games.h"
#include "menu.h"

#include <stdbool.h>
#include <stdint.h>
#include <buttons.h>
#include <chip8.h>
#include <logo.h>


void chip8swemu_main_loop(SAI_HandleTypeDef* hsai_BlockA1)
{
    bool QuitGame = 0;
    uint16_t GameNumber = 0;
    uint32_t CurrentMillis = 0;;
    uint32_t LastMillis = 0;

    // Run the logo ROM for a while
    chip8ResetMem(logo, sizeof(logo));
    chip8ResetCpu();
    chip8StepCpu(50);
    displayRefresh(chip8GetScreen());
    HAL_Delay(3000);

    // Enter main loop
    while (1)
    {
        QuitGame = 0;
        GameNumber = menuSelectGame();

        chip8ResetMem(GameList[GameNumber]->GameData, GameList[GameNumber]->GameSize);
        chip8ResetCpu();
        chip8CompatibilityMode(GameList[GameNumber]->GameEnCompatMode);
        chip8VerticalWrap(GameList[GameNumber]->GameEnCompatMode);

        while (!QuitGame)
        {
            CurrentMillis = HAL_GetTick();

            if ((CurrentMillis - LastMillis) >= 17)
            {
                LastMillis = CurrentMillis;

                if (GameList[GameNumber]->GameKeyUp    <= 0xF) chip8UpdateKey(GameList[GameNumber]->GameKeyUp,    buttons_get() & B_Up);
                if (GameList[GameNumber]->GameKeyDown  <= 0xF) chip8UpdateKey(GameList[GameNumber]->GameKeyDown,  buttons_get() & B_Down);
                if (GameList[GameNumber]->GameKeyLeft  <= 0xF) chip8UpdateKey(GameList[GameNumber]->GameKeyLeft,  buttons_get() & B_Left);
                if (GameList[GameNumber]->GameKeyRight <= 0xF) chip8UpdateKey(GameList[GameNumber]->GameKeyRight, buttons_get() & B_Right);
                if (GameList[GameNumber]->GameKeyB     <= 0xF) chip8UpdateKey(GameList[GameNumber]->GameKeyB,     buttons_get() & B_B);
                if (GameList[GameNumber]->GameKeyA     <= 0xF) chip8UpdateKey(GameList[GameNumber]->GameKeyA,     buttons_get() & B_A);
                if (GameList[GameNumber]->GameKeyTime  <= 0xF) chip8UpdateKey(GameList[GameNumber]->GameKeyTime,  buttons_get() & B_TIME);
                if (GameList[GameNumber]->GameKeyPause <= 0xF) chip8UpdateKey(GameList[GameNumber]->GameKeyPause, buttons_get() & B_PAUSE);

                chip8UpdateTimers();
                chip8StepCpu((GameList[GameNumber]->GameFrequency) / 60);

                if (chip8GetAudio())
                {
                    HAL_SAI_DMAResume(hsai_BlockA1);
                }
                else
                {
                    HAL_SAI_DMAPause(hsai_BlockA1);
                }

                displayRefresh(chip8GetScreen());

                if (buttons_get() & B_GAME)
                {
                    QuitGame = 1;

                    while (buttons_get() & B_GAME);
                }
            }
        }
    }
}
