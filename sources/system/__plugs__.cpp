#include <libsystem/__plugs__.h>
#include <libsystem/Logger.h>

#include "system/memory/MemoryManager.h"
#include "system/System.h"

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

/* --- Memory --------------------------------------------------------------- */

void memory_lock() {}

void memory_unlock() {}

libruntime::ErrorOr<uintptr_t> memory_alloc(size_t how_many_pages)
{
    logger_debug("Allocating {} pages.", how_many_pages);
    return libruntime::ErrorOr<uintptr_t>(system::MemoryManager::alloc_region(how_many_pages));
}

void memory_free(uintptr_t addr, size_t how_many_pages)
{
    logger_debug("Freeing {} pages at {x}.", how_many_pages, addr);
    system::MemoryManager::free_region(addr, how_many_pages);
}

/* --- Assert --------------------------------------------------------------- */

void assert_failled()
{
    system::PANIC("Assert() failled!");
}

void assert_not_reached_reached()
{
    system::PANIC("Assert_not_reached() reached!");
}

} // namespace __plugs__