#include <libsystem/__plugs__.h>

namespace __plugs__
{

libruntime::ErrorOr<int> file_open(const char *path, size_t len, libsystem::FileStreamFlags flags)
{
    __unused(path);
    __unused(len);
    __unused(flags);

    return libruntime::Error::NOT_IMPLEMENTED;
}

void file_close(int handle)
{
    __unused(handle);
}

libruntime::ErrorOr<size_t> file_read(int handle, void *buffer, size_t size)
{
    __unused(handle);
    __unused(buffer);
    __unused(size);

    return libruntime::Error::NOT_IMPLEMENTED;
}

libruntime::ErrorOr<size_t> file_write(int handle, const void *buffer, size_t size)
{
    __unused(handle);
    __unused(buffer);
    __unused(size);

    return libruntime::Error::NOT_IMPLEMENTED;
}

libruntime::ErrorOr<size_t> file_seek(int handle, libsystem::Stream::Offset offset, libsystem::SeekOrigine origine)
{
    __unused(handle);
    __unused(offset);
    __unused(origine);

    return libruntime::Error::NOT_IMPLEMENTED;
}

libruntime::ErrorOr<size_t> file_tell(int handle)
{
    __unused(handle);

    return libruntime::Error::NOT_IMPLEMENTED;
}

} // namespace __plugs__