#ifndef SAMPLEOS_MEMORY_H
#define SAMPLEOS_MEMORY_H

#include <stddef.h>

void *memset(void *ptr, int c, size_t size);
int memcmp(void *s1, void *s2, int count);

#endif  // SAMPLEOS_MEMORY_H
