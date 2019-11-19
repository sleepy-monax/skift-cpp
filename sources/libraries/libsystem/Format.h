#pragma once

#include <libc/string.h>

#include <libsystem/Stream.h>
#include <libsystem/Convert.h>

namespace libsystem
{

libruntime::ErrorOr<size_t> format(Stream &stream, const char *string)
{
    return stream.write(static_cast<const void *>(string), libc::strlen(string));
}

libruntime::ErrorOr<size_t> format(Stream &stream, uint value)
{
    char buffer[32];

    convert_uint_to_string(value, buffer, 10);

    return stream.write(static_cast<const void *>(buffer), libc::strlen(buffer));
}

template <typename First, typename... Args>
libruntime::ErrorOr<size_t> format(Stream &stream, const char *fmt, First first, Args... args)
{
    size_t written = 0;

    for (size_t i = 0; fmt[i]; i++)
    {
        if (fmt[i] == '{')
        {
            for (; fmt[i] != '}'; i++)
            {
                // TODO: format specifier
            }

            auto res_format = format(stream, first);

            written += res_format.value();

            if (res_format != libruntime::Error::SUCCEED)
            {
                return libruntime::ErrorOr<size_t>(res_format.error(), written);
            }

            auto res_print = format(stream, &fmt[i + 1], args...);

            written += res_print.value();

            if (res_print != libruntime::Error::SUCCEED)
            {
                return libruntime::ErrorOr<size_t>(res_print.error(), written);
            }
            else
            {
                return libruntime::ErrorOr<size_t>(written);
            }
        }
        else
        {
            auto err = stream.write_byte(fmt[i]);

            if (err != libruntime::Error::SUCCEED)
            {
                return libruntime::ErrorOr<size_t>(err, written);
            }

            written += 1;
        }
    }

    return libruntime::ErrorOr<size_t>(written);
}

template <typename... Args>
libruntime::ErrorOr<size_t> format(Stream *stream, const char *fmt, Args... args)
{
    return format(*stream, fmt, args...);
}

libruntime::ErrorOr<size_t> format(Stream *stream, const char *fmt)
{
    return format(*stream, fmt);
}

} // namespace libsystem
