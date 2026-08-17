#pragma once

#include <stdint.h>

/* Memory */
#define CHIP8_MEMORY_SIZE 4096
#define CHIP8_PROGRAM_START 0x200
#define CHIP8_FONT_START 0x050
#define CHIP8_ROM_MAX_SIZE (CHIP8_MEMORY_SIZE - CHIP8_PROGRAM_START)

/* CPU */
#define CHIP8_REGISTER_COUNT 16
#define CHIP8_STACK_SIZE 16
#define CHIP8_KEY_COUNT 16

/* Display */
#define CHIP8_SCREEN_WIDTH 64
#define CHIP8_SCREEN_HEIGHT 32
#define CHIP8_SCREEN_SIZE (CHIP8_SCREEN_WIDTH * CHIP8_SCREEN_HEIGHT)

/* Font */
#define CHIP8_FONT_CHAR_COUNT 16
#define CHIP8_FONT_CHAR_SIZE 5
#define CHIP8_FONT_SIZE (CHIP8_FONT_CHAR_COUNT * CHIP8_FONT_CHAR_SIZE)

/* Opcode masks */
#define CHIP8_OPCODE_TYPE_MASK 0xF000
#define CHIP8_OPCODE_X_MASK 0x0F00
#define CHIP8_OPCODE_Y_MASK 0x00F0
#define CHIP8_OPCODE_N_MASK 0x000F
#define CHIP8_OPCODE_NN_MASK 0x00FF
#define CHIP8_OPCODE_NNN_MASK 0x0FFF

typedef struct s_chip8 {
    uint8_t memory[CHIP8_MEMORY_SIZE];
    uint8_t v[CHIP8_REGISTER_COUNT];

    uint16_t i;
    uint16_t pc;

    uint8_t display[CHIP8_SCREEN_SIZE];

    uint8_t delay_timer;
    uint8_t sound_timer;

    uint16_t stack[CHIP8_STACK_SIZE];
    uint8_t sp;

    uint8_t keys[CHIP8_KEY_COUNT];
} t_chip8;

int chip8_init(t_chip8 *chip8, const char *rom_path);
void cycle(t_chip8 *chip8);
