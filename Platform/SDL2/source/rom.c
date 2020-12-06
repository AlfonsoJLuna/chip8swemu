#include "rom.h"

#include "chip8.h"
#include "logo.h"
#include <stdint.h>
#include <stdio.h>

#ifdef _WIN32

#include <shlobj.h>
#include <commdlg.h>

#endif


static int loadRom(uint8_t* rom, size_t rom_size)
{
	chip8ResetCpu();
	
	return chip8ResetMem(rom, rom_size);
}


int romLoadDefault()
{
	return loadRom(logo, sizeof(logo));
}


int romLoadFromPath(char* rom_path)
{
	FILE* rom = fopen(rom_path, "rb");

	if (rom == NULL)
	{
		return 1;
	}

	fseek(rom, 0, SEEK_END);
	
	size_t rom_size = ftell(rom);
	
	rewind(rom);

	if (rom_size > 3584)
	{
		fclose(rom);
		return 1;
	}

	uint8_t buffer[rom_size];
	
	size_t copy_size = fread(buffer, sizeof(uint8_t), rom_size, rom);
	
	fclose(rom);
	
	if (copy_size != rom_size)
	{
		return 1;
	}

	return loadRom(buffer, rom_size);
}


int romLoadFromDialog()
{
	#ifdef _WIN32

	char rom_path[260];

	OPENFILENAME ofn;
	
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = rom_path;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(rom_path);
	ofn.lpstrFilter = "CHIP-8 Roms\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = "roms";
	ofn.lpstrTitle = "Select ROM";
	ofn.Flags = OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn))
	{
		return romLoadFromPath(rom_path);
	}

	#endif

	return 1;
}
