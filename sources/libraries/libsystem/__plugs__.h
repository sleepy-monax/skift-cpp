#pragma once

#include <libruntime/ErrorOr.h>
#include <libruntime/Types.h>
#include <libsystem/Stream.h>

extern "C" void __plug_init_libsystem();

namespace __plugs__
{

libruntime::ErrorOr<int> file_open(const char *path, size_t len, int flags);
void file_close(int handle);
libruntime::ErrorOr<size_t> file_read(int handle, void *buffer, size_t size);
libruntime::ErrorOr<size_t> file_write(int handle, const void *buffer, size_t size);
libruntime::ErrorOr<size_t> file_seek(int handle, size_t offset, libsystem::SeekOrigine origine);
libruntime::ErrorOr<size_t> file_tell(int handle);

} // namespace __plugs__
