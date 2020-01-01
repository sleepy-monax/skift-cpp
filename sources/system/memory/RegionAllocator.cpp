
#include "system/memory/RegionAllocator.h"

namespace system::memory
{

Region RegionAllocator::alloc_region(int how_many_pages)
{
    Region result = _free_pool.take(how_many_pages);
    _used_pool.put(result);

    return result;
}

void RegionAllocator::free_region(Region region)
{
    _free_pool.put(region);
    _used_pool.take(region);
}

} // namespace system::memory
