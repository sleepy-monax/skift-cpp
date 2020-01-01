#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libruntime/Types.h>
#include <libsystem/Format.h>
#include <libsystem/Formattable.h>

namespace system::memory
{

class Region : public libsystem::Formattable
{
private:
    uintptr_t _base_page;
    size_t _page_count;

    Region(uintptr_t base_page, size_t page_count);

public:
    Region();

    virtual ~Region(){};

    static Region empty();

    static Region from_non_aligned_address(uintptr_t addr, size_t size);

    static Region create_around_non_aligned_address(uintptr_t addr, size_t size);

    static Region from_aligned_address(uintptr_t addr, size_t size);

    static Region from_page(uintptr_t page, size_t count);

    void merge(Region &other);

    Region take(size_t how_many_pages);

    Region half_under(Region split);

    Region half_over(Region split);

    bool is_overlaping_with(Region other);

    bool is_contiguous_with(Region other);

    bool is_empty();

    uintptr_t base_address();

    uintptr_t end_address();

    size_t size();

    uintptr_t base_page();

    uintptr_t end_page();

    size_t page_count();

    bool operator==(Region &other);

    libruntime::ErrorOr<size_t> format(libsystem::Stream &stream, libsystem::FormatInfo &info)
    {
        __unused(info);

        return libsystem::format(stream, "Region[{#x}-{#x}]", base_address(), end_address() - 1);
    }
};

} // namespace system::memory
