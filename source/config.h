#ifndef CONFIG_H
#define CONFIG_H


#include <stdbool.h>
#include <stdint.h>


typedef struct
{
    // Paths
    char* program_path;
    char rom_path[260];

    // Window size
    unsigned int window_size_multiplier;
    int window_width;
    int window_height;

    // Video parameters
    int screen_refresh_rate;
    int instructions_per_second;
    int ips_min;
    int ips_max;
    uint32_t background_color_bgr;
    uint32_t accent_color_bgr;

    // Settings flags
    bool load_rom;
    bool quit;
    bool reset;
    bool ips_default;
    bool window_10x;
    bool window_15x;
    bool window_20x;
    bool mute_sound;
} configuration;


void setDefaultConfig(configuration* config);


// Converts colors from RGB to BGR and viceversa
uint32_t colorYGX(uint32_t colorXGY);


#endif  // CONFIG_H

