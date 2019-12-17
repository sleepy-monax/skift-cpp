#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

/* This header should be able to be compiled for C and C++                    */

#include <libc/__libc__.h>
#include <libruntime/Types.h>

LIBC_BEGIN_HEADER

size_t strlen(const char *str);

void strapd(char *str, char c);

void strrvs(char *str);

void *memset(void *dest, int ch, size_t size);

void *memcpy(void *dest, const void *src, size_t size);

void *memmove(void *dest, const void *src, size_t size);

int memcmp(const void *ptr1, const void *ptr2, size_t n);

LIBC_END_HEADER
