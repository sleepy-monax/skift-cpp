#pragma once

#include "arch/Arch.h"
#include "system/memory/MemoryPool.h"

namespace system::memory
{

class MemoryRegionAllocator
{
private:
    MemoryPool _free_pool;
    MemoryPool _used_pool;

public:
    size_t used() { return _used_pool.quantity() * arch::get_page_size(); }
    size_t free() { return _free_pool.quantity() * arch::get_page_size(); }
    size_t total() { return used() + free(); }

    MemoryRegionAllocator() {}

    ~MemoryRegionAllocator() {}

    MemoryRegion alloc_region(int how_many_pages);

    void free_region(MemoryRegion region);
};

} // namespace system::memory
