#pragma once

#include <libruntime/Macros.h>
#include <libruntime/Types.h>

namespace x86
{

class __packed PageDirectoryEntry
{
private:
    bool _present : 1;
    bool _write : 1;
    bool _user : 1;
    bool _page_level_write_through : 1;
    bool _page_level_cache_disable : 1;
    bool _accessed : 1;
    bool _ignored1 : 1;
    bool _large_page : 1;
    uint32_t _ignored2 : 4;
    uint32_t _page_frame_number : 20;

public:
    PageDirectoryEntry() {}
    ~PageDirectoryEntry() {}
};

class PageDirectory
{
private:
    PageDirectoryEntry entries[1024];

public:
    PageDirectory() {}
    ~PageDirectory() {}
};

static_assert(sizeof(PageDirectoryEntry) == 4);
static_assert(sizeof(PageDirectory) == 4096);

} // namespace x86
