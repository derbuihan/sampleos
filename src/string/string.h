#ifndef SAMPLEOS_STRING_H
#define SAMPLEOS_STRING_H

#include <stdbool.h>

int strlen(const char *ptr);
int strnlen(const char *ptr, int max);
bool isdigit(char c);
int tonumericdigit(char c);
char *strcpy(char *dest, const char *src);

char tolower(char s1);
int strnlen_terminator(const char *ptr, int max, char terminator);
char *strncpy(char *dest, const char *src, int count);
int istrncmp(const char *s1, const char *s2, int n);
int strncmp(const char *s1, const char *s2, int n);

#endif  // SAMPLEOS_STRING_H
