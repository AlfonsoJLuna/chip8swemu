#ifndef GUI_H
#define GUI_H


#include "config.h"
#include <SDL.h>


#ifdef __cplusplus
    extern "C" {
#endif


void GUI_Initialize(SDL_Window* window);

void GUI_GetInput(SDL_Event* event);

void GUI_ProcessElements(SDL_Window* window, configuration* config);

void GUI_Render();

void GUI_Finalize();


#ifdef __cplusplus
    }
#endif


#endif  // GUI_H

