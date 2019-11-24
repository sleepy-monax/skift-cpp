#include <libruntime/Macros.h>
#include <libsystem/Convert.h>
#include <libsystem/Format.h>
#include <libsystem/Formattable.h>

namespace libsystem
{

libruntime::ErrorOr<size_t> format(Stream &stream, const char *fmt)
{
    return stream.write(fmt, libc::strlen(fmt));
}

libruntime::ErrorOr<size_t> format(Stream &stream, const char *string, FormatInfo &info)
{
    __unused(info);

    return stream.write(static_cast<const void *>(string), libc::strlen(string));
}

libruntime::ErrorOr<size_t> format(Stream &stream, void *value, FormatInfo &info)
{
    return format(stream, reinterpret_cast<uintptr_t>(value), info);
}

libruntime::ErrorOr<size_t> format(Stream &stream, uint value, FormatInfo &info)
{
    char buffer[32];

    if (info.prefix)
    {
    }

    convert_uint_to_string(value, buffer, info.base);

    return stream.write(static_cast<const void *>(buffer), libc::strlen(buffer));
}

libruntime::ErrorOr<size_t> format(Stream &stream, Formattable &value, FormatInfo &info)
{
    return value.format(stream, info);
}

}; // namespace libsystem