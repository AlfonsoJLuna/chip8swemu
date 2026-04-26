#include "video.h"

#include "chip8.h"
#include "config.h"
#include "gui.h"
#include "icon.h"

#include <SDL.h>
#include <stdio.h>


static bool UseVSync = false;

static SDL_Window* Window = NULL;
static SDL_Renderer* Renderer = NULL;
static SDL_Texture* Texture = NULL;

static double LastTime = 0;


static bool InitWindow()
{
    config_t Config = Config_Get();

    int MenuThickness;

    if (Config.EnableGUI)
    {
        MenuThickness = 26;
    }
    else
    {
        MenuThickness = 0;
    }
    
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return 1;
    }

    Window = SDL_CreateWindow(
        "chip8swemu",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        Config.WindowWidth,
        Config.WindowHeight + MenuThickness,
        SDL_WINDOW_SHOWN |
        SDL_WINDOW_RESIZABLE
    );

    if (Window == NULL)
    {
        printf("Error: %s\n", SDL_GetError());
        return 1;
    }

    if (Config.EnableFullscreen)
    {
        SDL_SetWindowFullscreen(Window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }

    return 0;
}

static void InitIcon()
{
    SDL_Surface* Surface = NULL;

    Surface = SDL_CreateRGBSurfaceFrom(
        (void*)icon, 96, 96, 32, 96*4,
        0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000
    );

    SDL_SetWindowIcon(Window, Surface);
}

static bool InitRenderer()
{
    config_t Config = Config_Get();

    UseVSync = Config.EnableVSync;

    if (UseVSync)
    {
        Renderer = SDL_CreateRenderer(
            Window,
            -1,
            SDL_RENDERER_ACCELERATED |
            SDL_RENDERER_PRESENTVSYNC
        );
    }
    else
    {
        Renderer = SDL_CreateRenderer(
            Window,
            -1,
            SDL_RENDERER_ACCELERATED
        );
    }

    if (Renderer == NULL)
    {
        printf("Error: %s\n", SDL_GetError());
        return 1;
    }

    return 0;
}

static bool InitTexture()
{
    Texture = SDL_CreateTexture(
        Renderer,
        SDL_PIXELFORMAT_RGB888,
        SDL_TEXTUREACCESS_TARGET,
        128,
        64
    );

    if (Texture == NULL)
    {
        printf("Error: %s\n", SDL_GetError());
        return 1;
    }

    return 0;
}

bool Video_Init()
{
    if (InitWindow())
    {
        return 1;
    }

    InitIcon();

    if (InitRenderer())
    {
        return 1;
    }

    if (InitTexture())
    {
        return 1;
    }

    GUI_Init(Window, Renderer);

    return 0;
}

void Video_EnableFullscreen(bool enable)
{
    if (enable)
    {
        SDL_SetWindowFullscreen(Window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }
    else
    {
        SDL_SetWindowFullscreen(Window, 0);
    }
}

void Video_SetWindowSize(bool gui_enabled, uint16_t width, uint16_t height)
{
    if (gui_enabled)
    {
        SDL_SetWindowSize(Window, width, height + 26);
    }
    else
    {
        SDL_SetWindowSize(Window, width, height);
    }
}

static void Render_Chip8Display()
{
    config_t Config = Config_Get();
    
    SDL_SetRenderTarget(Renderer, Texture);
    SDL_SetRenderDrawColor(Renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(Renderer);

    for (int x = 0; x < 128; x++)
    {
        for (int y = 0; y < 64; y++)
        {
            if (chip8GetPixel(y, x))
            {
                SDL_SetRenderDrawColor(
                    Renderer,
                    Config.ColorAccent.R,
                    Config.ColorAccent.G,
                    Config.ColorAccent.B,
                    SDL_ALPHA_OPAQUE
                );
            }
            else
            {
                SDL_SetRenderDrawColor(
                    Renderer,
                    Config.ColorBackground.R,
                    Config.ColorBackground.G,
                    Config.ColorBackground.B,
                    SDL_ALPHA_OPAQUE
                );
            }

            SDL_RenderDrawPoint(Renderer, x, y);
        }
    }
}

static void Render_Chip8Texture()
{
    config_t Config = Config_Get();

    SDL_Rect Dest;
    int MenuThickness, WindowWidth, WindowHeight, MaxWidth, MaxHeight;
    
    if (Config.EnableGUI)
    {
        MenuThickness = 26;
    }
    else
    {
        MenuThickness = 0;
    }

    SDL_GetWindowSize(Window, &WindowWidth, &WindowHeight);

    if (Config.Stretch)
    {
        // Stretch to window
        MaxWidth = WindowWidth;
        MaxHeight = WindowHeight - MenuThickness;
    }
    else
    {
        // Keep aspect ratio
        if (WindowWidth >= ((WindowHeight - MenuThickness) * 2))
        {
            // Height is the limiting factor
            MaxHeight = WindowHeight - MenuThickness;
            MaxWidth = MaxHeight * 2;        
        }
        else
        {
            // Width is the limiting factor
            MaxWidth = WindowWidth;
            MaxHeight = WindowWidth / 2;
        }
    }

    Dest.x = (WindowWidth - MaxWidth) / 2;
    Dest.y = MenuThickness + ((WindowHeight - MenuThickness - MaxHeight) / 2);
    Dest.w = MaxWidth;
    Dest.h = MaxHeight;

    SDL_SetRenderTarget(Renderer, NULL);
    SDL_SetRenderDrawColor(Renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(Renderer);
    SDL_RenderCopy(Renderer, Texture, NULL, &Dest);
}

static void Wait()
{
    double FramePeriod = 1000.0 / 60.0; // ms

    double CurrentTime = SDL_GetPerformanceCounter() / SDL_GetPerformanceFrequency();

    double BusyTime = (CurrentTime - LastTime) * 1000.0;

    if (BusyTime < FramePeriod)
    {
        SDL_Delay(FramePeriod - BusyTime);
    }

    LastTime = SDL_GetPerformanceCounter() / SDL_GetPerformanceFrequency();
}

void Video_Render()
{
    config_t Config = Config_Get();

    // Render the CHIP-8 display into the texture
    Render_Chip8Display();

    // Render the CHIP-8 texture into the window
    Render_Chip8Texture();

    // Render the GUI into the window
    if (Config.EnableGUI)
    {
        GUI_Render();
    }

    // Update window
    SDL_SetRenderTarget(Renderer, NULL);
    SDL_RenderPresent(Renderer);

    // Limit the framerate if VSync is disabled
    if (UseVSync == false)
    {
        Wait();
    }
}

void Video_Quit()
{
    GUI_Quit();
    
    if (Texture != NULL)
    {
        SDL_DestroyTexture(Texture);
        Texture = NULL;
    }

    if (Renderer != NULL)
    {
        SDL_DestroyRenderer(Renderer);
        Renderer = NULL;
    }

    if (Window != NULL)
    {
        SDL_DestroyWindow(Window);
        Window = NULL;
    }

    SDL_Quit();
}
