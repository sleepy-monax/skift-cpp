#pragma once
#include "system/memory/Region.h"
#include <libsystem/Formattable.h>

#include "thirdparty/multiboot/multiboot.h"

namespace x86
{

class MemoryMapEntry : public libsystem::Formattable
{
private:
    uintptr_t _addr;
    size_t _size;
    int _type;

public:
    MemoryMapEntry(uintptr_t addr, size_t size, int type)
        : _addr(addr), _size(size), _type(type) {}

    bool is_available()
    {
        return _type == MULTIBOOT_MEMORY_AVAILABLE;
    }

    bool is_reserved()
    {
        return _type == MULTIBOOT_MEMORY_RESERVED;
    }

    bool is_bad()
    {
        return _type == MULTIBOOT_MEMORY_BADRAM;
    }

    uintptr_t address()
    {
        return _addr;
    }

    size_t size()
    {
        return _size;
    }

    system::memory::Region region()
    {
        return system::memory::Region::from_non_aligned_address(_addr, _size);
    }

    libruntime::ErrorOr<size_t> format(libsystem::Stream &stream, libsystem::FormatInfo &info)
    {
        __unused(info);

        const char *multiboot_memory_type_name[] = {
            "INVALID",
            "AVAILABLE",
            "RESERVED",
            "ACPI_RECLAIMABLE",
            "NVS",
            "BADRAM",
        };

        return libsystem::format(
            stream,
            "MemoryMapEntry({}, {})",
            region(),
            multiboot_memory_type_name[_type]);
    }
};

class Multiboot
{
private:
    multiboot_info_t *_info;
    u32 _magic;

public:
    Multiboot(u32 magic, multiboot_info_t *info) : _info(info), _magic(magic) {}

    ~Multiboot() {}

    bool is_valid()
    {
        return _magic == MULTIBOOT_BOOTLOADER_MAGIC;
    }

    const char *bootloader()
    {
        return (const char *)_info->boot_loader_name;
    }

    template <typename Callback>
    void with_memory_map(Callback callback)
    {
        for (multiboot_memory_map_t *mmap = (multiboot_memory_map_t *)_info->mmap_addr;
             (u32)mmap < _info->mmap_addr + _info->mmap_length;
             mmap = (multiboot_memory_map_t *)((u32)mmap + mmap->size + sizeof(mmap->size)))
        {

            if (callback(MemoryMapEntry((uintptr_t)mmap->addr, (size_t)mmap->len, mmap->type)) == libruntime::Iteration::STOP)
            {
                return;
            }
        }
    }
};

}; // namespace x86
