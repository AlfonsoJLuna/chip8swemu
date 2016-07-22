#include "audio.h"
#include <SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


SDL_AudioSpec* audiospec = NULL;
SDL_AudioDeviceID device = 0;

double wave_position = 0;
double wave_increment = 2 * M_PI * 1000 / 44100;


// Samples can vary between 0 and 255
// Sine wave varies between 120 and 133
void sineWave(void* userdata, Uint8* stream, int lenght)
{
    for (int i = 0; i < lenght; i++)
    {
        stream[i] = 7 * sin(wave_position) + 127;
        wave_position += wave_increment;
    }
}


bool initializeAudio()
{
    audiospec = (SDL_AudioSpec*)malloc(sizeof(SDL_AudioSpec));
    audiospec->freq = 44100;
    audiospec->format = AUDIO_U8;
    audiospec->channels = 1;
    audiospec->samples = 4096;
    audiospec->callback = sineWave;

    device = SDL_OpenAudioDevice(NULL, 0, audiospec, NULL, 0);

    if (!device)
    {
        printf("Error: Failed to initialize audio device: %s\n", SDL_GetError());
        return 1;
    }
    return 0;
}


void updateAudio(bool enable)
{
    SDL_PauseAudioDevice(device, !enable);
}


void finalizeAudio()
{
    if (device != 0)
    {
        SDL_CloseAudioDevice(device);
        device = 0;
    }

    if (audiospec != NULL)
    {
        free(audiospec);
        audiospec = NULL;
    }
}

