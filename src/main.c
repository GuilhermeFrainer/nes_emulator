#include "../lib/cpu.h"
#include "../lib/instructions.h"
#include "../lib/io.h"
#include "../lib/cartridge.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <stdint.h>

int main(int argc, char **argv)
{
    if (argc > 2)
    {
        fprintf(stderr, "Too many arguments provided. Expected 1, received %i.\n", argc - 1);
        return 1;
    }
    else if (argc < 2)
    {
        fprintf(stderr, "Too few arguments provided. Expected 1, received %i.\n", argc - 1);
        return 1;
    }

    ROM *rom = get_rom(argv[1]);
    if (rom == NULL)
    {
        return 1;
    }
    
    // SDL Inits
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "SDL_Init() failed. Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window  = SDL_CreateWindow(
        "NES emulator",
        100,
        100,
        FRAME_WIDTH * SCALE,
        FRAME_HEIGHT * SCALE,
        SDL_WINDOW_SHOWN
    );
    if (window == NULL)
    {
        fprintf(stderr, "Error in creating window: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        fprintf(stderr, "Error in creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    
    if (SDL_RenderSetLogicalSize(renderer, FRAME_WIDTH, FRAME_HEIGHT) != 0)
    {
        fprintf(stderr, "Error in setting logical size: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    if (SDL_RenderSetScale(renderer, SCALE, SCALE) != 0)
    {
        fprintf(stderr, "Error in setting scale: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    

    SDL_Texture *texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGB24,
        SDL_TEXTUREACCESS_TARGET,
        FRAME_WIDTH,
        FRAME_HEIGHT
    );

    if (texture == NULL)
    {
        fprintf(stderr, "Error in creating texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    palette_initialize();
    render_tile(frame, rom->chr_rom, 1, 1);
    /*
    CPU *cpu = new_cpu(rom);
    populate_inst_list();
    load(cpu);
    reset(cpu);
    run(cpu, renderer, texture);
    */
    SDL_RenderClear(renderer);
    SDL_UpdateTexture(texture, NULL, frame, FRAME_WIDTH * 3);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    bool running = true;
    SDL_Event event;
    
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            running = false;
                            break;
                    }
            }
        }
    }

    cleanup:
    // Cleanup
    //destroy_cpu(cpu);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}
