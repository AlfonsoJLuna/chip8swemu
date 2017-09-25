#include "video.h"

#include "chip8.h"
#include "config.h"
#include "gui.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <stdint.h>
#include <stdio.h>


SDL_Window* window = NULL;
SDL_GLContext glcontext;

// Texture of 64 rows of 128 columns of 3 bytes (R, G, B)
uint8_t texture[64][128][3] = {0};

bool enable_vsync;

double last_time = 0;


bool videoInitialize()
{
    // Init SDL subsystems
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window with an OpenGL context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    window_size_t window_size = configGetWindowSize();

    window = SDL_CreateWindow("chip8swemu v1.1.1",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        window_size.width, window_size.height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (window == NULL)
    {
        printf("Error: %s\n", SDL_GetError());
        return 1;
    }

    glcontext = SDL_GL_CreateContext(window);

    // Disable V-Sync
    SDL_GL_SetSwapInterval(0);

    // Specify the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 64, 0,
        GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)texture);

    // Configure the texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Enable textures
    glEnable(GL_TEXTURE_2D);

    guiInitialize(window);

    return 0;
}


void videoToggleVsync(bool enable)
{
    SDL_GL_SetSwapInterval(enable);
    enable_vsync = enable;
}


void videoRender()
{
    window_size_t window_size;
    SDL_GetWindowSize(window, &window_size.width, &window_size.height);
    configSetWindowSize(window_size.width, window_size.height);

    guiProcessElements(window);

    // Set viewport
    glViewport(0, 0, window_size.width, window_size.height);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Update texture on memory
    color_t color_backgr = configGetColorBackground();
    color_t color_accent = configGetColorAccent();

    for (int y = 0; y < 64; y++)
    {
        for (int x = 0; x < 128; x++)
        {
            if (chip8GetPixel((63 - y), x))
            {
                texture[y][x][0] = color_accent.red;
                texture[y][x][1] = color_accent.green;
                texture[y][x][2] = color_accent.blue;
            }
            else
            {
                texture[y][x][0] = color_backgr.red;
                texture[y][x][1] = color_backgr.green;
                texture[y][x][2] = color_backgr.blue;
            }
        }
    }

    // Send texture to GPU
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 128, 64,
        GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)texture);

    // Render the texture
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(-1.0f, -1.0f);

        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(1.0f, -1.0f);

        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(1.0f, (float)(window_size.height - 19 * 2) / window_size.height);

        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(-1.0f, (float)(window_size.height - 19 * 2) / window_size.height);
    glEnd();

    guiRender();

    double current_time = SDL_GetPerformanceCounter() / SDL_GetPerformanceFrequency();
    double busy_time = current_time - last_time;
    if (!enable_vsync && ((busy_time * 1000) < (1000 / 60)))
        SDL_Delay(1000 / 60 - busy_time * 1000);
    last_time = SDL_GetPerformanceCounter() / SDL_GetPerformanceFrequency();

    SDL_GL_SwapWindow(window);
}


void videoFinalize()
{
    guiFinalize();

    SDL_GL_DeleteContext(glcontext);

    if (window != NULL)
    {
        SDL_DestroyWindow(window);
        window = NULL;
    }

    SDL_Quit();
}
