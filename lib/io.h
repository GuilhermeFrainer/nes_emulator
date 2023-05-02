#ifndef GAME_IO_H
#define GAME_IO_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define GAME_WIDTH 32
#define GAME_HEIGHT 32
#define SCALE 10
#define WIN_WIDTH GAME_WIDTH * SCALE
#define WIN_HEIGHT GAME_HEIGHT * SCALE

#define SCREEN_MEM 0x0200
#define SCREEN_SIZE 0x0400

#define RAND_NUM_ADDR 0xFE
#define INPUT_ADDR 0xFF

typedef struct CPU CPU;

bool handle_input(CPU *cpu, SDL_Event *event);

#endif