#pragma once

#include <libruntime/OwnPtr.h>
#include <libruntime/Vector.h>
#include <libsystem/Assert.h>

#include "system/memory/MemoryRegion.h"

namespace system::memory
{

class MemoryPool
{
private:
    size_t _quantity;
    libruntime::Vector<MemoryRegion> _regions;

public:
    size_t quantity() { return _quantity; }

    MemoryPool() {}

    ~MemoryPool() {}

    MemoryRegion take(size_t how_many_pages);

    void take(MemoryRegion region);

    void put(MemoryRegion region);
};

} // namespace system::memory
