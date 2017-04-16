#ifndef CONFIG_H
#define CONFIG_H


#include <stdbool.h>
#include <stdint.h>


typedef struct
{
    int width;
    int height;
} window_size_t;


typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} color_t;


void configSetDefaults();


void configSetCpuFreq(int freq);

void configSetWindowSize(int width, int height);

void configSetColorBackground(uint8_t red, uint8_t green, uint8_t blue);

void configSetColorAccent(uint8_t red, uint8_t green, uint8_t blue);


int configGetCpuFreq();

window_size_t configGetWindowSize();

color_t configGetColorBackground();

color_t configGetColorAccent();


bool configLoadFromFile();

bool configSaveToFile();


#endif  // CONFIG_H
