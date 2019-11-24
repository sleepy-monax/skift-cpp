#pragma once

#include <libc/string.h>
#include <libsystem/Stream.h>

namespace libsystem
{

class Formattable;

struct FormatInfo
{
    bool prefix;
    int base = 10;
};

libruntime::ErrorOr<size_t> format(Stream &stream, const char *fmt);

template <typename First, typename... Args>
libruntime::ErrorOr<size_t> format(Stream &stream, const char *fmt, First first, Args... args)
{
    size_t written = 0;

    for (size_t i = 0; fmt[i]; i++)
    {
        if (fmt[i] == '{')
        {
            FormatInfo info;

            for (; fmt[i] != '}'; i++)
            {
                if (fmt[i] == '#')
                    info.prefix = true;

                if (fmt[i] == 'x')
                    info.base = 16;
                else if (fmt[i] == 'd')
                    info.base = 10;
                else if (fmt[i] == 'o')
                    info.base = 8;
                else if (fmt[i] == 'b')
                    info.base = 2;
            }

            auto res_format = format(stream, first, info);

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

libruntime::ErrorOr<size_t> format(Stream &stream, const char *string, FormatInfo &info);

libruntime::ErrorOr<size_t> format(Stream &stream, void *value, FormatInfo &info);

libruntime::ErrorOr<size_t> format(Stream &stream, uint value, FormatInfo &info);

libruntime::ErrorOr<size_t> format(Stream &stream, Formattable &value, FormatInfo &info);

} // namespace libsystem
