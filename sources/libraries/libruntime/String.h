#pragma once

#include <libruntime/Move.h>
#include <libruntime/RefPtr.h>
#include <libruntime/StringBuffer.h>

namespace libruntime
{

class String
{
private:
    RefPtr<StringBuffer> _buffer;

public:
    const char *cstring() { return _buffer->cstring(); }
    size_t lenght() { return _buffer->lenght(); }

    String(const char *cstring = "")
    {
        _buffer = make<StringBuffer>(cstring);
    }

    String(const char *cstring, size_t lenght)
    {
        _buffer = make<StringBuffer>(cstring, lenght);
    }

    String(const String &other) : _buffer(const_cast<String &>(other)._buffer) {}

    String(String &&other) : _buffer(libruntime::move(other._buffer)) {}

    ~String() {}

    String &operator=(const String &other)
    {
        if (this != &other)
        {
            _buffer = const_cast<String &>(other)._buffer;
        }

        return *this;
    }

    String &operator=(String &&other)
    {
        if (this != &other)
        {
            _buffer = libruntime::move(other._buffer);
        }

        return *this;
    }
};

} // namespace libruntime
