#include "gui.h"

#ifdef __cplusplus
    extern "C" {
#endif

#include "audio.h"
#include "chip8.h"
#include "config.h"
#include "rom.h"
#include "video.h"

#ifdef __cplusplus
    }
#endif

#include <imgui.h>
#include "imgui_impl_sdl.h"


static bool flag_load_rom;
static bool flag_quit;
static bool flag_reset;
static bool flag_default_freq;
static bool flag_compatibility;
static bool flag_vertical_wrap;
static bool flag_window_5x;
static bool flag_window_10x;
static bool flag_window_15x;
static bool flag_mute_sound;
static bool flag_enable_vsync;

static bool show_window_controls;
static bool show_window_license;
static bool show_window_about;
static bool show_window_unknownop;


void guiInitialize(SDL_Window* window)
{
    ImGui_ImplSdl_Init(window);

    // Disable imgui.ini saving
    ImGui::GetIO().IniFilename = NULL;

    flag_load_rom = false;
    flag_quit = false;
    flag_reset = false;
    flag_default_freq = false;
    flag_window_5x = false;
    flag_window_10x = false;
    flag_window_15x = false;
    flag_mute_sound = false;
    flag_enable_vsync = false;

    show_window_controls = false;
    show_window_license = false;
    show_window_about = false;
    show_window_unknownop = false;
}


void guiProcessInput(SDL_Event* events)
{
    ImGui_ImplSdl_ProcessEvent(events);
}


void guiProcessElements(SDL_Window* window)
{
    config_t config = configGet();

    int chip8_cpu_freq = config.frequency;

    flag_compatibility = config.comp_mode;
    flag_vertical_wrap = config.vert_wrap;

    ImVec4 vector_backgr = ImGui::ColorConvertU32ToFloat4(
        config.background.blue << 16 | config.background.green << 8 | config.background.red);
    ImVec4 vector_accent = ImGui::ColorConvertU32ToFloat4(
        config.accent.blue << 16 | config.accent.green << 8 | config.accent.red);

    ImGui_ImplSdl_NewFrame(window);

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("Load ROM...", NULL, &flag_load_rom);
            ImGui::MenuItem("Exit", NULL, &flag_quit);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Emulation"))
        {
            ImGui::MenuItem("Reset", NULL, &flag_reset);
            if (ImGui::BeginMenu("CPU Frequency"))
            {
                ImGui::SliderInt("Hz", &chip8_cpu_freq, 60, 5000, "%.0f");
                ImGui::MenuItem("Default (840Hz)", NULL, &flag_default_freq);
                ImGui::EndMenu();
            }
            ImGui::MenuItem("Compatibility Mode", NULL, &flag_compatibility);
            ImGui::MenuItem("Vertical Wrap", NULL, &flag_vertical_wrap);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Settings"))
        {
            if (ImGui::BeginMenu("Window Size"))
            {
                ImGui::MenuItem("5x", NULL, &flag_window_5x);
                ImGui::MenuItem("10x", NULL, &flag_window_10x);
                ImGui::MenuItem("15x", NULL, &flag_window_15x);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Color Palette"))
            {
                ImGui::ColorEdit3("Background", &vector_backgr.x);
                ImGui::ColorEdit3("Accent", &vector_accent.x);
                ImGui::EndMenu();
            }
            ImGui::MenuItem("Mute Sound", NULL, &flag_mute_sound);
            ImGui::MenuItem("V-Sync", NULL, &flag_enable_vsync);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help"))
        {
            ImGui::MenuItem("Controls...", NULL, &show_window_controls);
            ImGui::MenuItem("License...", NULL, &show_window_license);
            ImGui::MenuItem("About...", NULL, &show_window_about);
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (flag_load_rom)
    {
        romLoadFromDialog();
        flag_load_rom = false;
    }

    if (flag_reset)
    {
        chip8ResetCpu();
        flag_reset = false;
    }

    config.frequency = chip8_cpu_freq;

    if (flag_default_freq)
    {
        config.frequency = 840;
        flag_default_freq = false;
    }

    config.comp_mode = flag_compatibility;
    chip8CompatibilityMode(flag_compatibility);

    config.vert_wrap = flag_vertical_wrap;
    chip8VerticalWrap(flag_vertical_wrap);

    if (flag_window_5x)
    {
        SDL_SetWindowSize(window, 5 * 128, 5 * 64 + 19);
        flag_window_5x = false;
    }

    if (flag_window_10x)
    {
        SDL_SetWindowSize(window, 10 * 128, 10 * 64 + 19);
        flag_window_10x = false;
    }

    if (flag_window_15x)
    {
        SDL_SetWindowSize(window, 15 * 128, 15 * 64 + 19);
        flag_window_15x = false;
    }

    uint32_t bgr_backgr = ImGui::ColorConvertFloat4ToU32(vector_backgr);
    uint32_t bgr_accent = ImGui::ColorConvertFloat4ToU32(vector_accent);

    config.background.red = bgr_backgr & 0xFF;
    config.background.green = (bgr_backgr & 0xFF00) >> 8;
    config.background.blue = bgr_backgr >> 16;
    config.accent.red = bgr_accent & 0xFF;
    config.accent.green = (bgr_accent & 0xFF00) >> 8;
    config.accent.blue = bgr_accent >> 16;

    configSet(config);

    audioMute(flag_mute_sound);

    videoToggleVsync(flag_enable_vsync);

    if (show_window_controls)
    {
        ImGui::SetNextWindowSize(ImVec2(450, 230), ImGuiSetCond_Appearing);
        ImGui::SetNextWindowPosCenter(ImGuiSetCond_Appearing);
        ImGui::Begin("Controls", &show_window_controls);
        ImGui::TextWrapped("CHIP-8 uses a 16-key hexadecimal keypad with the following layout:\n\n"
            "1 2 3 C\n4 5 6 D\n7 8 9 E\nA 0 B F\n\n"
            "This layout is mapped to your keyboard this way:\n\n"
            "1 2 3 4\nQ W E R\nA S D F\nZ X C V");
        ImGui::End();
    }

    if (show_window_license)
    {
        ImGui::SetNextWindowSize(ImVec2(450, 230), ImGuiSetCond_Appearing);
        ImGui::SetNextWindowPosCenter(ImGuiSetCond_Appearing);
        ImGui::Begin("License", &show_window_license);
        ImGui::TextWrapped("The MIT License (MIT)\n"
            "Copyright (c) 2016 AlfonsoJLuna\n\n"
            "Permission is hereby granted, free of charge, to any person "
            "obtaining a copy of this software and associated documentation "
            "files (the \"Software\"), to deal in the Software without "
            "restriction, including without limitation the rights to use, "
            "copy, modify, merge, publish, distribute, sublicense, and/or "
            "sell copies of the Software, and to permit persons to whom "
            "the Software is furnished to do so, subject to the following "
            "conditions:\n\n"
            "The above copyright notice and this permission notice shall be "
            "included in all copies or substantial portions of the Software.\n\n"
            "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY "
            "KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE "
            "WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE "
            "AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT "
            "HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, "
            "WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING "
            "FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR "
            "OTHER DEALINGS IN THE SOFTWARE.");
        ImGui::End();
    }

    if (show_window_about)
    {
        ImGui::SetNextWindowSize(ImVec2(450, 230), ImGuiSetCond_Appearing);
        ImGui::SetNextWindowPosCenter(ImGuiSetCond_Appearing);
        ImGui::Begin("About", &show_window_about);
        ImGui::TextWrapped("chip8swemu v1.1.3\n"
            "by AlfonsoJLuna\n\n"
            "Multiplatform CHIP-8 and Super-CHIP emulator.\n\n"
            "More details: https://github.com/AlfonsoJLuna/chip8swemu\n\n"
            "Libraries used:\n"
            "SDL2 - http://libsdl.org\n"
            "imgui - https://github.com/ocornut/imgui\n");
        ImGui::End();
    }

    if (show_window_unknownop)
    {
        ImGui::SetNextWindowSize(ImVec2(450, 115), ImGuiSetCond_Appearing);
        ImGui::SetNextWindowPosCenter(ImGuiSetCond_Appearing);
        ImGui::Begin("Unknown opcode", &show_window_unknownop);
        ImGui::TextWrapped("An unsupported opcode has been processed.\n\n"
            "Emulation will continue but may not work as expected.\n\n"
            "Please load a rom with CHIP-8/Super-CHIP instructions only.");
        ImGui::End();
    }
}


bool guiGetFlagQuit()
{
    return flag_quit;
}


void guiShowUnknownOp()
{
    show_window_unknownop = true;
}


void guiRender()
{
    ImGui::Render();
}


void guiFinalize()
{
    ImGui_ImplSdl_Shutdown();
}
