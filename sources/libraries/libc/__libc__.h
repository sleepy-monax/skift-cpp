#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

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
