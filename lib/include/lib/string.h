#ifndef LIB_STRING_H
#define LIB_STRING_H

#include <stddef.h>

size_t strlen(const char* str);
int strcmp(const char* a, const char* b);
char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t n);
void* memcpy(void* dest, const void* src, size_t n);

#endif
