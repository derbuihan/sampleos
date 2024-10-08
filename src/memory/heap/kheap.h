#ifndef SAMPLEOS_KHEAP_H
#define SAMPLEOS_KHEAP_H

#include <stddef.h>
#include <stdint.h>

void kheap_init();
void *kmalloc(size_t size);
void *kzalloc(size_t size);
void kfree(void *ptr);

#endif  // SAMPLEOS_KHEAP_H
