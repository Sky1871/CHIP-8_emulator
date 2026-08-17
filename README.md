# CHIP-8_emulator

A lightweight CHIP-8 emulator written in C. This project uses **SDL2** to handle window creation, graphics rendering, and keyboard input, providing a faithful recreation of the classic virtual machine.

---

## Features

* **Display:** Accurately renders the standard 64x32 monochrome display, scaled up by 10x for modern screens (640x320 window resolution).
* **Input:** Fully mapped 16-key hexadecimal keypad.
* **Timers:** Emulates both the Delay and Sound timers at ~60Hz.
* **Architecture:** Implements standard CHIP-8 memory layout, 16 8-bit registers (V0-VF), the index register (I), and the program counter (PC).

---

## Prerequisites

To build and run this emulator, you will need the following installed on your system:

* **A C Compiler** (e.g., `gcc` or `clang`)
* **Make**
* **SDL2 Development Libraries** (`libsdl2-dev` on Debian/Ubuntu, `sdl2` on Homebrew/macOS)
* **pkg-config** (Used by the Makefile to locate SDL2 headers and libraries)

---

## Building the Emulator

The project includes a comprehensive `Makefile`. Simply navigate to the root directory of the project in your terminal and run:

```bash
make

```

This will compile the source files and generate the executable named `CHIP-8_emulator`.

### Advanced Build Targets

* **`make debug`**: Compiles the emulator with debug symbols (`-g3`) and removes optimizations (`-O0`) for use with GDB.
* **`make san`**: Compiles the project with Address and Undefined Behavior sanitizers enabled (`-fsanitize=address,undefined`).
* **`make clean`**: Removes all compiled object files (`.o` and `.d` files).
* **`make fclean`**: Performs a clean and also removes the final executable.
* **`make re`**: Recompiles the entire project from scratch.

---

## Usage

The emulator requires a valid CHIP-8 ROM file (must have a `.ch8` extension) passed as an argument.

```bash
./CHIP-8_emulator path/to/your/rom.ch8

```

---

## Key Mappings

The original CHIP-8 used a 16-key hexadecimal keypad (0-F). This emulator maps those original keys to the left side of a modern QWERTY keyboard.

| Original CHIP-8 Key | Modern Keyboard Key |
| --- | --- |
| **1** | 1 |
| **2** | 2 |
| **3** | 3 |
| **C** | 4 |
| **4** | Q |
| **5** | W |
| **6** | E |
| **D** | R |
| **7** | A |
| **8** | S |
| **9** | D |
| **E** | F |
| **A** | Z |
| **0** | X |
| **B** | C |
| **F** | V |

To exit the emulator, simply close the SDL2 window.
