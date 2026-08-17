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
            } else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
                uint8_t state = (e.type == SDL_KEYDOWN) ? 1 : 0;

                switch (e.key.keysym.sym) {
                    case SDLK_1:
                        chip8.keys[0x1] = state;
                        break;
                    case SDLK_2:
                        chip8.keys[0x2] = state;
                        break;
                    case SDLK_3:
                        chip8.keys[0x3] = state;
                        break;
                    case SDLK_4:
                        chip8.keys[0xC] = state;
                        break;

                    case SDLK_q:
                        chip8.keys[0x4] = state;
                        break;
                    case SDLK_w:
                        chip8.keys[0x5] = state;
                        break;
                    case SDLK_e:
                        chip8.keys[0x6] = state;
                        break;
                    case SDLK_r:
                        chip8.keys[0xD] = state;
                        break;

                    case SDLK_a:
                        chip8.keys[0x7] = state;
                        break;
                    case SDLK_s:
                        chip8.keys[0x8] = state;
                        break;
                    case SDLK_d:
                        chip8.keys[0x9] = state;
                        break;
                    case SDLK_f:
                        chip8.keys[0xE] = state;
                        break;

                    case SDLK_z:
                        chip8.keys[0xA] = state;
                        break;
                    case SDLK_x:
                        chip8.keys[0x0] = state;
                        break;
                    case SDLK_c:
                        chip8.keys[0xB] = state;
                        break;
                    case SDLK_v:
                        chip8.keys[0xF] = state;
                        break;
                }
            }
        }
        for (int i = 0; i < 10; i++) {
            cycle(&chip8);
        }

        if (chip8.delay_timer > 0) {
            chip8.delay_timer--;
        }
        if (chip8.sound_timer > 0) {
            if (chip8.sound_timer == 1) {
                printf("\a");
            }
            chip8.sound_timer--;
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
