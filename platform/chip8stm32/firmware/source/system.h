#ifndef SYSTEM_H
#define SYSTEM_H


#include <stdint.h>


int systemInit();

void systemDelay(uint32_t time_ms);

uint32_t systemMillis();


#endif
