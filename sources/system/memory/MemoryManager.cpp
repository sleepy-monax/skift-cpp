#include <libsystem/Assert.h>
#include <system/memory/MemoryManager.h>

using namespace system;
using namespace libruntime;

namespace system::MemoryManager
{

static bool mm_bootstraped = false;

static uintptr_t mm_bootstrap_addr;

static size_t mm_bootstrap_size;

RefPtr<SharedMemoryObject> alloc_region(size_t size)
{
    __unused(size);

    assert_not_reached();
}

void free_region(uintptr_t addr, size_t size)
{
    if (!mm_bootstraped)
    {
        mm_bootstrap_addr = addr;
        mm_bootstrap_size = size;

        mm_bootstraped = true;
    }
    else
    {
        // TODO: add the region to the free list.
    }
}

} // namespace system::MemoryManager