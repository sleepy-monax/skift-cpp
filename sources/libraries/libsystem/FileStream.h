#pragma once

#include <libruntime/RefPtr.h>
#include <libsystem/Stream.h>

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
    int _handle;
    FileStreamFlags _flags;

public:
    static libruntime::ErrorOr<libruntime::RefPtr<FileStream>>
    open(const char *path, FileStreamFlags flags);

    FileStream(int handle, FileStreamFlags flags);

    ~FileStream();

    bool readable();

    bool writable();

    bool seekable();

    libruntime::ErrorOr<size_t> read(void *buffer, size_t size);

    libruntime::ErrorOr<size_t> write(const void *buffer, size_t size);

    libruntime::ErrorOr<byte> read_byte();

    libruntime::Error write_byte(byte byte);

    libruntime::ErrorOr<size_t> seek(Stream::Offset offset, SeekOrigine origine);

    libruntime::ErrorOr<size_t> tell();

    void flush();
};

} // namespace libsystem
