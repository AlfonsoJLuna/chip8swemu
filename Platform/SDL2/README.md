## Building

Required dependencies are ImGui, SDL2, OpenGL.

### How to build on Windows

1. [Install](https://git-scm.com/downloads) git and clone this repository.
2. Download and install the mingw-w64 toolchain. Prebuilt binaries are available [here](https://gnutoolchains.com/mingw64/). Be sure you check `Add to PATH` during installation.
3. [Download](https://www.libsdl.org/release/SDL2-devel-2.0.12-mingw.tar.gz) SDL 2.0.12 libraries for MinGW and extract them to `chip8swemu/frontend/libraries/SDL2-2.0.12/...`
4. [Download](https://github.com/ocornut/imgui/archive/v1.52.zip) ImGui 1.52 source code and extract it to `chip8swemu/frontend/libraries/imgui-1.52/...`
5. Open a command prompt in `chip8swemu/frontend/` and type: `mingw32-make`.

### How to build on Linux (Ubuntu)

1. Install git: `sudo apt-get install git`
2. Clone this repository: `git clone https://github.com/AlfonsoJLuna/chip8swemu`
3. Install SDL2 development libraries: `sudo apt-get install libsdl2-dev`
4. [Download](https://github.com/ocornut/imgui/archive/v1.52.zip) ImGui 1.52 source code and extract it to `chip8swemu/frontend/libraries/imgui-1.52/...`
5. Open a terminal in `chip8swemu/frontend/` and type: `make`. 
