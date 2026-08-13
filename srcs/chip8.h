#ifndef CHIP8_H
# define CHIP8_H

typdef struct CHIP8
{
  unsigned short opcode; // store current opcode //
  unsigned char memory[4096];
  unsigned char V[16]; // CPU registers: 15 general purpose (V0-VE), 16th is carry flag //

  unsigned short I; // Index register (0x000-0xFFF) //
  unsigned short pc; // program counter (0x000-0xFFF) //
    // 0x000-0x1FF - Chip 8 interpreter (contains font set in emu) //
    // 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F) //
    // 0x200-0xFFF - Program ROM and work RAM //

  unsigned char gfx[64 * 32] // 2048px //

  // timer registers count at 60 Hz, when set above zero they will count down to zero. //
  unsigned char delay_timer;
  unsigned char sound_timer;

  unsigned short stack[16]; // stack is used to remember the current location before a jump is performed //
  unsigned short sp; // stack pointer //

  unsigned char key[16]; // HEX based keypad (0x0-0xF) //
} chip8;

#endif
