/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libmath/MinMax.h>
#include <libsystem/Assert.h>

#include "arch/Arch.h"
#include "system/memory/MemoryRegion.h"

namespace system::memory
{

MemoryRegion::MemoryRegion()
{
    _base_page = 0;
    _page_count = 0;
}

MemoryRegion::MemoryRegion(uintptr_t base_page, size_t page_count)
{
    _base_page = base_page;
    _page_count = page_count;
}

MemoryRegion MemoryRegion::empty()
{
    return MemoryRegion(0, 0);
}

MemoryRegion MemoryRegion::from_non_aligned_address(uintptr_t addr, size_t size)
{
    auto align = arch::get_page_size() - addr % arch::get_page_size();

    if (addr % arch::get_page_size() == 0)
    {
        align = 0;
    }

    addr += align;
    size -= align;

    size -= size % arch::get_page_size();

    return from_aligned_address(addr, size);
}

MemoryRegion MemoryRegion::create_around_non_aligned_address(uintptr_t addr, size_t size)
{
    auto align = addr % arch::get_page_size();

    addr -= align;
    size += align;

    size += arch::get_page_size() - size % arch::get_page_size();

    return from_aligned_address(addr, size);
}

MemoryRegion MemoryRegion::from_aligned_address(uintptr_t addr, size_t size)
{
    assert(addr % arch::get_page_size() == 0);
    assert(size % arch::get_page_size() == 0);

    return MemoryRegion(addr / arch::get_page_size(), size / arch::get_page_size());
}

MemoryRegion MemoryRegion::from_page(uintptr_t page, size_t count)
{
    return MemoryRegion(page, count);
}

void MemoryRegion::merge(MemoryRegion &other)
{
    assert(is_contiguous_with(other));

    _base_page = libmath::min(_base_page, other._base_page);
    _page_count = _page_count + other._page_count;
}

MemoryRegion MemoryRegion::take(size_t how_many_pages)
{
    assert(_page_count >= how_many_pages);

    _page_count -= how_many_pages;

    return MemoryRegion::from_page(_base_page + _page_count, how_many_pages);
}

MemoryRegion MemoryRegion::half_under(MemoryRegion split)
{
    if (is_overlaping_with(split) &&
        base_page() < split.base_page())
    {
        return MemoryRegion::from_page(
            base_page(),
            split.base_page() - base_page());
    }
    else
    {
        return MemoryRegion::empty();
    }
}

MemoryRegion MemoryRegion::half_over(MemoryRegion split)
{
    if (is_overlaping_with(split) &&
        end_page() > split.end_page())
    {
        return MemoryRegion::from_page(
            split.end_page(),
            end_page() - split.end_page());
    }
    else
    {
        return MemoryRegion::empty();
    }
}

bool MemoryRegion::is_overlaping_with(MemoryRegion other)
{
    return _base_page < other.base_page() + other.page_count() &&
           _base_page + _page_count > other.base_page();
}

bool MemoryRegion::is_contiguous_with(MemoryRegion other)
{
    return (_base_page + _page_count) == other._base_page || (other._base_page + other._page_count) == _base_page;
}

bool MemoryRegion::is_empty() { return _page_count == 0; }

uintptr_t MemoryRegion::base_address() { return _base_page * arch::get_page_size(); }

uintptr_t MemoryRegion::end_address() { return base_address() + size(); }

size_t MemoryRegion::size() { return _page_count * arch::get_page_size(); }

uintptr_t MemoryRegion::base_page() { return _base_page; }

uintptr_t MemoryRegion::end_page() { return _base_page + _page_count; }

size_t MemoryRegion::page_count() { return _page_count; }

bool MemoryRegion::operator==(MemoryRegion &other)
{
    return _base_page == other._base_page &&
           _page_count == other._page_count;
}

} // namespace system::memory
