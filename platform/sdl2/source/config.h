#ifndef CONFIG_H
#define CONFIG_H


#include <stdint.h>


typedef struct
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
} color_t;

typedef struct
{
    uint16_t    Frequency;
    uint8_t     CompatibilityMode;
    uint16_t    WindowWidth;
    uint16_t    WindowHeight;
    uint8_t     EnableVSync;
    uint8_t     EnableFullscreen;
    uint8_t     EnableGUI;
    uint8_t     Stretch;
    uint8_t     Mute;
    color_t     ColorBackground;
    color_t     ColorAccent;
} config_t;


void        Config_SetDefaults();

void        Config_Set(config_t NewConfig);

config_t    Config_Get();

int         Config_LoadFromFile();

int         Config_SaveToFile();


#endif  // CONFIG_H
