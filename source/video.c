#include "video.h"
#include "gui.h"
#include "chip8.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


SDL_DisplayMode display_mode;
SDL_Window* window = NULL;
SDL_GLContext glcontext;
uint8_t texture[64][128][3];     // Create a texture of 64 rows of 128 columns of 3 bytes (R, G, B)


bool initializeSDL(configuration* config)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return 1;
    }

    config->program_path = SDL_GetBasePath();
    printf("Program path: %s\n", config->program_path);

    return 0;
}


bool createWindow(configuration* config)
{
    // Create a window with an OpenGL context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    int screen_width = 128 * config->window_size_multiplier;
    int screen_height = 64 * config->window_size_multiplier + 19;

    window = SDL_CreateWindow("chip8swemu v0.1.0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (window == NULL)
    {
        printf("Error: %s\n", SDL_GetError());
        return 1;
    }

    glcontext = SDL_GL_CreateContext(window);

    // Enable V-Sync
    SDL_GL_SetSwapInterval(1);

    // Get screen refresh rate
    SDL_GetCurrentDisplayMode(0, &display_mode);
    config->screen_refresh_rate = display_mode.refresh_rate;
    printf("Screen refresh rate: %dHz\n", config->screen_refresh_rate);
    if (config->screen_refresh_rate < 60 || config->screen_refresh_rate > 840)
    {
        printf("Error: Invalid screen refresh rate\n");
        return 1;
    }

    // Set minimum and maximum instruction/second rates
    config->ips_min = config->screen_refresh_rate;
    config->ips_max = 3000;
    if (config->instructions_per_second < config->screen_refresh_rate)
    {
        config->instructions_per_second = config->screen_refresh_rate;
    }

    // Clear the texture
    for (int y = 0; y < 64; y++)
    {
        for (int x = 0; x < 128; x++)
        {
            texture[y][x][0] = config->background_color_bgr & 0x0000FF;
            texture[y][x][1] = (config->background_color_bgr & 0x00FF00) >> 8;
            texture[y][x][2] = (config->background_color_bgr & 0xFF0000) >> 16;
        }
    }

    // Specify the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)texture);

    // Configure the texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Enable textures
    glEnable(GL_TEXTURE_2D);

    // Initialize GUI
    GUI_Initialize(window);

    return 0;
}


void renderScreen(configuration* config)
{
    if (!config->minimized)
    {
        SDL_GetWindowSize(window, &config->window_width, &config->window_height);

        GUI_ProcessElements(window, config);

        // Set viewport
        glViewport(0, 0, config->window_width, config->window_height);
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        // Update texture on memory
        for (int y = 0; y < 64; y++)
        {
            for (int x = 0; x < 128; x++)
            {
                if (chip8GetPixel((63 - y), x))
                {
                    texture[y][x][0] = config->accent_color_bgr & 0x0000FF;
                    texture[y][x][1] = (config->accent_color_bgr & 0x00FF00) >> 8;
                    texture[y][x][2] = (config->accent_color_bgr & 0xFF0000) >> 16;
                }
                else
                {
                    texture[y][x][0] = config->background_color_bgr & 0x0000FF;
                    texture[y][x][1] = (config->background_color_bgr & 0x00FF00) >> 8;
                    texture[y][x][2] = (config->background_color_bgr & 0xFF0000) >> 16;
                }
            }
        }

        // Send texture to GPU
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 128, 64, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)texture);

        // Render the texture
        glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);     glVertex2f(-1.0, -1.0);
            glTexCoord2f(1.0, 0.0);     glVertex2f(1.0, -1.0);
            glTexCoord2f(1.0, 1.0);     glVertex2f(1.0, (float)(config->window_height - 19 * 2) / config->window_height);
            glTexCoord2f(0.0, 1.0);     glVertex2f(-1.0, (float)(config->window_height - 19 * 2) / config->window_height);
        glEnd();

        GUI_Render();

        SDL_GL_SwapWindow(window);  // Waits for V-Sync
    }
    else
    {
        SDL_Delay(30);  // Wait without rendering
    }
}


void finalizeSDL()
{
    GUI_Finalize();

    SDL_GL_DeleteContext(glcontext);

    if (window != NULL)
    {
        SDL_DestroyWindow(window);
        window = NULL;
    }

    SDL_Quit();
}
