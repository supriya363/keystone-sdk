#ifndef __STRING_H__
#define __STRING_H__
#include <stddef.h>
#include <stdint.h>
void* memcpy(void* dest, const void* src, size_t len);
void* memset(void* dest, int byte, size_t len);
int memcmp(const void* ptr1, const void* ptr2, size_t len);
void *memmove(void *dest, const void *src, size_t count);
size_t strlen(const char* str);
int strcmp(const char *s1, const char *s2);
char *strncpy(char *dest, const char *src, size_t n);

#endif
