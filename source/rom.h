#ifndef ROM_H
#define ROM_H


#include <stdbool.h>


bool romLoadDefault();

void romSetPath(char* path);

bool romObtainPath();

bool romLoadFromPath();


#endif  // ROM_H
