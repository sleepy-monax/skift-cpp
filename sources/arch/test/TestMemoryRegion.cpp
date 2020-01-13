/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <assert.h>

#include "arch/Arch.h"
#include "system/memory/MemoryRegion.cpp"
#include "system/memory/MemoryRegion.h"

using system::memory::MemoryRegion;

size_t arch::get_page_size()
{
    return 4096;
}

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    MemoryRegion region = MemoryRegion::from_aligned_address(4096, 8192);

    assert(region.base_page() == 1);
    assert(region.page_count() == 2);
    assert(region.base_address() == 4096);
    assert(region.size() == 8192);

    region = MemoryRegion::from_non_aligned_address(4096, 8192);

    assert(region.base_page() == 1);
    assert(region.page_count() == 2);
    assert(region.base_address() == 4096);
    assert(region.size() == 8192);

    region = MemoryRegion::from_non_aligned_address(4096 + 1024, 8192);

    assert(region.base_page() == 2);
    assert(region.page_count() == 1);
    assert(region.base_address() == 8192);
    assert(region.size() == 4096);

    region = MemoryRegion::from_non_aligned_address(4096 - 1024, 8192);

    assert(region.base_page() == 1);
    assert(region.page_count() == 1);
    assert(region.base_address() == 4096);
    assert(region.size() == 4096);

    return 0;
}
