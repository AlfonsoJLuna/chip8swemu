#ifndef GUI_H
#define GUI_H


#include <SDL.h>
#include <stdbool.h>


#ifdef __cplusplus
    extern "C" {
#endif


void guiInitialize(SDL_Window* window);

void guiProcessInput(SDL_Event* events);

void guiProcessElements(SDL_Window* window);

bool guiGetFlagQuit();

void guiShowUnknownOp();

void guiRender();

void guiFinalize();


#ifdef __cplusplus
    }
#endif


#endif  // GUI_H
