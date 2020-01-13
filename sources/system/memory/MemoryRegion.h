#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libruntime/Types.h>
#include <libsystem/Format.h>
#include <libsystem/Formattable.h>

namespace system::memory
{

class MemoryRegion : public libsystem::Formattable
{
private:
    uintptr_t _base_page;
    size_t _page_count;

    MemoryRegion(uintptr_t base_page, size_t page_count);

public:
    MemoryRegion();

    virtual ~MemoryRegion(){};

    static MemoryRegion empty();

    static MemoryRegion from_non_aligned_address(uintptr_t addr, size_t size);

    static MemoryRegion create_around_non_aligned_address(uintptr_t addr, size_t size);

    static MemoryRegion from_aligned_address(uintptr_t addr, size_t size);

    static MemoryRegion from_page(uintptr_t page, size_t count);

    void merge(MemoryRegion &other);

    MemoryRegion take(size_t how_many_pages);

    MemoryRegion half_under(MemoryRegion split);

    MemoryRegion half_over(MemoryRegion split);

    bool is_overlaping_with(MemoryRegion other);

    bool is_contiguous_with(MemoryRegion other);

    bool is_empty();

    uintptr_t base_address();

    uintptr_t end_address();

    size_t size();

    uintptr_t base_page();

    uintptr_t end_page();

    size_t page_count();

    bool operator==(MemoryRegion &other);

    libruntime::ErrorOr<size_t> format(libsystem::Stream &stream, libsystem::FormatInfo &info)
    {
        __unused(info);

        return libsystem::format(stream, "MemoryRegion[{#x}-{#x}]", base_address(), end_address() - 1);
    }
};

} // namespace system::memory
