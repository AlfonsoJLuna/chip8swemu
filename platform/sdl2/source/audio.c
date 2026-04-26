#include "audio.h"

#include "config.h"

#include <SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#define PI 3.14159265358979323846


SDL_AudioSpec* audiospec = NULL;
SDL_AudioDeviceID device = 0;

static double wave_position = 0;
static double wave_increment = 2 * PI * 1000 / 44100;


// Samples can vary between -127 and 128
// Sine wave varies between -50 and 50
static void sineWave(void* userdata, Uint8* stream, int lenght)
{
    for (int i = 0; i < lenght; i++)
    {
        stream[i] = 50 * sin(wave_position);
        wave_position += wave_increment;
    }
}

bool Audio_Init()
{
    audiospec = (SDL_AudioSpec*)malloc(sizeof(SDL_AudioSpec));
    audiospec->freq = 44100;
    audiospec->format = AUDIO_S8;
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

void Audio_Update(bool beep)
{
    config_t Config = Config_Get();

    SDL_PauseAudioDevice(device, !beep || Config.Mute);
}

void Audio_Quit()
{
    SDL_PauseAudioDevice(device, true);

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
