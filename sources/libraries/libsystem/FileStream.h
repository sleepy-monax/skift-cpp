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

class FileStream
{
private:
    int _fd;
    FileStreamFlags flags;

public:
    static ErrorOr<RefPtr<FileStream>>
    open(const char *path, FileStreamFlags flags);

    FileStream(int fd);

    ~FileStream();

    virtual bool readable();

    virtual bool writable();

    virtual bool seekable();

    virtual ErrorOr<size_t> read(void *buffer, size_t size);

    virtual ErrorOr<size_t> write(const void *buffer, size_t size);

    virtual ErrorOr<byte> read_byte();

    virtual Error write_byte(byte byte);

    virtual ErrorOr<size_t> seek(size_t offset, SeekOrigine origine);

    virtual ErrorOr<size_t> tell();

    virtual void flush();
};

} // namespace libsystem
