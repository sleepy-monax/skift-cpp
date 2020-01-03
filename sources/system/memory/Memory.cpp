/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libruntime/LinkedList.h>
#include <libsystem/Assert.h>
#include <libsystem/Logger.h>

#include "arch/Arch.h"
#include "system/System.h"
#include "system/memory/Memory.h"
#include "system/memory/Region.h"
#include "system/memory/RegionAllocator.h"

using namespace system;
using namespace libruntime;

namespace system::memory
{

static bool _bootstraped = false;
static Region _boostrap;
static RegionAllocator *_allocator;

bool is_bootstraped()
{
    return _bootstraped;
}

Region alloc_region(size_t how_many_pages)
{
    assert(how_many_pages > 0);

    if (!_bootstraped)
    {
        PANIC("MemoryManager not bootstraped!");
    }

    Region region = Region::empty();

    if (_boostrap.page_count() >= how_many_pages)
    {
        // Look like we have some space in the bootstrap :)
        region = _boostrap.take(how_many_pages);

        if (_boostrap.is_empty())
        {
            logger_info("The bootstrap is now empty.");
        }
    }
    else
    {
        region = _allocator->alloc_region(how_many_pages);
    }

    if (region.is_empty())
    {
        PANIC("Out of memory!");
    }

    return region;
}

void free_region(Region region)
{
    auto kernel_region = arch::get_kernel_region();

    if (region.is_overlaping_with(kernel_region))
    {
        // An half of the region is under the kernel.
        Region lower_half = region.half_under(kernel_region);

        if (!lower_half.is_empty())
        {
            free_region(lower_half);
        }

        // An another half of the region is over the kernel.
        Region upper_half = region.half_over(kernel_region);

        if (!upper_half.is_empty())
        {
            free_region(lower_half);
        }
    }
    else if (!_bootstraped)
    {
        logger_info("Bootstraping with {}", region);

        _boostrap = region;
        _bootstraped = true;

        _allocator = new RegionAllocator();
    }
    else
    {
        _allocator->free_region(region);
    }
}

} // namespace system::memory
