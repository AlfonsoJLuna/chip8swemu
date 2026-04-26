#include "config.h"

#include <inttypes.h>
#include <stdio.h>


static config_t Config;


static uint16_t Constrain(uint16_t Value, uint16_t Min, uint16_t Max)
{
    if (Value < Min)
    {
        Value = Min;
    }
    else if (Value > Max)
    {
        Value = Max;
    }

    return Value;
}

static config_t Config_Sanitize(config_t NewConfig)
{
    NewConfig.Frequency = Constrain(NewConfig.Frequency, 60, 65535);
    NewConfig.CompatibilityMode = Constrain(NewConfig.CompatibilityMode, 0, 1);
    NewConfig.WindowWidth = Constrain(NewConfig.WindowWidth, 128, 7680);
    NewConfig.WindowHeight = Constrain(NewConfig.WindowHeight, 90, 4320);
    NewConfig.EnableVSync = Constrain(NewConfig.EnableVSync, 0, 1);
    NewConfig.EnableFullscreen = Constrain(NewConfig.EnableFullscreen, 0, 1);
    NewConfig.EnableGUI = Constrain(NewConfig.EnableGUI, 0, 1);
    NewConfig.Stretch = Constrain(NewConfig.Stretch, 0, 1);
    NewConfig.Mute = Constrain(NewConfig.Mute, 0, 1);

    return NewConfig;
}

void Config_Set(config_t NewConfig)
{
    Config = Config_Sanitize(NewConfig);
}

void Config_SetDefaults()
{
    config_t Default;

    Default.Frequency = 840;
	Default.CompatibilityMode = 0;
	Default.WindowWidth = 6 * 128;
	Default.WindowHeight = 6 * 64;
	Default.EnableVSync = 0;
    Default.EnableFullscreen = 0;
    Default.EnableGUI = 1;
    Default.Stretch = 0;
	Default.Mute = 0;
    Default.ColorBackground.R = 0x00;
    Default.ColorBackground.G = 0x00;
    Default.ColorBackground.B = 0x00;
	Default.ColorAccent.R = 0xFF;
    Default.ColorAccent.G = 0xFF;
    Default.ColorAccent.B = 0xFF;

    Config_Set(Default);
}

config_t Config_Get()
{
    return Config;
}

int Config_LoadFromFile()
{
    FILE* file = fopen("config.ini", "r");

	if (file == NULL)
	{
        printf("Error: Cannot open config file.\n");
		return 1;
	}

    config_t LoadedConfig;

    fscanf(file,
        "Frequency=%"SCNu16"\n"
    	"CompatibilityMode=%"SCNu8"\n"
    	"WindowWidth=%"SCNu16"\n"
    	"WindowHeight=%"SCNu16"\n"
    	"EnableVSync=%"SCNu8"\n"
        "EnableFullscreen=%"SCNu8"\n"
        "EnableGUI=%"SCNu8"\n"
        "Stretch=%"SCNu8"\n"
    	"Mute=%"SCNu8"\n"
    	"ColorBackground.R=%"SCNx8"\n"
        "ColorBackground.G=%"SCNx8"\n"
        "ColorBackground.B=%"SCNx8"\n"
    	"ColorAccent.R=%"SCNx8"\n"
        "ColorAccent.G=%"SCNx8"\n"
        "ColorAccent.B=%"SCNx8"\n",
        &LoadedConfig.Frequency,
    	&LoadedConfig.CompatibilityMode,
    	&LoadedConfig.WindowWidth,
    	&LoadedConfig.WindowHeight,
    	&LoadedConfig.EnableVSync,
        &LoadedConfig.EnableFullscreen,
        &LoadedConfig.EnableGUI,
        &LoadedConfig.Stretch,
    	&LoadedConfig.Mute,
    	&LoadedConfig.ColorBackground.R,
        &LoadedConfig.ColorBackground.G,
        &LoadedConfig.ColorBackground.B,
    	&LoadedConfig.ColorAccent.R,
        &LoadedConfig.ColorAccent.G,
        &LoadedConfig.ColorAccent.B);

	fclose(file);

    Config_Set(LoadedConfig);

	return 0;
}


int Config_SaveToFile()
{
    FILE* file = fopen("config.ini", "w");

	if (file == NULL)
	{
        printf("Error: Cannot open config file.\n");
		return 1;
	}

	fprintf(file,
        "Frequency=%u\n"
    	"CompatibilityMode=%u\n"
    	"WindowWidth=%u\n"
    	"WindowHeight=%u\n"
    	"EnableVSync=%u\n"
        "EnableFullscreen=%u\n"
        "EnableGUI=%u\n"
        "Stretch=%u\n"
    	"Mute=%u\n"
    	"ColorBackground.R=%X\n"
        "ColorBackground.G=%X\n"
        "ColorBackground.B=%X\n"
    	"ColorAccent.R=%X\n"
        "ColorAccent.G=%X\n"
        "ColorAccent.B=%X\n",
        Config.Frequency,
    	Config.CompatibilityMode,
    	Config.WindowWidth,
    	Config.WindowHeight,
    	Config.EnableVSync,
        Config.EnableFullscreen,
        Config.EnableGUI,
        Config.Stretch,
    	Config.Mute,
    	Config.ColorBackground.R,
        Config.ColorBackground.G,
        Config.ColorBackground.B,
    	Config.ColorAccent.R,
        Config.ColorAccent.G,
        Config.ColorAccent.B);

	fclose(file);

	return 0;
}
