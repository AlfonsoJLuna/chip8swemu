# Source files

C_FILES += $(wildcard ../../Core/*.c) $(wildcard source/*.c)

CPP_FILES += $(wildcard source/*.cpp) \
	libraries/imgui-1.52/imgui.cpp \
	libraries/imgui-1.52/imgui_draw.cpp

C_OBJECTS += $(addprefix objects/,$(notdir $(C_FILES:.c=.o)))

CPP_OBJECTS += $(addprefix objects/,$(notdir $(CPP_FILES:.cpp=.o)))

# Common flags

HEADERS += -I../../Core -I../../Assets/ROMs -Isource -Ilibraries/imgui-1.52

CFLAGS += -Wall -Wextra -MMD -O2

# Platform-specific flags

ifeq ($(OS),Windows_NT)

	CFLAGS += -D__USE_MINGW_ANSI_STDIO
	LFLAGS += -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 \
		-lOpenGL32 -limm32 -lcomdlg32

	ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)

		HEADERS += -Ilibraries/SDL2-2.0.12/x86_64-w64-mingw32/include/SDL2
		CFLAGS += -m64
		LFLAGS += -m64 -Llibraries/SDL2-2.0.12/x86_64-w64-mingw32/lib

	else

		ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)

			HEADERS += -Ilibraries/SDL2-2.0.12/x86_64-w64-mingw32/include/SDL2
			CFLAGS += -m64
			LFLAGS += -m64 -Llibraries/SDL2-2.0.12/x86_64-w64-mingw32/lib

		endif

		ifeq ($(PROCESSOR_ARCHITECTURE),x86)

			HEADERS += -Ilibraries/SDL2-2.0.12/i686-w64-mingw32/include/SDL2
			CFLAGS += -m32
			LFLAGS += -m32 -Llibraries/SDL2-2.0.12/i686-w64-mingw32/lib

		endif

	endif

else

	UNAME_S := $(shell uname -s)

	ifeq ($(UNAME_S),Linux)

		HEADERS += $(shell sdl2-config --cflags)
		LFLAGS += $(shell sdl2-config --libs) -lGL

	endif

	ifeq ($(UNAME_S),Darwin)

		CFLAGS += -D OSX

	endif

	UNAME_P := $(shell uname -p)

	ifeq ($(UNAME_P),x86_64)

		CFLAGS += -D AMD64

	endif

	ifneq ($(filter %86,$(UNAME_P)),)

		CFLAGS += -D IA32

	endif

	ifneq ($(filter arm%,$(UNAME_P)),)

		CFLAGS += -D ARM

	endif

endif

# Rules

build/chip8swemu: $(C_OBJECTS) $(CPP_OBJECTS)
	g++ $^ $(LFLAGS) -o $@

objects/%.o: ../../Core/%.c
	gcc -std=c99 $(CFLAGS) -c $< -o $@

objects/%.o: source/%.c
	gcc -std=c99 $(CFLAGS) $(HEADERS) -c $< -o $@

objects/%.o: source/%.cpp
	g++ $(CFLAGS) $(HEADERS) -c $< -o $@

objects/%.o: libraries/imgui-1.52/%.cpp
	g++ $(CFLAGS) -c $< -o $@

-include $(C_OBJECTS:.o=.d)

-include $(CPP_OBJECTS:.o=.d)
