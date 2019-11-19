#pragma once

/* This header should be able to be compiled for C and C++                    */

#include <libc/__libc__.h>
#include <libruntime/Types.h>

LIBC_BEGIN_HEADER

size_t strlen(const char *str);

void strapd(char *str, char c);

void strrvs(char *str);

void *memset(char *dest, int ch, size_t size);

void *memcpy(char *dest, const char *src, size_t size);

void *memmove(char *dest, const char *src, size_t size);

int memcmp(const void *ptr1, const void *ptr2, size_t n);

LIBC_END_HEADER
