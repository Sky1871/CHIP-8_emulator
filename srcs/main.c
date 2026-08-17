#include "chip8.h"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define WINDOW_SCALE 10

int main(int argc, char **argv) {
    t_chip8 chip8;

    validate_args(argc, argv);

    if (chip8_init(&chip8, argv[1]) != 0) {
        return EXIT_FAILURE;
    }

    printf("Loaded ROM: %s\n", argv[1]);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *window = SDL_CreateWindow("CHIP-8 Emulator",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          CHIP8_SCREEN_WIDTH * WINDOW_SCALE,
                                          CHIP8_SCREEN_HEIGHT * WINDOW_SCALE,
                                          SDL_WINDOW_SHOWN);

    if (!window) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture *texture = SDL_CreateTexture(renderer,
                                             SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             CHIP8_SCREEN_WIDTH,
                                             CHIP8_SCREEN_HEIGHT);

    uint32_t pixels[CHIP8_SCREEN_SIZE];
    int quit = 0;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }
        for (int i = 0; i < 10; i++) {
            cycle(&chip8);
        }

        for (int i = 0; i < CHIP8_SCREEN_SIZE; i++) {
            pixels[i] = chip8.display[i] ? 0xFFFFFFFF : 0xFF000000;
        }

        SDL_UpdateTexture(texture, NULL, pixels, CHIP8_SCREEN_WIDTH * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
