#ifndef VIDEO_H
#define VIDEO_H


#include <stdbool.h>
#include <stdint.h>


bool Video_Init();

void Video_EnableFullscreen(bool enable);

void Video_SetWindowSize(bool gui_enabled, uint16_t width, uint16_t height);

void Video_Render();

void Video_Quit();


#endif  // VIDEO_H
