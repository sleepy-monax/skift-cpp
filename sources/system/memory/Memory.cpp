#include <libruntime/LinkedList.h>
#include <libsystem/Assert.h>
#include <libsystem/Logger.h>

#include "arch/Arch.h"
#include "system/System.h"
#include "system/memory/Region.h"
#include "system/memory/Memory.h"

using namespace system;
using namespace libruntime;

namespace system::memory
{

static bool bootstraped = false;
static Region bootstarp;

static LinkedList<Region> *free_list;

static Region take_from_free_list(size_t how_many_pages)
{
    Region region = Region::empty();

    free_list->iterate([&](auto &free_region) {
        if (free_region.page_count() >= how_many_pages)
        {
            region = free_region.take(how_many_pages);

            if (free_region.is_empty())
            {
                free_list->remove_all(free_region);
            }

            return Iteration::STOP;
        }
        else
        {
            return Iteration::CONTINUE;
        }
    });

    return region;
}

Region alloc_region(size_t how_many_pages)
{
    assert(how_many_pages > 0);

    logger_trace("Allocating {} pages of memory.", how_many_pages);

    if (!bootstraped)
    {
        PANIC("MemoryManager not bootstraped!");
    }

    Region region = Region::empty();

    if (bootstarp.page_count() >= how_many_pages)
    {
        // Look like we have some space in the bootstrap :)
        region = bootstarp.take(how_many_pages);

        if (bootstarp.is_empty())
        {
            logger_info("The bootstrap is now empty.");
        }
    }
    else
    {
        region = take_from_free_list(how_many_pages);
    }

    if (region.is_empty())
    {
        PANIC("Out of memory!");
    }

    logger_trace("{} allocated.", region);

    return region;
}

void free_region(Region region)
{
    logger_trace("Free'ing {}", region);

    auto kernel_region = arch::get_kernel_region();

    if (region.is_overlaping_with(kernel_region))
    {
        logger_trace("{} is intersecting with the kernel {}, spliting...", region, arch::get_kernel_region());

        // An half of the region is under the kernel.
        if (region.base_page() < kernel_region.base_page())
        {
            Region lower_half = Region::from_page(
                region.base_page(),
                kernel_region.base_page() - region.base_page());

            logger_trace("The lower half is {}", lower_half);

            free_region(lower_half);
        }

        // An another half of the region is over the kernel.
        if (region.end_page() > kernel_region.end_page())
        {
            Region upper_half = Region::from_page(
                kernel_region.end_page(),
                region.end_page() - kernel_region.end_page());

            logger_trace("The upper half is {}", upper_half);

            free_region(upper_half);
        }
    }
    else if (!bootstraped)
    {
        logger_info("Bootstraping with {}", region);

        bootstarp = region;
        bootstraped = true;

        free_list = new LinkedList<Region>();
    }
    else
    {
        // FIXME: we should also merge if we are filling an hole.
        bool has_been_merge_with_other_region = false;

        free_list->iterate([&](auto &other) {
            if (other.is_contiguous_with(region))
            {
                other.merge(region);

                has_been_merge_with_other_region = true;

                return Iteration::STOP;
            }

            return Iteration::CONTINUE;
        });

        if (!has_been_merge_with_other_region)
        {
            free_list->push_back(region);
        }
    }
}

} // namespace system::memory
