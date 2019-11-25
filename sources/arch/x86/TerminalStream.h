#pragma once

#include <libsystem/Stream.h>
#include <arch/x86/CGAScreen.h>

class TerminalStream : public libsystem::Stream
{
private:
    CGAScreen _screen;

    size_t _row = 0;
    size_t _column = 0;
    CGAColor _foreground = CGAColor::LIGHT_GREY;
    CGAColor _background = CGAColor::BLACK;

public:
    TerminalStream(CGAScreen screen) : _screen(screen)
    {
        screen.clear(_foreground, _background);
    }

    ~TerminalStream() {}

    bool writable() override
    {
        return true;
    }

    virtual libruntime::Error write_byte(byte byte)
    {
        if (byte == '\n')
        {
            _column = 0;
            if (++_row == CGA_HEIGHT)
            {
                _row = CGA_HEIGHT - 1;
                _screen.scroll(_foreground, _background);
            }
        }
        else
        {
            _screen.set(_column, _row, byte, _foreground, _background);

            if (++_column == CGA_WIDTH)
            {
                _column = 0;
                if (++_row == CGA_HEIGHT)
                    _row = 0;
            }
        }

        return libruntime::Error::SUCCEED;
    }
};
