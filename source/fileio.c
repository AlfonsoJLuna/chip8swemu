#include "fileio.h"
#include <stdio.h>


#ifdef _WIN32


#include <shlobj.h>
#include <commdlg.h>


bool getRomPath(char* rom_path)
{
    OPENFILENAME ofn;

    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = rom_path;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = 260;
    ofn.lpstrFilter = "CHIP-8 Roms\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = "roms";
    ofn.lpstrTitle = "Select ROM";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    return !GetOpenFileName(&ofn);
}


#elif __linux__


#include <gtk/gtk.h>


bool getRomPath(char* rom_path)
{
    // To do
    return 1;
}


#else


#error Platform not supported


#endif


bool loadRomFromPath(char* rom_path, uint8_t* memory)
{
    // Open ROM file in read and binary mode
    FILE* ROM = fopen(rom_path, "rb");

    if (ROM == NULL)
    {
        perror("Error");
        printf("Error: ROM file could not be opened.\n");
        return 1;
    }
    printf("ROM file opened successfully.\n");

    // Obtain ROM file size
    fseek(ROM, 0, SEEK_END);
    unsigned long int rom_size = ftell(ROM);
    rewind(ROM);
    printf("ROM size is %lu bytes.\n", rom_size);

    // Check if ROM file size fits CHIP-8 memory
    if (rom_size > 3584)
    {
        printf("Error: ROM file is too large. Max file size is 3584 bytes.\n");
        fclose(ROM);
        return 1;
    }

    // Copy ROM to CHIP-8 memory
    unsigned long int result = fread(&memory[512], 1, rom_size, ROM);
    if (result != rom_size)
    {
        printf("Error: Program could not be loaded into memory.\n");
        fclose(ROM);
        return 1;
    }

    // Everything was OK
    printf("Program loaded into memory.\n");
    fclose(ROM);

    return 0;
}


bool loadConfig(configuration* config)
{
    // Get config.ini path
    char config_path[260];
    snprintf(config_path, sizeof(config_path), "%s%s", config->program_path, "config.ini");

    // Load new values from config.ini if the file exists
    FILE* file = fopen(config_path, "r");

    if (file == NULL)
    {
        perror("Error");
        printf("Error: Configuration file could not be loaded.\n");
        return 1;
    }
    else
    {
        printf("Configuration file loaded successfully.\n");

        unsigned int new_window_size_multiplier;
        unsigned int new_instructions_per_second;
        uint32_t new_background_color_rgb;
        uint32_t new_accent_color_rgb;

        fscanf(file,
            "window_size_multiplier=%u\n"
            "instructions_per_second=%d\n"
            "background_color_rgb=%x\n"
            "accent_color_rgb=%x\n",
            &new_window_size_multiplier,
            &new_instructions_per_second,
            &new_background_color_rgb,
            &new_accent_color_rgb);

        fclose(file);

        // Replace default values with new values if they are between reasonable limits
        if (10 <= new_window_size_multiplier && new_window_size_multiplier <= 80)
            config->window_size_multiplier = new_window_size_multiplier;

        if (60 <= new_instructions_per_second && new_instructions_per_second <= 3000)
            config->instructions_per_second = new_instructions_per_second;

        if (new_background_color_rgb <= 0xFFFFFF)
            config->background_color_bgr = colorYGX(new_background_color_rgb);

        if (new_accent_color_rgb <= 0xFFFFFF)
            config->accent_color_bgr = colorYGX(new_accent_color_rgb);
    }

    // Print final values
    printf("Configuration variables:\n"
        "    Window size multiplier: %u\n"
        "    Instructions per second: %d\n"
        "    Background color: %06X\n"
        "    Accent color: %06X\n",
        config->window_size_multiplier,
        config->instructions_per_second,
        config->background_color_bgr,
        config->accent_color_bgr);

    return 0;
}


bool saveConfig(configuration* config)
{
    // Get config.ini path
    char config_path[260];
    snprintf(config_path, sizeof(config_path), "%s%s", config->program_path, "config.ini");

    FILE* file = fopen(config_path, "w");

    if (file == NULL)
    {
        perror("Error");
        printf("Error: Configuration file could not be saved.\n");
        return 1;
    }
    else
    {
        fprintf(file,
            "window_size_multiplier=%u\n"
            "instructions_per_second=%d\n"
            "background_color_rgb=%06X\n"
            "accent_color_rgb=%06X\n",
            config->window_size_multiplier,
            config->instructions_per_second,
            colorYGX(config->background_color_bgr),
            colorYGX(config->accent_color_bgr));

        fclose(file);

        printf("Configuration file saved successfully.\n");
    }
    return 0;
}

