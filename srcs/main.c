#include "chip8.h"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WINDOW_SCALE 10

static int has_ch8_extension(const char *path) {
    size_t len;

    len = strlen(path);
    return len >= 4 && strcmp(path + len - 4, ".ch8") == 0;
}

static void validate_args(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s ROM.ch8\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (!has_ch8_extension(argv[1])) {
        fprintf(stderr, "Invalid ROM: expected a .ch8 file\n");
        exit(EXIT_FAILURE);
    }
}

static void init_fontset(t_chip8 *chip8) {
    static const uint8_t font[CHIP8_FONT_SIZE] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    memcpy(&chip8->memory[CHIP8_FONT_START], font, sizeof(font));
}

static int load_rom(t_chip8 *chip8, const char *path) {
    FILE *file;
    long size;
    size_t bytes_read;

    file = fopen(path, "rb");
    if (file == NULL) {
        perror(path);
        return -1;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        perror("fseek");
        fclose(file);
        return -1;
    }

    size = ftell(file);
    if (size < 0) {
        perror("ftell");
        fclose(file);
        return -1;
    }

    if ((size_t)size > CHIP8_ROM_MAX_SIZE) {
        fprintf(stderr, "%s: ROM is too large\n", path);
        fclose(file);
        return -1;
    }

    rewind(file);

    bytes_read = fread(&chip8->memory[CHIP8_PROGRAM_START], 1, (size_t)size, file);

    if (bytes_read != (size_t)size) {
        fprintf(stderr, "%s: failed to read ROM\n", path);
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}

int chip8_init(t_chip8 *chip8, const char *rom_path) {
    memset(chip8, 0, sizeof(*chip8));

    chip8->pc = CHIP8_PROGRAM_START;

    init_fontset(chip8);

    if (load_rom(chip8, rom_path) != 0) {
        return -1;
    }

    return 0;
}

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
