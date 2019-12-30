#pragma once

#include <libruntime/Macros.h>
#include <libruntime/Types.h>

namespace x86
{

class __packed PageTableEntry
{
private:
    bool _present : 1;
    bool _write : 1;
    bool _user : 1;
    bool _page_level_write_through : 1;
    bool _page_level_cache_disable : 1;
    bool _accessed : 1;
    bool _dirty : 1;
    bool _pat : 1;
    uint32_t _ignored : 4;
    uint32_t _page_frame_number : 20;

public:
    PageTableEntry() {}
    ~PageTableEntry() {}
};

class PageTable
{
private:
    PageTableEntry _entries[1024];

public:
    PageTable(/* args */) {}
    ~PageTable() {}
};

static_assert(sizeof(PageTableEntry) == 4);
static_assert(sizeof(PageTable) == 4096);

} // namespace x86
