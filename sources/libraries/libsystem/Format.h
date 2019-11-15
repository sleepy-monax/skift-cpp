#pragma once

#include <libc/string.h>
#include <libsystem/Stream.h>

namespace libsystem
{

ErrorOr<size_t> format(Stream &stream, const char *string)
{
    return stream.write(static_cast<const void *>(string), libc::strlen(string));
}

ErrorOr<size_t> format(Stream &stream, int value)
{
    char buffer[32];

    __unused(value);

    return stream.write(static_cast<const void *>(buffer), libc::strlen(buffer));
}

template <typename First, typename... Args>
ErrorOr<size_t> format(Stream &stream, const char *fmt, First first, Args... args)
{
    size_t written = 0;

    for (size_t i = 0; fmt[i]; i++)
    {
        if (fmt[i] == '%')
        {
            auto res_format = format(stream, first);

            written += res_format.value();

            if (res_format != Error::SUCCEED)
            {
                return ErrorOr<size_t>(res_format.error(), written);
            }

            auto res_print = format(stream, &fmt[i + 1], args...);

            written += res_print.value();

            if (res_print != Error::SUCCEED)
            {
                return ErrorOr<size_t>(res_print.error(), written);
            }
            else
            {
                return ErrorOr<size_t>(written);
            }
        }
        else
        {
            auto err = stream.write_byte(fmt[i]);

            if (err != Error::SUCCEED)
            {
                return ErrorOr<size_t>(err, written);
            }

            written += 1;
        }
    }

    return ErrorOr<size_t>(written);
}

ErrorOr<size_t> format(Stream *stream, const char *fmt)
{
    return format(*stream, fmt);
}

template <typename First, typename... Args>
ErrorOr<size_t> format(Stream *stream, const char *fmt, First first, Args... args)
{
    return format(*stream, fmt, first, args...);
}

} // namespace libsystem
