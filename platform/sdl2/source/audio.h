#ifndef AUDIO_H
#define AUDIO_H


#include <stdbool.h>


bool Audio_Init();

void Audio_Update(bool beep);

void Audio_Quit();


#endif  // AUDIO_H
