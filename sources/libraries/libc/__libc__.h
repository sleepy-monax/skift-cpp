#pragma once

#ifdef __cplusplus

#define LIBC_BEGIN_HEADER \
    namespace libc        \
    {                     \
    extern "C"            \
    {

#define LIBC_END_HEADER \
    }                   \
    } // namespace libc

#else

#define LIBC_BEGIN_HEADER

#define LIBC_END_HEADER

#endif
