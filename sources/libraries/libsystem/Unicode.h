#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libruntime/Types.h>
#include <libruntime/Callback.h>

namespace libsystem
{

// FIXME: only ascii is supported... sigh...
class Codepoint
{
private:
    char32_t _value;

public:
    Codepoint() : _value(0) {}
    Codepoint(char32_t value) : _value(value) {}
    ~Codepoint() {}

    bool is_digit()
    {
        return (_value >= U'0' && _value <= U'9');
    }

    bool is_alpha()
    {
        return (_value >= U'a' && _value <= U'z') ||
               (_value >= U'A' && _value <= U'Z');
    }

    int numeric_value()
    {
        if (is_digit())
        {
            return _value - U'0';
        }
        else
        {
            return 0;
        }
    }

    operator char32_t() { return _value; }
    bool operator==(char32_t other) { return _value == other; }
};

class UTF8Decoder
{
private:
    libruntime::Callback<void(Codepoint)> _callback;

    bool _is_decoding = false;
    char32_t _current = 0;
    int _width = 0;

public:
    template <typename Callback>
    UTF8Decoder(Callback callback) : _callback(callback) {}
    ~UTF8Decoder() {}

    void write(const uint8_t *bytes, size_t count)
    {
        for (size_t i = 0; i < count; i++)
        {
            uint8_t byte = bytes[i];

            if (!_is_decoding)
            {
                if ((byte & 0xf8) == 0xf0)
                {
                    _current = (0x07 & byte) << 18;

                    _is_decoding = true;
                    _width = 3;
                }
                else if ((byte & 0xf0) == 0xe0)
                {
                    _current = (0x0f & byte) << 12;

                    _is_decoding = true;
                    _width = 2;
                }
                else if ((byte & 0xe0) == 0xc0)
                {
                    _current = (0x1f & byte) << 6;

                    _is_decoding = true;
                    _width = 1;
                }
                else
                {
                    _current = byte;

                    _is_decoding = true;
                    _width = 0;
                }
            }
            else
            {
                if ((byte & 0b11000000) == 0b10000000)
                {
                    _width--;
                    _current = (0x3f & byte) << (6 * _width);
                }
                else
                {
                    // FIXME: maybe have a special callback for this case.
                    _callback(Codepoint(U'?'));
                    _is_decoding = false;
                }
            }

            if (_is_decoding && _width == 0)
            {
                _callback(Codepoint(_current));
                _is_decoding = false;
            }
        }
    }
};

} // namespace libsystem
