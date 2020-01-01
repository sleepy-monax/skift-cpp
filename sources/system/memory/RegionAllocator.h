#pragma once

#include "arch/Arch.h"
#include "system/memory/RegionPool.h"

namespace system::memory
{

class RegionAllocator
{
private:
    RegionPool _free_pool;
    RegionPool _used_pool;

public:
    size_t used() { return _used_pool.quantity() * arch::get_page_size(); }
    size_t free() { return _free_pool.quantity() * arch::get_page_size(); }
    size_t total() { return used() + total(); }

    RegionAllocator() {}

    ~RegionAllocator() {}

    Region alloc_region(int how_many_pages);

    void free_region(Region region);
};

} // namespace system::memory
