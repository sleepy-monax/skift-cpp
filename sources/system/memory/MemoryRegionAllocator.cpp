
#include "system/memory/MemoryRegionAllocator.h"

namespace system::memory
{

MemoryRegion MemoryRegionAllocator::alloc_region(int how_many_pages)
{
    MemoryRegion result = _free_pool.take(how_many_pages);
    _used_pool.put(result);

    return result;
}

void MemoryRegionAllocator::free_region(MemoryRegion region)
{
    _free_pool.put(region);
    _used_pool.take(region);
}

} // namespace system::memory
