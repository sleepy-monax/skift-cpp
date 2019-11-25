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

static const char *get_prefix(int base)
{
    switch (base)
    {
    case 2:
        return "0b";

    case 8:
        return "0";

    case 16:
        return "0x";

    default:
        return "";
    }
}

libruntime::ErrorOr<size_t> format(Stream &stream, uint value, FormatInfo &info)
{
    char buffer[32];
    convert_uint_to_string(value, buffer, info.base);

    if (info.prefix == true)
    {
        return format(stream, "{}{}", get_prefix(info.base), buffer);
    }
    else
    {
        return stream.write(static_cast<const void *>(buffer), libc::strlen(buffer));
    }
}

libruntime::ErrorOr<size_t> format(Stream &stream, Formattable &value, FormatInfo &info)
{
    return value.format(stream, info);
}

}; // namespace libsystem