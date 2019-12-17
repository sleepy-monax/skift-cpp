#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libsystem/Format.h>

namespace libsystem
{

class Formattable
{
public:
    virtual libruntime::ErrorOr<size_t> format(Stream &stream, FormatInfo &info)
    {
        __unused(info);

        return libsystem::format(stream, "Object({#x})", reinterpret_cast<void *>(this));
    }
};

} // namespace libsystem
