#include "kernel.h"

#include <stddef.h>
#include <stdint.h>

#include "config.h"
#include "disk/disk.h"
#include "disk/streamer.h"
#include "fs/pparser.h"
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"
#include "memory/memory.h"
#include "memory/paging/paging.h"
#include "status.h"
#include "string/string.h"
#include "task/process.h"
#include "task/task.h"
#include "task/tss.h"

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

void panic(const char *msg) {
  print(msg);
  while (1) {
  }
}

struct tss tss;
struct gdt gdt_real[SAMPLEOS_TOTAL_GDT_SEGMENTS];
struct gdt_structured gdt_structured[SAMPLEOS_TOTAL_GDT_SEGMENTS] = {
    {.base = 0x00, .limit = 0x00, .type = 0x00},        // NULL Segment
    {.base = 0x00, .limit = 0xFFFFFFFF, .type = 0x9A},  // Code Segment
    {.base = 0x00, .limit = 0xFFFFFFFF, .type = 0x92},  // Data Segment
    {.base = 0x00, .limit = 0xFFFFFFFF, .type = 0xF8},  // User Code Segment
    {.base = 0x00, .limit = 0xFFFFFFFF, .type = 0xF2},  // User Data Segment
    {.base = (uint32_t)&tss,
     .limit = sizeof(tss),
     .type = 0xE9},  // TSS Segment
};

void kernel_main() {
  terminal_initialize();
  print("Hello, SampleOS!\n");

  memset(&gdt_real, 0x00, sizeof(gdt_real));
  gdt_structured_to_gdt(gdt_real, gdt_structured, SAMPLEOS_TOTAL_GDT_SEGMENTS);
  // Load the GDT
  gdt_load(gdt_real, sizeof(gdt_real));

  // Initialize the heap
  kheap_init();

  // Initialize filesystems
  fs_init();

  // Search and initialize the disk
  disk_search_and_init();

  // Initialize the IDT
  idt_init();

  // Setup the TSS
  memset(&tss, 0x00, sizeof(tss));
  tss.esp0 = 0x600000;
  tss.ss0 = KERNEL_DATA_SELECTOR;

  // Load the TSS
  tss_load(0x28);

  // Setup paging
  kernel_chunk = paging_new_4gb(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT |
                                PAGING_ACCESS_FROM_ALL);

  // Switch to kernel paging chunk
  paging_switch(paging_4gb_chunk_get_directory(kernel_chunk));

  // Enable paging
  enable_paging();

  struct process *process = 0;
  int res = process_load("0:/blank.bin", &process);
  if (res != SAMPLEOS_ALL_OK) {
    panic("Failed to load blank.bin\n");
  }
  print("1");

  task_run_first_ever_task();

  print("2");

  while (1) {
  }
}
