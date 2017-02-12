#include "input.h"
#include "gui.h"
#include <SDL.h>


SDL_Event events;


void processInput(bool* quit, bool* minimized, bool* keyboard)
{
    while (SDL_PollEvent(&events))
    {
        GUI_GetInput(&events);

        switch (events.type)
        {
            case SDL_QUIT:
                *quit = true;
            break;

            case SDL_WINDOWEVENT:
                switch (events.window.event)
                {
                    case SDL_WINDOWEVENT_MINIMIZED:
                        *minimized = true;
                    break;

                    case SDL_WINDOWEVENT_MAXIMIZED:
                        *minimized = false;
                    break;

                    case SDL_WINDOWEVENT_RESTORED:
                        *minimized = false;
                    break;
                }
            break;

            case SDL_KEYDOWN:
                switch (events.key.keysym.scancode)
                {
                    case SDL_SCANCODE_X:    keyboard[0x0] = 1;    break;
                    case SDL_SCANCODE_1:    keyboard[0x1] = 1;    break;
                    case SDL_SCANCODE_2:    keyboard[0x2] = 1;    break;
                    case SDL_SCANCODE_3:    keyboard[0x3] = 1;    break;
                    case SDL_SCANCODE_Q:    keyboard[0x4] = 1;    break;
                    case SDL_SCANCODE_W:    keyboard[0x5] = 1;    break;
                    case SDL_SCANCODE_E:    keyboard[0x6] = 1;    break;
                    case SDL_SCANCODE_A:    keyboard[0x7] = 1;    break;
                    case SDL_SCANCODE_S:    keyboard[0x8] = 1;    break;
                    case SDL_SCANCODE_D:    keyboard[0x9] = 1;    break;
                    case SDL_SCANCODE_Z:    keyboard[0xA] = 1;    break;
                    case SDL_SCANCODE_C:    keyboard[0xB] = 1;    break;
                    case SDL_SCANCODE_4:    keyboard[0xC] = 1;    break;
                    case SDL_SCANCODE_R:    keyboard[0xD] = 1;    break;
                    case SDL_SCANCODE_F:    keyboard[0xE] = 1;    break;
                    case SDL_SCANCODE_V:    keyboard[0xF] = 1;    break;
                    default:                                      break;
                }
            break;

            case SDL_KEYUP:
                switch (events.key.keysym.scancode)
                {
                    case SDL_SCANCODE_X:    keyboard[0x0] = 0;    break;
                    case SDL_SCANCODE_1:    keyboard[0x1] = 0;    break;
                    case SDL_SCANCODE_2:    keyboard[0x2] = 0;    break;
                    case SDL_SCANCODE_3:    keyboard[0x3] = 0;    break;
                    case SDL_SCANCODE_Q:    keyboard[0x4] = 0;    break;
                    case SDL_SCANCODE_W:    keyboard[0x5] = 0;    break;
                    case SDL_SCANCODE_E:    keyboard[0x6] = 0;    break;
                    case SDL_SCANCODE_A:    keyboard[0x7] = 0;    break;
                    case SDL_SCANCODE_S:    keyboard[0x8] = 0;    break;
                    case SDL_SCANCODE_D:    keyboard[0x9] = 0;    break;
                    case SDL_SCANCODE_Z:    keyboard[0xA] = 0;    break;
                    case SDL_SCANCODE_C:    keyboard[0xB] = 0;    break;
                    case SDL_SCANCODE_4:    keyboard[0xC] = 0;    break;
                    case SDL_SCANCODE_R:    keyboard[0xD] = 0;    break;
                    case SDL_SCANCODE_F:    keyboard[0xE] = 0;    break;
                    case SDL_SCANCODE_V:    keyboard[0xF] = 0;    break;
                    default:                                      break;
                }
            break;

            default:
            break;
        }
    }
}

