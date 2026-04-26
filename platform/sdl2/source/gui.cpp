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

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"


static bool FlagQuit = false;

static bool ShowWindowControls = false;
static bool ShowWindowLicense = false;
static bool ShowWindowAbout = false;
static bool ShowVSyncSettingChange = false;
static bool ShowWindowUnknownOp = false;


void GUI_Init(SDL_Window* Window, SDL_Renderer* Renderer)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Disable imgui.ini saving
    ImGui::GetIO().IniFilename = NULL;

    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("fonts/Roboto-Medium.ttf", 20);

    ImGui_ImplSDL2_InitForSDLRenderer(Window, Renderer);
    ImGui_ImplSDLRenderer_Init(Renderer);
}

void GUI_ProcessEvents(SDL_Event* Events)
{
    ImGui_ImplSDL2_ProcessEvent(Events);
}

static void Draw_Menu()
{
    // Load previous settings
    config_t Config = Config_Get();

    int  SettingFrequency = Config.Frequency;

    ImVec4 SettingColorBackground = ImGui::ColorConvertU32ToFloat4(
        Config.ColorBackground.B << 16 |
        Config.ColorBackground.G << 8  | 
        Config.ColorBackground.R
    );

    ImVec4 SettingColorAccent = ImGui::ColorConvertU32ToFloat4(
        Config.ColorAccent.B << 16 |
        Config.ColorAccent.G << 8  |
        Config.ColorAccent.R
    );

    // Draw and process the menu
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Load ROM..."))
            {
                ROM_LoadFromDialog();
            }
            ImGui::MenuItem("Exit", NULL, &FlagQuit);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Emulation"))
        {
            if (ImGui::MenuItem("Reset", "F1"))
            {
                chip8ResetCpu();
            }
            if (ImGui::BeginMenu("CPU Frequency"))
            {
                if (ImGui::InputInt("Hz", &SettingFrequency, 10, 100, 0))
                {
                    if (SettingFrequency < 60)
                    {
                        Config.Frequency = 60;
                    }
                    else if (SettingFrequency > 65535)
                    {
                        Config.Frequency = 65535;
                    }
                    else
                    {
                        Config.Frequency = SettingFrequency;
                    }
                }
                if (ImGui::MenuItem("Default"))
                {
                    Config.Frequency = 840;
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Compatibility"))
            {
                if (ImGui::MenuItem("COSMAC VIP", NULL, Config.CompatibilityMode))
                {
                    Config.CompatibilityMode = true;
                }
                if (ImGui::MenuItem("Super-CHIP 1.1", NULL, !Config.CompatibilityMode))
                {
                    Config.CompatibilityMode = false;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::BeginMenu("Aspect Ratio"))
            {
                if (ImGui::MenuItem("Original", NULL, !Config.Stretch))
                {
                    Config.Stretch = false;
                }

                if (ImGui::MenuItem("Stretch", NULL, Config.Stretch))
                {
                    Config.Stretch = true;
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Color Palette"))
            {
                ImGui::ColorEdit3("Background", &SettingColorBackground.x);
                ImGui::ColorEdit3("Accent", &SettingColorAccent.x);

                if (ImGui::MenuItem("Default"))
                {
                    SettingColorBackground = ImGui::ColorConvertU32ToFloat4(0x000000);
                    SettingColorAccent = ImGui::ColorConvertU32ToFloat4(0xFFFFFF);
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Window Size"))
            {
                if (ImGui::MenuItem("6x"))
                {
                    Config.WindowWidth = 6 * 128;
                    Config.WindowHeight = 6 * 64;
                    Video_SetWindowSize(Config.EnableGUI, Config.WindowWidth, Config.WindowHeight);
                }

                if (ImGui::MenuItem("8x"))
                {
                    Config.WindowWidth = 8 * 128;
                    Config.WindowHeight = 8 * 64;
                    Video_SetWindowSize(Config.EnableGUI, Config.WindowWidth, Config.WindowHeight);
                }

                if (ImGui::MenuItem("10x"))
                {
                    Config.WindowWidth = 10 * 128;
                    Config.WindowHeight = 10 * 64;
                    Video_SetWindowSize(Config.EnableGUI, Config.WindowWidth, Config.WindowHeight);
                }

                if (ImGui::MenuItem("12x"))
                {
                    Config.WindowWidth = 12 * 128;
                    Config.WindowHeight = 12 * 64;
                    Video_SetWindowSize(Config.EnableGUI, Config.WindowWidth, Config.WindowHeight);
                }

                ImGui::EndMenu();
            }

            if (ImGui::MenuItem("Fullscreen", "F11", Config.EnableFullscreen))
            {
                if (Config.EnableFullscreen)
                {
                    Config.EnableFullscreen = false;
                    Video_EnableFullscreen(false);
                }
                else
                {
                    Config.EnableFullscreen = true;
                    Video_EnableFullscreen(true);
                }
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Settings"))
        {
            if (ImGui::MenuItem("Mute Audio", NULL, Config.Mute))
            {
                Config.Mute = !Config.Mute;
            }

            if (ImGui::MenuItem("Enable GUI", "F12", Config.EnableGUI))
            {
                Config.EnableGUI = !Config.EnableGUI;
            }

            if (ImGui::MenuItem("Enable V-Sync", NULL, Config.EnableVSync))
            {
                Config.EnableVSync = !Config.EnableVSync;
                ShowVSyncSettingChange = true;
            }
            
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help"))
        {
            ImGui::MenuItem("Controls...", NULL, &ShowWindowControls);
            ImGui::MenuItem("License...", NULL, &ShowWindowLicense);
            ImGui::MenuItem("About...", NULL, &ShowWindowAbout);
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    // Save new settings
    uint32_t ColorBackgroundBGR = ImGui::ColorConvertFloat4ToU32(SettingColorBackground);
    uint32_t ColorAccentBGR     = ImGui::ColorConvertFloat4ToU32(SettingColorAccent);

    Config.ColorBackground.R = ColorBackgroundBGR & 0xFF;
    Config.ColorBackground.G = (ColorBackgroundBGR & 0xFF00) >> 8;
    Config.ColorBackground.B = ColorBackgroundBGR >> 16;
    Config.ColorAccent.R     = ColorAccentBGR & 0xFF;
    Config.ColorAccent.G     = (ColorAccentBGR & 0xFF00) >> 8;
    Config.ColorAccent.B     = ColorAccentBGR >> 16;

    Config_Set(Config);
}

static void Draw_Windows()
{
    if (ShowWindowControls)
    {
        ImGui::SetNextWindowSize(ImVec2(500, 250), ImGuiCond_Appearing);
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::Begin("Controls", &ShowWindowControls);
        ImGui::TextWrapped("CHIP-8 uses a 16-key hexadecimal keypad with the following layout:\n\n"
            "1 2 3 C\n4 5 6 D\n7 8 9 E\nA 0 B F\n\n"
            "This layout is mapped to your keyboard this way:\n\n"
            "1 2 3 4\nQ W E R\nA S D F\nZ X C V");
        ImGui::End();
    }

    if (ShowWindowLicense)
    {
        ImGui::SetNextWindowSize(ImVec2(500, 250), ImGuiCond_Appearing);
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::Begin("License", &ShowWindowLicense);
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

    if (ShowWindowAbout)
    {
        ImGui::SetNextWindowSize(ImVec2(500, 250), ImGuiCond_Appearing);
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::Begin("About", &ShowWindowAbout);
        ImGui::TextWrapped("chip8swemu v1.2.0\n"
            "by AlfonsoJLuna\n\n"
            "Multiplatform CHIP-8 and Super-CHIP emulator.\n\n"
            "More details:\n"
            "https://github.com/AlfonsoJLuna/chip8swemu\n\n"
            "Libraries used:\n"
            "SDL2 - https://libsdl.org\n"
            "Dear ImGui - https://github.com/ocornut/imgui\n");
        ImGui::End();
    }

    if (ShowVSyncSettingChange)
    {
        ImGui::SetNextWindowSize(ImVec2(500, 250), ImGuiCond_Appearing);
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::Begin("Message", &ShowVSyncSettingChange);
        ImGui::TextWrapped("Please restart the application to apply VSync settings.");
        ImGui::End();
    }

    if (ShowWindowUnknownOp)
    {
        ImGui::SetNextWindowSize(ImVec2(500, 250), ImGuiCond_Appearing);
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::Begin("Unknown Opcode", &ShowWindowUnknownOp);
        ImGui::TextWrapped("An unsupported opcode has been detected.\n\n"
            "Emulation will continue but may not work as expected.\n\n"
            "Please load a ROM with CHIP-8/Super-CHIP instructions only.");
        ImGui::End();
    }
}

void GUI_Render()
{
    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    Draw_Menu();
    Draw_Windows();
    
    ImGui::Render();
    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
}

bool GUI_GetFlagQuit()
{
    return FlagQuit;
}

void GUI_ShowUnknownOp()
{
    ShowWindowUnknownOp = true;
}

void GUI_Quit()
{
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}
