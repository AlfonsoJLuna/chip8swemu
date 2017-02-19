#include "gui.h"
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <stdbool.h>


bool show_window_controls;
bool show_window_license;
bool show_window_about;


void GUI_Initialize(SDL_Window* window)
{
    ImGui_ImplSdl_Init(window);
    
    // Disable imgui.ini saving
    ImGui::GetIO().IniFilename = NULL;

    show_window_controls = false;
    show_window_license = false;
    show_window_about = false;
}


void GUI_GetInput(SDL_Event* event)
{
    ImGui_ImplSdl_ProcessEvent(event);
}


void GUI_ProcessElements(SDL_Window* window, configuration* config)
{
    // Convert BGR to ImVec4
    ImVec4 background_vector = ImGui::ColorConvertU32ToFloat4(config->background_color_bgr);
    ImVec4 accent_vector = ImGui::ColorConvertU32ToFloat4(config->accent_color_bgr);

    ImGui_ImplSdl_NewFrame(window);

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("Load ROM...", NULL, &config->load_rom);
            ImGui::MenuItem("Exit", NULL, &config->quit);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Emulation"))
        {
            ImGui::MenuItem("Reset", NULL, &config->reset);
            if (ImGui::BeginMenu("Speed"))
            {
                ImGui::SliderInt("Hz", &config->instructions_per_second, config->ips_min, config->ips_max, "%.0f");
                ImGui::MenuItem("Default (840Hz)", NULL, &config->ips_default);
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Settings"))
        {
            if (ImGui::BeginMenu("Window Size"))
            {
                ImGui::MenuItem("5x", NULL, &config->window_5x);
                ImGui::MenuItem("10x", NULL, &config->window_10x);
                ImGui::MenuItem("15x", NULL, &config->window_15x);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Color Palette"))
            {
                ImGui::ColorEdit3("Background", &background_vector.x);
                ImGui::ColorEdit3("Accent", &accent_vector.x);
                ImGui::EndMenu();
            }
            ImGui::MenuItem("Mute Sound", NULL, &config->mute_sound);
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
            "Copyright (c) 2017 AlfonsoJLuna\n\n"
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
        ImGui::TextWrapped("chip8swemu v0.1.0\n"
            "by AlfonsoJLuna\n\n"
            "Multiplatform CHIP-8 and Super-CHIP emulator.\n\n"
            "More details: https://github.com/AlfonsoJLuna/chip8swemu\n\n"
            "Libraries used:\n"
            "SDL 2.0.5 - http://libsdl.org\n"
            "imgui 1.49 - https://github.com/ocornut/imgui\n");
        ImGui::End();
    }

    if (config->ips_default)
    {
        config->instructions_per_second = 840;
        config->ips_default = false;
    }

    // Resize window if multiplier was changed
    if (config->window_5x)
    {
            config->window_size_multiplier = 5;
            SDL_SetWindowSize(window, 5 * 128, 5 * 64 + 19);
            config->window_5x = false;
    }
    if (config->window_10x)
    {
            config->window_size_multiplier = 10;
            SDL_SetWindowSize(window, 10 * 128, 10 * 64 + 19);
            config->window_10x = false;
    }
    if (config->window_15x)
    {
            config->window_size_multiplier = 15;
            SDL_SetWindowSize(window, 15 * 128, 15 * 64 + 19);
            config->window_15x = false;
    }
    
    // Convert ImVec4 to BGR
    config->background_color_bgr = ImGui::ColorConvertFloat4ToU32(background_vector);
    config->accent_color_bgr = ImGui::ColorConvertFloat4ToU32(accent_vector);
}


void GUI_Render()
{
    ImGui::Render();
}


void GUI_Finalize()
{
    ImGui_ImplSdl_Shutdown();
}

