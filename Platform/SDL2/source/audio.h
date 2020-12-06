#ifndef AUDIO_H
#define AUDIO_H


#include <stdbool.h>


bool audioInitialize();

void audioMute(bool mute);

void audioUpdate(bool beep);

void audioFinalize();


#endif  // AUDIO_H
