#ifndef VIDEO_H
#define VIDEO_H


#include "config.h"
#include <stdbool.h>
#include <stdint.h>


bool initializeSDL(configuration* config);

bool createWindow(configuration* config);

void renderScreen(configuration* config, uint8_t* screen);

void finalizeSDL();


#endif  // VIDEO_H

