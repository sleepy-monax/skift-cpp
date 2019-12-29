/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libsystem/__plugs__.h>
#include <libsystem/Stdio.h>
#include <libsystem/FileStream.h>
#include <libruntime/SpinLock.h>

libruntime::RefPtr<libsystem::Stream> libsystem::stdin;
libruntime::RefPtr<libsystem::Stream> libsystem::stdout;
libruntime::RefPtr<libsystem::Stream> libsystem::stderr;
libruntime::RefPtr<libsystem::Stream> libsystem::stdlog;

namespace __plugs__
{

extern "C" void __plug_init(void)
{
    libsystem::stdin = libruntime::make<libsystem::FileStream>(0, libsystem::FileStreamFlags::READ);
    libsystem::stdout = libruntime::make<libsystem::FileStream>(1, libsystem::FileStreamFlags::WRITE);
    libsystem::stderr = libruntime::make<libsystem::FileStream>(2, libsystem::FileStreamFlags::WRITE);
    libsystem::stdlog = libruntime::make<libsystem::FileStream>(3, libsystem::FileStreamFlags::WRITE);
}

extern "C" void __plug_fini(void)
{
    libsystem::stdout->flush();
    libsystem::stderr->flush();
    libsystem::stdlog->flush();
}

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

/* --- Memory --------------------------------------------------------------- */

size_t get_page_size()
{
    return 4096; //FIXME: ask the kernel.
}

libruntime::SpinLock _memory_lock;

bool memory_is_lock()
{
    return _memory_lock.is_acquired();
}

void memory_lock()
{
    _memory_lock.acquire();
}

void memory_unlock()
{
    _memory_lock.release();
}

libruntime::ErrorOr<uintptr_t> memory_alloc(size_t how_many_pages)
{
    __unused(how_many_pages);

    return libruntime::Error::NOT_IMPLEMENTED;
}

void memory_free(uintptr_t addr, size_t how_many_pages)
{
    __unused(addr);
    __unused(how_many_pages);
}

/* --- Assert --------------------------------------------------------------- */

void assert_failled()
{
    for (;;)
    {
        /* code */
    }
}

void assert_not_reached_reached()
{
    for (;;)
    {
        /* code */
    }
}

} // namespace __plugs__