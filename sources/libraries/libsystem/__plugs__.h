#pragma once

#include <libruntime/ErrorOr.h>
#include <libruntime/Types.h>
#include <libsystem/Stream.h>
#include <libsystem/FileStream.h>

extern "C" void __plug_init_libsystem();

namespace __plugs__
{

/* --- I/O Stream ----------------------------------------------------------- */

libruntime::ErrorOr<int> file_open(const char *path, size_t len, libsystem::FileStreamFlags flags);

void file_close(int handle);

libruntime::ErrorOr<size_t> file_read(int handle, void *buffer, size_t size);

libruntime::ErrorOr<size_t> file_write(int handle, const void *buffer, size_t size);

libruntime::ErrorOr<size_t> file_seek(int handle, libsystem::Stream::Offset offset, libsystem::SeekOrigine origine);

libruntime::ErrorOr<size_t> file_tell(int handle);

/* --- Memory Managment ----------------------------------------------------- */

void memory_lock();

void memory_unlock();

libruntime::ErrorOr<uintptr_t> memory_alloc(size_t how_many_pages);

void memory_free(uintptr_t addr, size_t how_many_pages);

/* --- Assert --------------------------------------------------------------- */

void assert_failled() __noreturn;

void assert_not_reached_reached() __noreturn;

} // namespace __plugs__
