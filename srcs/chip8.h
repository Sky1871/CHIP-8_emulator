#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  uint8_t opcode; // store current opcode //
  unsigned char memory[4096];
  unsigned char
      V[16]; // CPU registers: 15 general purpose (V0-VE), 16th is carry flag //

  uint8_t I;  // Index register (0x000-0xFFF) //
  uint8_t pc; // program counter (0x000-0xFFF) //
              // 0x000-0x1FF - Chip 8 interpreter (contains font set in emu) //
              // 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F) //
              // 0x200-0xFFF - Program ROM and work RAM //

  unsigned char gfx[64 * 32]; // 2048px //

  // timer registers count at 60 Hz, when set above zero they will count
  // down to zero. //
  unsigned char delay_timer;
  unsigned char sound_timer;

  uint8_t stack[16]; // stack is used to remember the current location
                     // before a jump is performed //
  uint8_t sp;        // stack pointer //

  unsigned char key[16]; // HEX based keypad (0x0-0xF) //
} chip8;
