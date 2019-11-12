#pragma once

#include <libruntime/Types.h>
#include <libruntime/ErrorOr.h>

namespace libsystem
{

using namespace libruntime;

enum SeekOrigine
{
    BEGIN,
    CURRENT,
    END,
};

class Stream
{
public:
    Stream() {}

    ~Stream() {}

    virtual bool readable()
    {
        return false;
    }

    virtual bool writable()
    {
        return false;
    }

    virtual bool seekable()
    {
        return false;
    }

    virtual ErrorOr<size_t> read(void *buffer, size_t size)
    {
        size_t readed;

        for (readed = 0; readed < size; readed++)
        {
            auto result = read_byte();

            if (result != Error::SUCCEED)
            {
                return (result.error(), readed);
            }
            else
            {
                ((byte *)buffer)[readed] = result.value();
            }
        }

        return (readed);
    }

    virtual ErrorOr<size_t> write(const void *buffer, size_t size)
    {
        size_t written;

        for (written = 0; written < size; written++)
        {
            auto result = write_byte(((byte *)buffer)[written]);

            if (result != Error::SUCCEED)
            {
                return (result, written);
            }
        }

        return (written);
    }

    virtual ErrorOr<byte> read_byte()
    {
        return (Error::NOT_IMPLEMENTED);
    }

    virtual Error write_byte(byte byte)
    {
        return Error::NOT_IMPLEMENTED;
    }

    virtual ErrorOr<size_t> seek(size_t offset, SeekOrigine origine)
    {
        return (Error::NOT_IMPLEMENTED);
    }

    virtual ErrorOr<size_t> tell(SeekOrigine origine)
    {
        return (Error::NOT_IMPLEMENTED);
    }

    virtual void flush() {}
};

} // namespace libsystem
