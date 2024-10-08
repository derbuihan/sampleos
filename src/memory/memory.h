#ifndef SAMPLEOS_MEMORY_H
#define SAMPLEOS_MEMORY_H

#include <stddef.h>

void *memset(void *ptr, int c, size_t size);
int memcmp(void *s1, void *s2, int count);
void *memcpy(void *dest, void *src, int len);

#endif  // SAMPLEOS_MEMORY_H
