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

typedef struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Color;

extern uint8_t buffer[GAME_WIDTH * GAME_HEIGHT * 3];
extern Color SYSTEM_PALETTE[sizeof(Color) * 64];

bool handle_input(CPU *cpu, SDL_Event *event);
Color get_color(uint8_t byte);
Color new_color(uint8_t red, uint8_t green, uint8_t blue);
bool read_screen_state(CPU *cpu, uint8_t buffer[GAME_HEIGHT * GAME_WIDTH * 3]);

void palette_initialize(void);

#endif
