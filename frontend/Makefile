C_FILES := $(wildcard ../core/*.c) $(wildcard source/*.c)

CPP_FILES := $(wildcard source/*.cpp) \
	libraries/imgui-1.51/imgui.cpp \
	libraries/imgui-1.51/imgui_draw.cpp \

C_OBJECTS := $(addprefix objects/,$(notdir $(C_FILES:.c=.o)))

CPP_OBJECTS := $(addprefix objects/,$(notdir $(CPP_FILES:.cpp=.o)))

HEADERS := -I../core -Isource -Ilibraries/imgui-1.51 \
	-Ilibraries/SDL2-2.0.5/x86_64-w64-mingw32/include/SDL2

CFLAGS := -Wall -Wextra -MMD -O3 -m64

LFLAGS := -Llibraries/SDL2-2.0.5/x86_64-w64-mingw32/lib \
	-Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 \
	-lOpenGL32 -limm32 -lcomdlg32 -m64

build/chip8swemu.exe: $(C_OBJECTS) $(CPP_OBJECTS)
		@test -d build || mkdir build
		g++ $^ $(LFLAGS) -o $@
		xcopy /y libraries\SDL2-2.0.5\x86_64-w64-mingw32\bin\SDL2.dll build

objects/%.o: ../core/%.c
	@test -d objects || mkdir objects
	gcc -std=c99 $(CFLAGS) -c $< -o $@

objects/%.o: source/%.c
	@test -d objects || mkdir objects
	gcc -std=c99 $(CFLAGS) $(HEADERS) -c $< -o $@

objects/%.o: source/%.cpp
	@test -d objects || mkdir objects
	g++ $(CFLAGS) $(HEADERS) -c $< -o $@

objects/%.o: libraries/imgui-1.51/%.cpp
	@test -d objects || mkdir objects
	g++ $(CFLAGS) -c $< -o $@

-include $(C_OBJECTS:.o=.d)

-include $(CPP_OBJECTS:.o=.d)
