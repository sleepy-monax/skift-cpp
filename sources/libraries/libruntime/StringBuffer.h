#pragma once

#include <libc/string.h>
#include <libruntime/RefCounted.h>
#include <libruntime/Types.h>

namespace libruntime
{

class StringBuffer : public RefCounted<StringBuffer>
{
private:
    size_t _lenght;
    char *_buffer;

public:
    const char *cstring() { return _buffer; }
    size_t lenght() { return _lenght; }

    StringBuffer(const char *cstring) : StringBuffer(cstring, libc::strlen(cstring)){};

    StringBuffer(const char *cstring, size_t lenght)
    {
        _lenght = lenght;
        _buffer = new char[lenght + 1];
        libc::memcpy(_buffer, cstring, lenght);
        _buffer[lenght] = '\0';
    }

    ~StringBuffer()
    {
        delete _buffer;
    }
};

} // namespace libruntime
