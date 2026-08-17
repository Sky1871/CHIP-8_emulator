#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>

void cycle(t_chip8 *chip8) {
    uint16_t opcode = (chip8->memory[chip8->pc] << 8) | chip8->memory[chip8->pc + 1];
    chip8->pc += 2;

    uint16_t type = opcode & CHIP8_OPCODE_TYPE_MASK;
    uint8_t x = (opcode & CHIP8_OPCODE_X_MASK) >> 8;
    uint8_t y = (opcode & CHIP8_OPCODE_Y_MASK) >> 4;
    uint8_t z = opcode & CHIP8_OPCODE_N_MASK;
    uint8_t nn = opcode & CHIP8_OPCODE_NN_MASK;
    uint16_t nnn = opcode & CHIP8_OPCODE_NNN_MASK;

    switch (type) {
        case 0x0000:
            if (opcode == 0x00E0) {
                for (int i = 0; i < CHIP8_SCREEN_SIZE; i++) {
                    chip8->display[i] = 0;
                }
            } else if (opcode == 0x00EE) {
                chip8->sp--;
                chip8->pc = chip8->stack[chip8->sp];
            }
            break;
        case 0x1000:
            chip8->pc = nnn;
            break;
        case 0x6000:
            chip8->v[x] = nn;
            break;
        case 0x7000:
            chip8->v[x] += nn;
            break;
        case 0xA000:
            chip8->i = nnn;
            break;
        case 0xD000: {
            uint8_t x_start = chip8->v[x] % CHIP8_SCREEN_WIDTH;
            uint8_t y_start = chip8->v[y] % CHIP8_SCREEN_HEIGHT;

            chip8->v[0xF] = 0;

            for (int row = 0; row < z; row++) {
                uint8_t sprite_byte = chip8->memory[chip8->i + row];

                if (y_start + row >= CHIP8_SCREEN_HEIGHT) {
                    break;
                }
                for (int col = 0; col < 8; col++) {
                    if (x_start + col >= CHIP8_SCREEN_WIDTH) {
                        break;
                    }

                    if (sprite_byte & (0x80 >> col)) {
                        int screen_idx = (y_start + row) * CHIP8_SCREEN_WIDTH + (x_start + col);

                        if (chip8->display[screen_idx] == 1) {
                            chip8->v[0xF] = 1;
                        }
                        chip8->display[screen_idx] ^= 1;
                    }
                }
            }
            break;
        }
    }
}
