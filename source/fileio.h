#ifndef FILEIO_H
#define FILEIO_H


#include "config.h"
#include <stdbool.h>
#include <stdint.h>


bool getRomPath(char* rom_path);

bool loadRomFromPath(char* rom_path, uint8_t* memory);

bool loadConfig(configuration* config);

bool saveConfig(configuration* config);


#endif  // FILEIO_H

