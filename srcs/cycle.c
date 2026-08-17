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
        case 0x2000:
            chip8->stack[chip8->sp] = chip8->pc;
            chip8->sp++;
            chip8->pc = nnn;
            break;
        case 0x3000:
            if (chip8->v[x] == nn) {
                chip8->pc += 2;
            }
            break;
        case 0x4000:
            if (chip8->v[x] != nn) {
                chip8->pc += 2;
            }
            break;
        case 0x5000:
            if (chip8->v[x] == chip8->v[y]) {
                chip8->pc += 2;
            }
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
        case 0x8000:
            switch (z) {
                case 0x0:
                    chip8->v[x] = chip8->v[y];
                    break;
                case 0x1:
                    chip8->v[x] |= chip8->v[y];
                    break;
                case 0x2:
                    chip8->v[x] &= chip8->v[y];
                    break;
                case 0x3:
                    chip8->v[x] ^= chip8->v[y];
                    break;
                case 0x4: {
                    uint16_t sum = chip8->v[x] + chip8->v[y];
                    chip8->v[x] = sum & 0xFF;
                    chip8->v[0xF] = (sum > 0xFF) ? 1 : 0;
                    break;
                }
                case 0x5: {
                    uint8_t flag = (chip8->v[x] >= chip8->v[y]) ? 1 : 0;
                    chip8->v[x] -= chip8->v[y];
                    chip8->v[0xF] = flag;
                    break;
                }
                case 0x6: {
                    uint8_t flag = chip8->v[x] & 0x1;
                    chip8->v[x] >>= 1;
                    chip8->v[0xF] = flag;
                    break;
                }
                case 0x7: {
                    uint8_t flag = (chip8->v[y] >= chip8->v[x]) ? 1 : 0;
                    chip8->v[x] = chip8->v[y] - chip8->v[x];
                    chip8->v[0xF] = flag;
                    break;
                }
                case 0xE: {
                    uint8_t flag = (chip8->v[x] & 0x80) >> 7;
                    chip8->v[x] <<= 1;
                    chip8->v[0xF] = flag;
                    break;
                }
                default:
                    fprintf(stderr, "Unknown opcode: 0x%04X\n", opcode);
                    break;
            }
            break;
        case 0x9000:
            if (chip8->v[x] != chip8->v[y]) {
                chip8->pc += 2;
            }
            break;
        case 0xB000:
            chip8->pc = nnn + chip8->v[0];
            break;
        case 0xC000:
            chip8->v[x] = (rand() % 256) & nn;
            break;
        case 0xE000:
            if (nn == 0x9E) {
                if (chip8->keys[chip8->v[x]]) {
                    chip8->pc += 2;
                }
            } else if (nn == 0xA1) {
                if (!chip8->keys[chip8->v[x]]) {
                    chip8->pc += 2;
                }
            }
            break;
        case 0xF000:
            switch (nn) {
                case 0x0A: {
                    uint8_t key_pressed = 0;

                    for (int i = 0; i < CHIP8_KEY_COUNT; i++) {
                        if (chip8->keys[i]) {
                            chip8->v[x] = i;
                            key_pressed = 1;
                            break;
                        }
                    }

                    if (!key_pressed) {
                        chip8->pc -= 2;
                    }
                    break;
                }
                case 0x07:
                    chip8->v[x] = chip8->delay_timer;
                    break;
                case 0x15:
                    chip8->delay_timer = chip8->v[x];
                    break;
                case 0x18:
                    chip8->sound_timer = chip8->v[x];
                    break;
                case 0x1E:
                    chip8->i += chip8->v[x];
                    break;
                case 0x29:
                    chip8->i = CHIP8_FONT_START + (chip8->v[x] * 5);
                    break;
                case 0x33:
                    chip8->memory[chip8->i] = chip8->v[x] / 100;
                    chip8->memory[chip8->i + 1] = (chip8->v[x] / 10) % 10;
                    chip8->memory[chip8->i + 2] = chip8->v[x] % 10;
                    break;
                case 0x55:
                    for (int i = 0; i <= x; i++) {
                        chip8->memory[chip8->i + i] = chip8->v[i];
                    }
                    chip8->i += x + 1;
                    break;
                case 0x65:
                    for (int i = 0; i <= x; i++) {
                        chip8->v[i] = chip8->memory[chip8->i + i];
                    }
                    chip8->i += x + 1;
                    break;
                default:
                    fprintf(stderr, "Unknown opcode: 0x%04X\n", opcode);
                    break;
            }
    }
}
