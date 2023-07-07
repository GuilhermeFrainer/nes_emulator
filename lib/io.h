#ifndef GAME_IO_H
#define GAME_IO_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define FRAME_WIDTH 256
#define FRAME_HEIGHT 240

#define SCALE 2

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

extern Color SYSTEM_PALETTE[sizeof(Color) * 64];
extern uint8_t frame[FRAME_WIDTH * FRAME_HEIGHT * 3];

bool handle_input(CPU *cpu, SDL_Event *event);

// Screen functions
void draw_pixel(uint8_t *frame, int x, int y, Color color);
void render_tile(uint8_t *frame, uint8_t *chr_rom, int bank, int tile_n);

// Color functions
Color get_color(uint8_t byte);
Color new_color(uint8_t red, uint8_t green, uint8_t blue);

void palette_initialize(void);

#endif
