#ifndef GAME_IO_H
#define GAME_IO_H

#include <stdint.h>

#define GAME_WIDTH 32
#define GAME_HEIGHT 32
#define SCALE 10
#define WIN_WIDTH GAME_WIDTH * SCALE
#define WIN_HEIGHT GAME_HEIGHT * SCALE

#define SCREEN_MEM 0x0200
#define SCREEN_SIZE 0x0400

//extern uint8_t buffer[GAME_WIDTH * GAME_HEIGHT * 3];

#endif