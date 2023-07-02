#include "../lib/io.h"
#include "../lib/cpu.h"
#include "../lib/cartridge.h"

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdint.h>

Color SYSTEM_PALETTE[sizeof(Color) * 64];
uint8_t frame[FRAME_WIDTH * FRAME_HEIGHT * 3];

// Returns true if program should stop
bool handle_input(CPU *cpu, SDL_Event *event)
{
    while(SDL_PollEvent(event))
    {
        switch (event->type)
        {
            case SDL_QUIT:
                return true;
            case SDL_KEYDOWN:
                switch (event->key.keysym.sym)
                {
                    case SDLK_UP:
                        mem_write(cpu, 0x77, INPUT_ADDR);
                        break;
                    case SDLK_DOWN:
                        mem_write(cpu, 0x73, INPUT_ADDR);
                        break;
                    case SDLK_LEFT:
                        mem_write(cpu, 0x61, INPUT_ADDR);
                        break;
                    case SDLK_RIGHT:
                        mem_write(cpu, 0x64, INPUT_ADDR);
                        break;
                    default:
                        break;
                }
            default:
                return false;
        }
    }
    return false;
}

// Colors pixel in frame array
void draw_pixel(uint8_t *frame, int x, int y, Color color)
{
    int pixel = y * 3 * FRAME_WIDTH + x * 3;
    // Checks if it's not out of bounds
    if (pixel + 2 < FRAME_WIDTH * FRAME_HEIGHT * 3)
    {
        frame[pixel] = color.r;
        frame[pixel + 1] = color.g;
        frame[pixel + 2] = color.b;
    }
}

// Renders tile on frame array
void render_tile(uint8_t *frame, uint8_t *chr_rom, int bank, int tile_n)
{
    bank *= 0x1000;
    // Just for testing
    int tile_x = 0;
    int tile_y = 0;

    for (int i = 0; i < 255; i++)
    {
        if (i != 0 && i % 20 == 0)
        {
            tile_y += 10;
            tile_x = 0;
        }
        int tile_lower_bound = bank + i * 16;
        int tile_upper_bound = tile_lower_bound + 15;
        for (int y = 0; y < 8; y ++)
        {

            // Each pixel is represented by 2 bits in different bytes, 8 addresses apart
            // Each tile is 16 bytes
            uint8_t upper = chr_rom[tile_lower_bound + y];
            uint8_t lower = chr_rom[tile_lower_bound + y + 8];

            for (int x = 7; x >= 0; x--)
            {
                uint8_t value = (upper & 1) << 1 | (lower & 1);
                // "Consume" the bits to go to the next one
                upper >>= 1;
                lower >>= 1;
                Color color;
                switch (value)
                {
                    case 0:
                        color = SYSTEM_PALETTE[0x01];
                        break;
                    case 1:
                        color = SYSTEM_PALETTE[0x23];
                        break;
                    case 2:
                        color = SYSTEM_PALETTE[0x27];
                        break;
                    case 3:
                        color = SYSTEM_PALETTE[0x30];
                        break;
                    default:
                        fprintf(stderr, "Impossible color wih value %i chosen at 'render_tile'.\n", value);
                        color = (Color) {0, 0, 0};
                }
                draw_pixel(frame, tile_x + x, tile_y + y, color);
            }
        }
        tile_x += 10;
    }
}

// Color functions

Color get_color(uint8_t byte)
{
    switch (byte)
    {
        // Black
        case 0: return new_color(0, 0, 0);
        // White
        case 1: return new_color(0xFF, 0xFF, 0xFF); 
        // Grey
        case 2:
        case 9:
            return new_color(0x80, 0x80, 0x80); 
        // Red
        case 3:
        case 10:
            return new_color(0xFF, 0, 0);
        // Green
        case 4:
        case 11:
            return new_color(0, 0xFF, 0);
        // Blue
        case 5:
        case 12:
            return new_color(0, 0, 0xFF);
        // Magenta
        case 6:
        case 13:
            return new_color(0xFF, 0, 0xFF);
        // Yellow
        case 7:
        case 14:
            return new_color(0xFF, 0xFF, 0);
        // Cyan
        default:
            return new_color(0, 0xFF, 0xFF);
    }
    return new_color(0, 0, 0);
}

Color new_color(uint8_t red, uint8_t green, uint8_t blue)
{
    Color color;
    color.r = red;
    color.g = green;
    color.b = blue;
    return color;
}

// Initializes the SYSTEM_PALETTE array
void palette_initialize()
{
    Color temp_array[] = {
        (Color) {0x80, 0x80, 0x80},
        (Color) {0x00, 0x3D, 0xA6},
        (Color) {0x00, 0x12, 0xB0},
        (Color) {0x44, 0x00, 0x96},
        (Color) {0xA1, 0x00, 0x5E},
        (Color) {0xC7, 0x00, 0x28},
        (Color) {0xBA, 0x06, 0x00},
        (Color) {0x8C, 0x17, 0x00},
        (Color) {0x5C, 0x2F, 0x00},
        (Color) {0x10, 0x45, 0x00},
        (Color) {0x05, 0x4A, 0x00},
        (Color) {0x00, 0x47, 0x2E},
        (Color) {0x00, 0x41, 0x66},
        (Color) {0x00, 0x00, 0x00},
        (Color) {0x05, 0x05, 0x05},
        (Color) {0x05, 0x05, 0x05},
        (Color) {0xC7, 0xC7, 0xC7},
        (Color) {0x00, 0x77, 0xFF},
        (Color) {0x21, 0x55, 0xFF},
        (Color) {0x82, 0x37, 0xFA},
        (Color) {0xEB, 0x2F, 0xB5},
        (Color) {0xFF, 0x29, 0x50},
        (Color) {0xFF, 0x22, 0x00},
        (Color) {0xD6, 0x32, 0x00},
        (Color) {0xC4, 0x62, 0x00},
        (Color) {0x35, 0x80, 0x00},
        (Color) {0x05, 0x8F, 0x00},
        (Color) {0x00, 0x8A, 0x55},
        (Color) {0x00, 0x99, 0xCC},
        (Color) {0x21, 0x21, 0x21},
        (Color) {0x09, 0x09, 0x09},
        (Color) {0x09, 0x09, 0x09},
        (Color) {0xFF, 0xFF, 0xFF},
        (Color) {0x0F, 0xD7, 0xFF},
        (Color) {0x69, 0xA2, 0xFF},
        (Color) {0xD4, 0x80, 0xFF},
        (Color) {0xFF, 0x45, 0xF3},
        (Color) {0xFF, 0x61, 0x8B},
        (Color) {0xFF, 0x88, 0x33},
        (Color) {0xFF, 0x9C, 0x12},
        (Color) {0xFA, 0xBC, 0x20},
        (Color) {0x9F, 0xE3, 0x0E},
        (Color) {0x2B, 0xF0, 0x35},
        (Color) {0x0C, 0xF0, 0xA4},
        (Color) {0x05, 0xFB, 0xFF},
        (Color) {0x5E, 0x5E, 0x5E},
        (Color) {0x0D, 0x0D, 0x0D},
        (Color) {0x0D, 0x0D, 0x0D},
        (Color) {0xFF, 0xFF, 0xFF},
        (Color) {0xA6, 0xFC, 0xFF},
        (Color) {0xB3, 0xEC, 0xFF},
        (Color) {0xDA, 0xAB, 0xEB},
        (Color) {0xFF, 0xA8, 0xF9},
        (Color) {0xFF, 0xAB, 0xB3},
        (Color) {0xFF, 0xD2, 0xB0},
        (Color) {0xFF, 0xEF, 0xA6},
        (Color) {0xFF, 0xF7, 0x9C},
        (Color) {0xD7, 0xE8, 0x95},
        (Color) {0xA6, 0xED, 0xAF},
        (Color) {0xA2, 0xF2, 0xDA},
        (Color) {0x99, 0xFF, 0xFC},
        (Color) {0xDD, 0xDD, 0xDD},
        (Color) {0x11, 0x11, 0x11},
        (Color) {0x11, 0x11, 0x11},
    };

    memcpy(SYSTEM_PALETTE, temp_array, sizeof(temp_array));
}
