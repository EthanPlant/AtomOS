#ifndef _STRING_H
#define _STRING_H 1

#include <sys/cdefs.h>

#include <stddef.h>

size_t strlen(const char*);

void* memset(void*, int, size_t);
void* memcpy(void* __restrict, const void* __restrict, size_t);
void* memmove(void*, const void*, size_t);
int memcmp(const void*, const void*, size_t);

#endif