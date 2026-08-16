#include "chip8.h"

void input_err(int ac, char **av) {
  int i = 0;
  if (ac != 2) {
    if (ac == 1)
      printf("Please specify a .ch8 ROM to load.\n");
    else
      printf("Too many arguments.\n");
    exit(1);
  }

  size_t len = strlen(av[1]);
  if (len > 4 && strcmp(av[1] + len - 4, ".ch8") == 0)
    return;
  else
    printf("Invalid file\n");
  exit(1);
}

void init(chip8 *ch8, const char *filename) {
  memset(ch8, 0, sizeof(chip8));
  unsigned char chip8_fontset[80] = {
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

  ch8->pc = 0x200;
  ch8->opcode = 0;
  ch8->I = 0;
  ch8->sp = 0;
  ch8->delay_timer = 0;
  ch8->sound_timer = 0;

  for (int i = 0; i < 80; i++)
    ch8->memory[0x50 + i] = chip8_fontset[i];

  FILE *file = fopen(filename, "rb"); // rb = read binary

  if (file == NULL) {
    printf("Could not open %s\n", filename);
    exit(1);
  }

  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  rewind(file);

  if (size > (4096 - 0x200)) {
    printf("%s is too large\n", filename);
    fclose(file);
    exit(1);
  }

  size_t read = fread(&ch8->memory[0x200], 1, size, file);
  if (read != size) {
    printf("Could not read %s\n", filename);
    fclose(file);
    exit(1);
  }

  fclose(file);
}

int main(int ac, char **av) {
  input_err(ac, av);
  chip8 ch8;

  init(&ch8, av[1]);
}
