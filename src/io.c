#include "../lib/io.h"
#include "../lib/cpu.h"

#include <stdbool.h>
#include <SDL2/SDL.h>

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

bool read_screen_state(CPU *cpu, uint8_t buffer[GAME_HEIGHT * GAME_WIDTH * 3])
{
    bool update = false;
    for (int i = 0; i < GAME_HEIGHT * GAME_WIDTH; i++)
    {
        uint8_t read_memory = mem_read(cpu, SCREEN_MEM + i);
        Color color = get_color(read_memory);
        if (buffer[3 * i] != color.r || buffer[3 * i + 1] != color.g || buffer[3 * i + 2] != color.b)
        {
            buffer[3 * i] = color.r;
            buffer[3 * i + 1] = color.g;
            buffer[3 * i + 2] = color.b;
            update = true;
        }
    }
    return update;
}