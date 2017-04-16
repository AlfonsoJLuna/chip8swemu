#include "config.h"

#include <stdio.h>


static int chip8_cpu_freq;

static window_size_t window_size;

static color_t color_backgr;
static color_t color_accent;


void configSetDefaults()
{
    chip8_cpu_freq = 840;
    window_size = (window_size_t) { .width = 640, .height = 339 };
    color_backgr = (color_t) { .red = 0x11, .green = 0x11, .blue = 0x11 };
    color_accent = (color_t) { .red = 0xEE, .green = 0xEE, .blue = 0xEE };
}


void configSetCpuFreq(int freq)
{
    chip8_cpu_freq = freq;
}


void configSetWindowSize(int width, int height)
{
    window_size = (window_size_t) { .width = width, .height = height };
}


void configSetColorBackground(uint8_t red, uint8_t green, uint8_t blue)
{
    color_backgr = (color_t) { .red = red, .green = green, .blue = blue };
}


void configSetColorAccent(uint8_t red, uint8_t green, uint8_t blue)
{
    color_accent = (color_t) { .red = red, .green = green, .blue = blue };
}


int configGetCpuFreq()
{
    return chip8_cpu_freq;
}


window_size_t configGetWindowSize()
{
    return window_size;
}


color_t configGetColorBackground()
{
    return color_backgr;
}


color_t configGetColorAccent()
{
    return color_accent;
}


bool configLoadFromFile()
{
    FILE* file = fopen("config.ini", "r");

    if (file == NULL)
    {
        perror("Error");
        printf("Error: Configuration file could not be loaded.\n");
        return 1;
    }
    else
    {
        printf("Configuration file loaded successfully.\n");

        unsigned int cbr, cbg, cbb, car, cag, cab;

        fscanf(file,
            "chip8_cpu_freq=%d\n"
            "window_size.width=%d\n"
            "window_size.height=%d\n"
            "color_backgr.red=%X\n"
            "color_backgr.green=%X\n"
            "color_backgr.blue=%X\n"
            "color_accent.red=%X\n"
            "color_accent.green=%X\n"
            "color_accent.blue=%X\n",
            &chip8_cpu_freq,
            &window_size.width, &window_size.height,
            &cbr, &cbg, &cbb, &car, &cag, &cab);

        fclose(file);

        color_backgr = (color_t) { cbr & 0xFF, cbg & 0xFF, cbb & 0xFF };
        color_accent = (color_t) { car & 0xFF, cag & 0xFF, cab & 0xFF };

        // Check introduced values and replace them if they werent between reasonable limits
        if (chip8_cpu_freq < 60) chip8_cpu_freq = 60;
        if (window_size.width < 128) window_size.width = 128;
        if (window_size.height < 83) window_size.height = 83;
    }

    return 0;
}


bool configSaveToFile()
{
    FILE* file = fopen("config.ini", "w");

    if (file == NULL)
    {
        perror("Error");
        printf("Error: Configuration file could not be saved.\n");
        return 1;
    }
    else
    {
        fprintf(file,
            "chip8_cpu_freq=%d\n"
            "window_size.width=%d\n"
            "window_size.height=%d\n"
            "color_backgr.red=%X\n"
            "color_backgr.green=%X\n"
            "color_backgr.blue=%X\n"
            "color_accent.red=%X\n"
            "color_accent.green=%X\n"
            "color_accent.blue=%X\n",
            chip8_cpu_freq,
            window_size.width,
            window_size.height,
            color_backgr.red,
            color_backgr.green,
            color_backgr.blue,
            color_accent.red,
            color_accent.green,
            color_accent.blue);

        fclose(file);

        printf("Configuration file saved successfully.\n");
    }

    return 0;
}
