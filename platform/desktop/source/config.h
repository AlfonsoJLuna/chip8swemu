#ifndef CONFIG_H
#define CONFIG_H


#include <stdint.h>


typedef struct
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} color_t;


typedef struct
{
	uint16_t frequency;
	uint8_t comp_mode;
	uint8_t vert_wrap;
	uint16_t width;
	uint16_t height;
	uint8_t vsync;
	uint8_t mute;
	color_t accent;
	color_t background;
} config_t;


void configSetDefaults();

void configSet(config_t new_config);

config_t configGet();


int configLoadFromFile();

int configSaveToFile();


#endif
