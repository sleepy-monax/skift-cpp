#pragma once

#include <libruntime/Types.h>
#include <libruntime/RefPtr.h>

#include "system/memory/SharedMemoryObject.h"

namespace system::MemoryManager
{

libruntime::RefPtr<SharedMemoryObject> alloc_region(size_t count);

void free_region(uintptr_t page, size_t count);

} // namespace system::MemoryManager