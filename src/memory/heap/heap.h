#ifndef SAMPLEOS_HEAP_H
#define SAMPLEOS_HEAP_H

#include <stddef.h>
#include <stdint.h>

#define HEAP_BLOCK_TABLE_ENTRY_TAKEN 0x01
#define HEAP_BLOCK_TABLE_ENTRY_FREE 0x00

#define HEAP_BLOCK_HAS_NEXT 0b10000000
#define HEAP_BLOCK_IS_FIRST 0b01000000

typedef unsigned char HEAP_BLOCK_TABLE_ENTRY;

struct heap_table {
  HEAP_BLOCK_TABLE_ENTRY* entries;
  size_t total;
};

struct heap {
  struct heap_table* table;

  // Start address of the heap table
  void* saddr;
};

int heap_create(struct heap* heap, void* ptr, void* end,
                struct heap_table* table);

void* heap_malloc(struct heap* heap, size_t size);

void heap_free(struct heap* heap, void* ptr);

#endif  // SAMPLEOS_HEAP_H
