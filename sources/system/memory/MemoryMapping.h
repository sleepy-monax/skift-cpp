#pragma once

#include "system/memory/MemoryObject.h"
#include <libruntime/RefPtr.h>

namespace system::memory
{

class MemoryMapping
{
private:
    MemoryRegion _virtual_region;
    libruntime::RefPtr<MemoryObject> _object;

public:
    MemoryMapping(MemoryRegion virtual_region, MemoryObject object) {}
    ~MemoryMapping() {}
};

} // namespace system::memory
