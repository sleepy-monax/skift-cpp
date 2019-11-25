#pragma once

#include <arch/x86/x86.h>
#include <libsystem/Stream.h>
#include <libruntime/Types.h>

enum class SerialPort : u16
{
    COM1 = 0x3F8,
    COM2 = 0x2F8,
    COM3 = 0x3E8,
    COM4 = 0x2E8,
};

class SerialStream : public libsystem::Stream
{
private:
    SerialPort _port;

    void wait_read()
    {
        while ((x86::in8(static_cast<u16>(_port) + 5) & 1) == 0)
        {
            /* do nothing */
        }
    }

    void wait_write()
    {
        while ((x86::in8(static_cast<u16>(_port) + 5) & 0x20) == 0)
        {
            /* do nothing */
        }
    }

public:
    SerialStream(SerialPort port) : _port(port)
    {
        x86::out8(static_cast<u16>(_port) + 1, 0x00);
        x86::out8(static_cast<u16>(_port) + 3, 0x80);
        x86::out8(static_cast<u16>(_port) + 0, 0x03);
        x86::out8(static_cast<u16>(_port) + 1, 0x00);
        x86::out8(static_cast<u16>(_port) + 3, 0x03);
        x86::out8(static_cast<u16>(_port) + 2, 0xC7);
        x86::out8(static_cast<u16>(_port) + 4, 0x0B);
    }

    ~SerialStream() {}

    bool writable() override
    {
        return true;
    }

    virtual libruntime::Error write_byte(byte byte)
    {
        wait_write();

        x86::out8(static_cast<u16>(_port), byte);

        return libruntime::Error::SUCCEED;
    }
};
