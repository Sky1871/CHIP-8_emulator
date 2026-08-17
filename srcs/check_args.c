#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WINDOW_SCALE 10

static int has_ch8_extension(const char *path) {
    size_t len;

    len = strlen(path);
    return len >= 4 && strcmp(path + len - 4, ".ch8") == 0;
}

void validate_args(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s ROM.ch8\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (!has_ch8_extension(argv[1])) {
        fprintf(stderr, "Invalid ROM: expected a .ch8 file\n");
        exit(EXIT_FAILURE);
    }
}
