#pragma once

#include <libc/__libc__.h>
#include <libruntime/Types.h>

LIBC_BEGIN_HEADER

static size_t strlen(const char *str)
{
    size_t len = 0;

    while (str[len])
    {
        len++;
    }

    return len;
}

LIBC_END_HEADER