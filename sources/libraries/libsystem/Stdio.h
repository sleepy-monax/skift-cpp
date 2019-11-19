#pragma once

#include <libsystem/Stream.h>
#include <libsystem/Format.h>

namespace libsystem
{

extern Stream *stdin;
extern Stream *stdout;
extern Stream *stderr;
extern Stream *stdlog;

template <typename... Args>
libruntime::ErrorOr<size_t> print(Args... args)
{
    return format(stdout, args...);
}

} // namespace libsystem