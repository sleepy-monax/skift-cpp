#include <libc/string.h>
#include <libsystem/FileStream.h>
#include <libsystem/__plugs__.h>

using namespace libruntime;

namespace libsystem
{

ErrorOr<RefPtr<FileStream>> FileStream::open(const char *path, FileStreamFlags flags)
{
    auto handle = __plugs__::file_open(path, libc::strlen(path), flags);

    if (handle == Error::SUCCEED)
    {
        return libruntime::make<FileStream>(handle.value());
    }
    else
    {
        return ErrorOr<RefPtr<FileStream>>(handle.error());
    }
}

FileStream::FileStream(int handle, FileStreamFlags flags) : _handle(handle), _flags(flags) {}

FileStream::~FileStream()
{
    __plugs__::file_close(_handle);
}

// FIXME: we should do a fstat to get these values.
bool FileStream::readable() { return true; }

bool FileStream::writable() { return true; }

bool FileStream::seekable() { return true; }

ErrorOr<size_t> FileStream::read(void *buffer, size_t size)
{
    return __plugs__::file_read(_handle, buffer, size);
}

ErrorOr<size_t> FileStream::write(const void *buffer, size_t size)
{
    return __plugs__::file_write(_handle, buffer, size);
}

ErrorOr<byte> FileStream::read_byte()
{
    byte b;
    auto err = __plugs__::file_read(_handle, &b, 1);
    return ErrorOr<byte>(err, b);
}

Error FileStream::write_byte(byte byte)
{
    auto err = __plugs__::file_write(_handle, &byte, 1);
    return err.error();
}

ErrorOr<size_t> FileStream::seek(Stream::Offset offset, SeekOrigine origine)
{
    return __plugs__::file_seek(_handle, offset, origine);
}

ErrorOr<size_t> FileStream::tell()
{
    return __plugs__::file_tell(_handle);
}

void FileStream::flush()
{
    // TODO
}

} // namespace libsystem
