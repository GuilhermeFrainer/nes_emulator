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
                        write_mem(cpu, 0x77, INPUT_ADDR);
                        break;
                    case SDLK_DOWN:
                        write_mem(cpu, 0x73, INPUT_ADDR);
                        break;
                    case SDLK_LEFT:
                        write_mem(cpu, 0x61, INPUT_ADDR);
                        break;
                    case SDLK_RIGHT:
                        write_mem(cpu, 0x64, INPUT_ADDR);
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