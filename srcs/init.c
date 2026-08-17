#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
