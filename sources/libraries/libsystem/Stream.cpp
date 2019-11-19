#include <libsystem/Stream.h>
#include <libruntime/Macros.h>

using namespace libruntime;

namespace libsystem
{

Stream::Stream()
{
}

Stream::~Stream() {}

bool Stream::readable()
{
    return false;
}

bool Stream::writable()
{
    return false;
}

bool Stream::seekable()
{
    return false;
}

ErrorOr<size_t> Stream::read(void *buffer, size_t size)
{
    size_t readed;

    for (readed = 0; readed < size; readed++)
    {
        auto result = read_byte();

        if (result != Error::SUCCEED)
        {
            return ErrorOr<size_t>(result.error(), readed);
        }
        else
        {
            ((byte *)buffer)[readed] = result.value();
        }
    }

    return ErrorOr<size_t>(readed);
}

ErrorOr<size_t> Stream::write(const void *buffer, size_t size)
{
    size_t written;

    for (written = 0; written < size; written++)
    {
        auto result = write_byte(((byte *)buffer)[written]);

        if (result != Error::SUCCEED)
        {
            return ErrorOr<size_t>(result, written);
        }
    }

    return ErrorOr<size_t>(written);
}

ErrorOr<byte> Stream::read_byte()
{
    return ErrorOr<byte>(Error::NOT_IMPLEMENTED);
}

Error Stream::write_byte(byte byte)
{
    __unused(byte);

    return Error::NOT_IMPLEMENTED;
}

ErrorOr<size_t> Stream::seek(size_t offset, SeekOrigine origine)
{
    __unused(offset);
    __unused(origine);

    return ErrorOr<size_t>(Error::NOT_IMPLEMENTED);
}

ErrorOr<size_t> Stream::tell()
{
    return ErrorOr<size_t>(Error::NOT_IMPLEMENTED);
}

void Stream::flush() {}

} // namespace libsystem
