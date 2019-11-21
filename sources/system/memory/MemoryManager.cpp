#include <libruntime/LinkedList.h>
#include <libsystem/Assert.h>
#include <libsystem/Logger.h>

#include "system/System.h"
#include "system/memory/FreeMemoryRegion.h"
#include "system/memory/MemoryManager.h"

using namespace system;
using namespace libruntime;

namespace system::MemoryManager
{

static bool bootstraped = false;
static uintptr_t bootstrap_base_page;
static size_t bootstrap_page_count;

static LinkedList<FreeMemoryRegion> *free_list;

static uintptr_t take_from_bootstrap(size_t how_many_pages)
{
    bootstrap_page_count -= how_many_pages;
    return (bootstrap_base_page + bootstrap_page_count);
}

static uintptr_t take_from_free_list(size_t how_many_pages)
{
    uintptr_t found = 0;

    free_list->iterate([&](auto &region) {
        if (region.page_count() >= how_many_pages)
        {
            found = region.take(how_many_pages);

            if (region.is_empty())
            {
                free_list->remove_all(region);
            }

            return Iteration::STOP;
        }
        else
        {
            return Iteration::CONTINUE;
        }
    });

    return found;
}

uintptr_t alloc_region(size_t how_many_pages)
{
    logger_trace("Allocating a memory region with a size of {}kib", how_many_pages * arch::get_page_size() / 1024);

    if (!bootstraped)
    {
        PANIC("MemoryManager not bootstraped!");
    }

    uintptr_t found = 0;

    if (bootstrap_page_count >= how_many_pages)
    {
        // Look like we have some space in the bootstrap :)
        found = take_from_bootstrap(how_many_pages);

        if (bootstrap_page_count == 0)
        {
            logger_info("The bootstrap is now empty.");
        }
    }
    else
    {
        found = take_from_free_list(how_many_pages);
    }

    if (found == 0)
    {
        PANIC("Out of memory!");
    }

    return found * arch::get_page_size();
}

static bool is_intersecting_with_kernel(uintptr_t addr, size_t size)
{
    return addr < arch::get_kernel_base_address() + arch::get_kernel_size() &&
           addr + size > arch::get_kernel_base_address();
}

void free_region(uintptr_t addr, size_t size)
{
    logger_trace("Free'ing memory region at {#x} with size of {} kib", addr, size / 1024);

    if (is_intersecting_with_kernel(addr, size))
    {
        logger_info("Memory region at {#x} is intersecting with the kernel, spliting...", addr);

        // FIXME: we should do something with this memory...
        return;
    }

    // We expect to get addresses and sizes page aligned.
    assert(addr % arch::get_page_size() == 0);

    size = size - size % arch::get_page_size();

    assert(size % arch::get_page_size() == 0);

    if (!bootstraped)
    {
        logger_info("Bootstraping with block at {#x} with size {}kib", addr, size / 1024);

        bootstrap_base_page = addr / arch::get_page_size();
        bootstrap_page_count = size / arch::get_page_size();

        bootstraped = true;

        free_list = new LinkedList<FreeMemoryRegion>();
    }
    else
    {
        free_list->push_back(FreeMemoryRegion(addr, size));
    }
}

} // namespace system::MemoryManager
