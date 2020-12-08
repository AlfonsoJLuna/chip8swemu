#ifndef CHIP8SWEMU_H
#define CHIP8SWEMU_H


#include <stm32h7xx_hal.h>


void chip8swemu_main_loop(SAI_HandleTypeDef* hsai_BlockA1);


#endif
