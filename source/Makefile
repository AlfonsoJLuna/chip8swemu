C_FILES := $(wildcard core/*.c) $(wildcard frontend/*.c)

CPP_FILES := $(wildcard frontend/gui/*.cpp) \
	../libraries/imgui-1.51/imgui.cpp \
	../libraries/imgui-1.51/imgui_draw.cpp \

C_OBJECTS :=  $(addprefix ../objects/,$(notdir $(C_FILES:.c=.o)))
CPP_OBJECTS :=  $(addprefix ../objects/,$(notdir $(CPP_FILES:.cpp=.o)))

INCLUDES := -Icore -Ifrontend -Ifrontend/gui

LSDL2 := -L../libraries/SDL2-2.0.5/i686-w64-mingw32/lib
ISDL2 := -I../libraries/SDL2-2.0.5/i686-w64-mingw32/include/SDL2
IIMGUI := -I../libraries/imgui-1.51

LFLAGS := -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 \
	-lOpenGL32 -limm32 -lcomdlg32

CFLAGS := -Wall -Wextra -MMD -O3

../build/chip8swemu.exe: $(C_OBJECTS) $(CPP_OBJECTS)
	g++ $^ $(LSDL2) $(LFLAGS) -o $@

../objects/%.o: core/%.c
	gcc -std=c99 $(CFLAGS) -c $< -o $@

../objects/%.o: frontend/%.c
	gcc -std=c99 $(CFLAGS) $(INCLUDES) $(ISDL2) -c $< -o $@

../objects/%.o: frontend/gui/%.cpp
	g++ $(CFLAGS) $(INCLUDES) $(ISDL2) $(IIMGUI) -c $< -o $@

../objects/%.o: ../libraries/imgui-1.51/%.cpp
	g++ $(CFLAGS) -c $< -o $@

-include $(C_OBJECTS:.o=.d)

-include $(CPP_OBJECTS:.o=.d)
