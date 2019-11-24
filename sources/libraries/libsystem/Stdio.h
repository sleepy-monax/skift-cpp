#pragma once

#include <libsystem/Stream.h>
#include <libsystem/Format.h>
#include <libruntime/RefPtr.h>

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