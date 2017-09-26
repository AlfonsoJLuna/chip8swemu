#include "config.h"

#include "chip8.h"
#include <stdio.h>


static config_t config;


static config_t sanitizeValues(config_t new_config)
{
    if (new_config.frequency < 60)
    {
        new_config.frequency = 60;
    }

    if (new_config.comp_mode > 1)
    {
        new_config.comp_mode = 1;
    }

    if (new_config.vert_wrap > 1)
    {
        new_config.vert_wrap = 1;
    }

    if (new_config.width < 128)
    {
        new_config.width = 128;
    }

    if (new_config.height < 83)
    {
        new_config.height = 83;
    }

    if (new_config.vsync > 1)
    {
        new_config.vsync = 1;
    }

    if (new_config.mute > 1)
    {
        new_config.mute = 1;
    }

    return new_config;
}


void configSetDefaults()
{
    config_t defaults;

    defaults.frequency = 840;
	defaults.comp_mode = 0;
	defaults.vert_wrap = 1;
	defaults.width = 640;
	defaults.height = 339;
	defaults.vsync = 0;
	defaults.mute = 0;
	defaults.accent.red = 0xFF;
    defaults.accent.green = 0xFF;
    defaults.accent.blue = 0xFF;
	defaults.background.red = 0x00;
    defaults.background.green = 0x00;
    defaults.background.blue = 0x00;

    configSet(defaults);

    configSetDefaults_old();
}


void configSet(config_t new_config)
{
    config = sanitizeValues(new_config);
}


config_t configGet()
{
    return config;
}


int configLoadFromFile()
{
    FILE* file = fopen("config.ini", "r");

	if (file == NULL)
	{
		return 1;
	}

    config_t new_config;

    fscanf(file,
        "frequency=%u\n"
    	"comp_mode=%u\n"
    	"vert_wrap=%u\n"
    	"width=%u\n"
    	"height=%u\n"
    	"vsync=%u\n"
    	"mute=%u\n"
    	"accent.red=%X\n"
        "accent.green=%X\n"
        "accent.blue=%X\n"
    	"background.red=%X\n"
        "background.green=%X\n"
        "background.blue=%X\n",
        &new_config.frequency,
    	&new_config.comp_mode,
    	&new_config.vert_wrap,
    	&new_config.width,
    	&new_config.height,
    	&new_config.vsync,
    	&new_config.mute,
    	&new_config.accent.red,
        &new_config.accent.green,
        &new_config.accent.blue,
    	&new_config.background.red,
        &new_config.background.green,
        &new_config.background.blue);

	fclose(file);

    configSet(new_config);

	return 0;
}


int configSaveToFile()
{
    FILE* file = fopen("config.ini", "w");

	if (file == NULL)
	{
		return 1;
	}

	fprintf(file,
        "frequency=%u\n"
    	"comp_mode=%u\n"
    	"vert_wrap=%u\n"
    	"width=%u\n"
    	"height=%u\n"
    	"vsync=%u\n"
    	"mute=%u\n"
    	"accent.red=%X\n"
        "accent.green=%X\n"
        "accent.blue=%X\n"
    	"background.red=%X\n"
        "background.green=%X\n"
        "background.blue=%X\n",
        config.frequency,
    	config.comp_mode,
    	config.vert_wrap,
    	config.width,
    	config.height,
    	config.vsync,
    	config.mute,
    	config.accent.red,
        config.accent.green,
        config.accent.blue,
    	config.background.red,
        config.background.green,
        config.background.blue);

	fclose(file);

	return 0;
}













// DEPRECATED










static int chip8_cpu_freq;

static window_size_t window_size;

static color_t color_backgr;
static color_t color_accent;


void configSetDefaults_old()
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
