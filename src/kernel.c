#include "kernel.h"

#include <stddef.h>
#include <stdint.h>

#include "disk/disk.h"
#include "fs/pparser.h"
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"
#include "string/string.h"

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

void print(const char *str) {
  size_t len = strlen(str);
  for (size_t i = 0; i < len; i++) {
    terminal_writechar(str[i], 0xf);
  }
}

static struct paging_4gb_chunk *kernel_chunk = 0;

void kernel_main() {
  terminal_initialize();
  print("Hello, SampleOS!\n");

  // Initialize the heap
  kheap_init();

  // Search and initialize the disk
  disk_search_and_init();

  // Initialize the IDT
  idt_init();

  // Setup paging
  kernel_chunk = paging_new_4gb(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT |
                                PAGING_ACCESS_FROM_ALL);

  // Switch to kernel paging chunk
  paging_switch(paging_4gb_chunk_get_directory(kernel_chunk));

  // Enable paging
  enable_paging();

  // Enable interrupts
  enable_interrupts();

  struct path_root *root_path = pathparser_parse("0:/bin/shell.exe", NULL);
  struct path_part *part = root_path->first;

  while (part) {
    print("/");
    print(part->part);
    part = part->next;
  }
}
