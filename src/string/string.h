#ifndef SAMPLEOS_STRING_H
#define SAMPLEOS_STRING_H

#include <stdbool.h>

int strlen(const char *ptr);
int strnlen(const char *ptr, int max);
bool isdigit(char c);
int tonumericdigit(char c);

#endif  // SAMPLEOS_STRING_H
