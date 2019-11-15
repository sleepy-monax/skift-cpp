#include <libc/string.h>
#include <libsystem/__plugs__.h>
#include <libsystem/FileStream.h>

namespace libsystem
{

ErrorOr<RefPtr<FileStream>> FileStream::open(const char *path, FileStreamFlags flags)
{
    auto fd = __plugs__::file_open(path, libc::strlen(path), flags);

    if (fd == Error::SUCCEED)
    {
        return libruntime::make<FileStream>(fd.value());
    }
    else
    {
        return ErrorOr<RefPtr<FileStream>>(fd.error());
    }
}

FileStream::FileStream(int fd) : _fd(fd) {}

FileStream::~FileStream()
{
    __plugs__::file_close(_fd);
}

// FIXME: we should do a fstat to get these values.
bool FileStream::readable() { return true; }

bool FileStream::writable() { return true; }

bool FileStream::seekable() { return true; }

ErrorOr<size_t> FileStream::read(void *buffer, size_t size)
{
    return __plugs__::file_read(_fd, buffer, size);
}

ErrorOr<size_t> FileStream::write(const void *buffer, size_t size)
{
    return __plugs__::file_write(_fd, buffer, size);
}

ErrorOr<byte> FileStream::read_byte()
{
    byte b;
    auto err = __plugs__::file_read(_fd, &b, 1);
    return ErrorOr<byte>(err, b);
}

Error FileStream::write_byte(byte byte)
{
    auto err = __plugs__::file_write(_fd, &byte, 1);
    return err.error();
}

ErrorOr<size_t> FileStream::seek(Stream::Offset offset, SeekOrigine origine)
{
    return __plugs__::file_seek(_fd, offset, origine);
}

ErrorOr<size_t> FileStream::tell()
{
    return __plugs__::file_tell(_fd);
}

void FileStream::flush()
{
    // TODO
}

} // namespace libsystem
