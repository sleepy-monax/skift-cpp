#include <libsystem/__plugs__.h>

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

namespace __plugs__
{

//FIXME: convert errno to Error

libruntime::ErrorOr<int> file_open(const char *path, size_t len, libsystem::FileStreamFlags flags)
{
    __unused(flags);

    char *buffer = reinterpret_cast<char *>(malloc(len + 1));

    strncpy(buffer, path, len);

    buffer[len] = '\0';

    int handle = open(buffer, O_CREAT);

    free(buffer);

    return handle;
}

void file_close(int handle)
{
    close(handle);
}

libruntime::ErrorOr<size_t> file_read(int handle, void *buffer, size_t size)
{
    return read(handle, buffer, size);
}

libruntime::ErrorOr<size_t> file_write(int handle, const void *buffer, size_t size)
{
    return write(handle, buffer, size);
}

int SeekOrigine_to_whence(libsystem::SeekOrigine origine)
{
    switch (origine)
    {
    case libsystem::SeekOrigine::BEGIN:
        return SEEK_SET;
    case libsystem::SeekOrigine::END:
        return SEEK_END;
    case libsystem::SeekOrigine::CURRENT:
        return SEEK_CUR;
    default:
        return SEEK_SET;
    }
}

libruntime::ErrorOr<size_t> file_seek(int handle, libsystem::Stream::Offset offset, libsystem::SeekOrigine origine)
{
    return lseek(handle, offset, SeekOrigine_to_whence(origine));
}

libruntime::ErrorOr<size_t> file_tell(int handle)
{
    return file_seek(handle, 0, libsystem::CURRENT);
}

} // namespace __plugs__