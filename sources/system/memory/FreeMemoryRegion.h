#pragma once

#include <libmath/MinMax.h>
#include <libsystem/Assert.h>
#include <libruntime/Types.h>
#include <libruntime/RefCounted.h>

#include "arch/Arch.h"

namespace system::MemoryManager
{

class FreeMemoryRegion
{
private:
    uintptr_t _base_page;
    size_t _page_count;

public:
    FreeMemoryRegion(uintptr_t base_page, size_t page_count)
    {
        _base_page = base_page;
        _page_count = page_count;
    }

    bool is_contiguous_with(FreeMemoryRegion &other)
    {
        return (_base_page + _page_count) == other._base_page || (other._base_page + other._page_count) == _base_page;
    }

    void merge_with(FreeMemoryRegion &other)
    {
        assert(is_contiguous_with(other));

        _base_page = libmath::min(_base_page, other._base_page);
        _page_count = _page_count + other._page_count;
    }

    bool is_empty()
    {
        return _page_count == 0;
    }

    size_t page_count()
    {
        return _page_count;
    }

    uintptr_t take(size_t how_many_pages)
    {
        assert(_page_count >= how_many_pages);

        _page_count -= how_many_pages;

        return (_base_page + _page_count);
    }

    bool operator==(FreeMemoryRegion &other)
    {
        return _base_page == other._base_page &&
               _page_count == other._page_count;
    }
};

} // namespace system::MemoryManager
