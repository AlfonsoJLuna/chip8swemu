#include "config.h"


void setDefaultConfig(configuration* config)
{
    config->instructions_per_second = 840;
    config->window_size_multiplier = 10;
    config->background_color_bgr = 0x111111;
    config->accent_color_bgr = 0xEEEEEE;

    config->load_rom = false;
    config->quit = false;
    config->reset = false;
    config->ips_default = false;
    config->window_5x = false;
    config->window_10x = false;
    config->window_15x = false;
    config->minimized = false;
    config->mute_sound = false;
}


uint32_t colorYGX(uint32_t colorXGY)
{
    return (colorXGY & 0xFF0000) >> 16 | (colorXGY & 0x00FF00) | (colorXGY & 0x0000FF) << 16;
}

