#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libruntime/RefPtr.h>
#include <libsystem/Format.h>
#include <libsystem/Stream.h>

namespace libsystem
{

extern libruntime::RefPtr<Stream> stdin;
extern libruntime::RefPtr<Stream> stdout;
extern libruntime::RefPtr<Stream> stderr;
extern libruntime::RefPtr<Stream> stdlog;

template <typename... Args>
libruntime::ErrorOr<size_t> print(Args... args)
{
    return format(*stdout, args...);
}

} // namespace libsystem