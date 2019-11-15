#pragma once

#include <libsystem/Stream.h>
#include <libsystem/__plugs__.h>
#include <libruntime/RefPtr.h>

namespace libsystem
{

enum FileStreamFlags
{
    READ = 1 << 0,
    WRITE = 2 << 1,
};

class FileStream : public Stream
{
private:
    int _fd;
    FileStreamFlags flags;

public:
    static ErrorOr<RefPtr<FileStream>>
    open(const char *path, FileStreamFlags flags);

    FileStream(int fd);

    ~FileStream();

    bool readable();

    bool writable();

    bool seekable();

    ErrorOr<size_t> read(void *buffer, size_t size);

    ErrorOr<size_t> write(const void *buffer, size_t size);

    ErrorOr<byte> read_byte();

    Error write_byte(byte byte);

    ErrorOr<size_t> seek(Stream::Offset offset, SeekOrigine origine);

    ErrorOr<size_t> tell();

    void flush();
};

} // namespace libsystem
