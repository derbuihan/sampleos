#include "kernel.h"

#include <stddef.h>
#include <stdint.h>

#include "idt/idt.h"

uint16_t *video_mem = 0;

uint16_t terminal_make_char(char c, char color) { return c | (color << 8); }

void terminal_putchar(int x, int y, char c, char color) {
  video_mem[y * VGA_WIDTH + x] = terminal_make_char(c, color);
}

void terminal_writechar(char c, char color) {
  static int terminal_row = 0;
  static int terminal_col = 0;

  if (c == '\n') {
    terminal_row++;
    terminal_col = 0;
    return;
  }

  terminal_putchar(terminal_col, terminal_row, c, color);
  terminal_col++;
  if (terminal_col >= VGA_WIDTH) {
    terminal_col = 0;
    terminal_row++;
  }
}

void terminal_initialize() {
  video_mem = (uint16_t *)0xb8000;
  for (int y = 0; y < VGA_HEIGHT; y++) {
    for (int x = 0; x < VGA_WIDTH; x++) {
      terminal_putchar(x, y, ' ', 0);
    }
  }
}

size_t strlen(const char *str) {
  size_t len = 0;
  while (str[len]) {
    len++;
  }
  return len;
}

void print(const char *str) {
  size_t len = strlen(str);
  for (size_t i = 0; i < len; i++) {
    terminal_writechar(str[i], 0xf);
  }
}

void kernel_main() {
  terminal_initialize();
  print("Hello, World!\n");

  // Initialize the IDT
  idt_init();
}
