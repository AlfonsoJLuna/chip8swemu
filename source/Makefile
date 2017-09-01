C_FILES := $(wildcard *.c)
CPP_FILES := $(wildcard *.cpp) \
	../libraries/imgui-1.51/imgui.cpp \
	../libraries/imgui-1.51/imgui_draw.cpp \
	../libraries/imgui-1.51/examples/sdl_opengl2_example/imgui_impl_sdl.cpp

C_OBJECTS :=  $(addprefix ../objects/,$(notdir $(C_FILES:.c=.o)))
CPP_OBJECTS :=  $(addprefix ../objects/,$(notdir $(CPP_FILES:.cpp=.o)))

LSDL2 := -L../libraries/SDL2-2.0.5/i686-w64-mingw32/lib
ISDL2 := -I../libraries/SDL2-2.0.5/i686-w64-mingw32/include/SDL2
IIMGUI := -I../libraries/imgui-1.51 -I../libraries/imgui-1.51/examples/sdl_opengl2_example

LFLAGS := -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -lOpenGL32 -limm32 -lcomdlg32
CFLAGS := -Wall -Wextra -MMD -O3

../build/chip8swemu.exe: $(C_OBJECTS) $(CPP_OBJECTS)
	g++ $^ $(LSDL2) $(LFLAGS) -o $@

../objects/%.o: %.c
	gcc -std=c99 $(CFLAGS) $(ISDL2) -c $< -o $@

../objects/%.o: %.cpp
	g++ $(CFLAGS) $(ISDL2) $(IIMGUI) -c $< -o $@

../objects/%.o: ../libraries/imgui-1.51/%.cpp
	g++ $(CFLAGS) -c $< -o $@

../objects/%.o: ../libraries/imgui-1.51/examples/sdl_opengl2_example/%.cpp
	g++ $(CFLAGS) $(ISDL2) $(IIMGUI) -c $< -o $@

-include $(C_OBJECTS:.o=.d)

-include $(CPP_OBJECTS:.o=.d)

print:
	@echo $(C_FILES)
	@echo $(CPP_FILES)
	@echo $(C_OBJECTS)
	@echo $(CPP_OBJECTS)
