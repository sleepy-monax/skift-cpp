#pragma once

#include <libruntime/Macros.h>

namespace libsystem
{

#ifndef assert
#define assert(__expr)                                                      \
    ({                                                                      \
        if (!(__expr))                                                      \
        {                                                                   \
            libsystem::__assert_failled(__FILE__, __LINE__, __str(__expr)); \
        }                                                                   \
    })

void __assert_failled(const char *file, int line, const char *expr);
#endif

#define assert_not_reached() libsystem::__assert_not_reached_reached(__FILE__, __LINE__)

void __assert_not_reached_reached(const char *file, int line) __noreturn;

} // namespace libsystem
