COMPILER = g++

FILES = \
	source\chip8swemu.c \
	source\audio.c \
	source\input.c \
	source\fileio.c \
	source\video.c \
	source\cpu.c \
	source\config.c \
	source\gui.cpp \
	source\imgui_impl_sdl.cpp \
	libraries\imgui\imgui.cpp \
	libraries\imgui\imgui_draw.cpp

IMGUI_HEADERS = -Ilibraries\imgui
SDL2_HEADERS = -Ilibraries\SDL2-2.0.4\i686-w64-mingw32\include\SDL2
SDL2_LIB = -Llibraries\SDL2-2.0.4\i686-w64-mingw32\lib

COMPILER_FLAGS = -Wall -Wl,-subsystem,windows
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lOpenGL32 -limm32 -lcomdlg32

EXE = chip8swemu

all:
	$(COMPILER) $(FILES) $(IMGUI_HEADERS) $(SDL2_HEADERS) $(SDL2_LIB) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(EXE)

