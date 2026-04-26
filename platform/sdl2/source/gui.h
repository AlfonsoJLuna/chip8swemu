#ifndef GUI_H
#define GUI_H


#include <SDL.h>
#include <stdbool.h>


#ifdef __cplusplus
    extern "C" {
#endif


void GUI_Init(SDL_Window* Window, SDL_Renderer* Renderer);

void GUI_ProcessEvents(SDL_Event* Events);

bool GUI_GetFlagQuit();

void GUI_ShowUnknownOp();

void GUI_Render();

void GUI_Quit();


#ifdef __cplusplus
    }
#endif


#endif  // GUI_H
