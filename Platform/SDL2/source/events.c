#include "events.h"

#include "chip8.h"
#include "gui.h"
#include "rom.h"
#include <SDL.h>


SDL_Event events;


bool eventsProcess()
{
    bool quit = false;

    while (SDL_PollEvent(&events))
    {
        guiProcessInput(&events);

        switch (events.type)
        {
            case SDL_QUIT:
                quit = true;
            break;

            case SDL_DROPFILE:
                romLoadFromPath(events.drop.file);
                SDL_free(events.drop.file);
            break;

            case SDL_KEYDOWN:
                switch (events.key.keysym.scancode)
                {
                    case SDL_SCANCODE_X:    chip8UpdateKey(0x0, true);      break;
                    case SDL_SCANCODE_1:    chip8UpdateKey(0x1, true);      break;
                    case SDL_SCANCODE_2:    chip8UpdateKey(0x2, true);      break;
                    case SDL_SCANCODE_3:    chip8UpdateKey(0x3, true);      break;
                    case SDL_SCANCODE_Q:    chip8UpdateKey(0x4, true);      break;
                    case SDL_SCANCODE_W:    chip8UpdateKey(0x5, true);      break;
                    case SDL_SCANCODE_E:    chip8UpdateKey(0x6, true);      break;
                    case SDL_SCANCODE_A:    chip8UpdateKey(0x7, true);      break;
                    case SDL_SCANCODE_S:    chip8UpdateKey(0x8, true);      break;
                    case SDL_SCANCODE_D:    chip8UpdateKey(0x9, true);      break;
                    case SDL_SCANCODE_Z:    chip8UpdateKey(0xA, true);      break;
                    case SDL_SCANCODE_C:    chip8UpdateKey(0xB, true);      break;
                    case SDL_SCANCODE_4:    chip8UpdateKey(0xC, true);      break;
                    case SDL_SCANCODE_R:    chip8UpdateKey(0xD, true);      break;
                    case SDL_SCANCODE_F:    chip8UpdateKey(0xE, true);      break;
                    case SDL_SCANCODE_V:    chip8UpdateKey(0xF, true);      break;
                    default:                                                break;
                }
            break;

            case SDL_KEYUP:
                switch (events.key.keysym.scancode)
                {
                    case SDL_SCANCODE_X:    chip8UpdateKey(0x0, false);      break;
                    case SDL_SCANCODE_1:    chip8UpdateKey(0x1, false);      break;
                    case SDL_SCANCODE_2:    chip8UpdateKey(0x2, false);      break;
                    case SDL_SCANCODE_3:    chip8UpdateKey(0x3, false);      break;
                    case SDL_SCANCODE_Q:    chip8UpdateKey(0x4, false);      break;
                    case SDL_SCANCODE_W:    chip8UpdateKey(0x5, false);      break;
                    case SDL_SCANCODE_E:    chip8UpdateKey(0x6, false);      break;
                    case SDL_SCANCODE_A:    chip8UpdateKey(0x7, false);      break;
                    case SDL_SCANCODE_S:    chip8UpdateKey(0x8, false);      break;
                    case SDL_SCANCODE_D:    chip8UpdateKey(0x9, false);      break;
                    case SDL_SCANCODE_Z:    chip8UpdateKey(0xA, false);      break;
                    case SDL_SCANCODE_C:    chip8UpdateKey(0xB, false);      break;
                    case SDL_SCANCODE_4:    chip8UpdateKey(0xC, false);      break;
                    case SDL_SCANCODE_R:    chip8UpdateKey(0xD, false);      break;
                    case SDL_SCANCODE_F:    chip8UpdateKey(0xE, false);      break;
                    case SDL_SCANCODE_V:    chip8UpdateKey(0xF, false);      break;
                    default:                                                 break;
                }
            break;

            default:
            break;
        }
    }

    return quit;
}
