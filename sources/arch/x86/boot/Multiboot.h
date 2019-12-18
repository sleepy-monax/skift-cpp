#pragma once

#include <libsystem/Formattable.h>

#include "libruntime/Iteration.h"
#include "system/memory/Region.h"
#include "thirdparty/multiboot/multiboot2.h"

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
        return _type == MULTIBOOT2_MEMORY_AVAILABLE;
    }

    bool is_reserved()
    {
        return _type == MULTIBOOT2_MEMORY_RESERVED;
    }

    bool is_bad()
    {
        return _type == MULTIBOOT2_MEMORY_BADRAM;
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
    u32 _magic;
    uintptr_t _address;
    size_t _size;

public:
    Multiboot(u32 magic, uintptr_t address)
    {
        _magic = magic;

        if (is_valid())
        {
            _size = *reinterpret_cast<size_t *>(address);
            _address = address + 8;
        }
    }

    ~Multiboot() {}

    bool is_valid()
    {
        return _magic == MULTIBOOT2_BOOTLOADER_MAGIC;
    }

    template <typename Callback>
    void for_each_tags(Callback callback)
    {
        multiboot2_tag *current_tag = reinterpret_cast<multiboot2_tag *>(_address);

        while (current_tag->type != MULTIBOOT2_TAG_TYPE_END)
        {
            if (callback(current_tag) == libruntime::Iteration::STOP)
            {
                return;
            }

            current_tag = reinterpret_cast<multiboot2_tag *>(__align_up(reinterpret_cast<uintptr_t>(current_tag) + current_tag->size, 8));
        }
    }

    template <typename T>
    T *find_tag(multiboot2_uint32_t type)
    {
        T *result = NULL;

        for_each_tags([&](multiboot2_tag *tag) {
            if (tag->type == type)
            {
                result = reinterpret_cast<T *>(tag);

                return libruntime::Iteration::STOP;
            }
            else
            {
                return libruntime::Iteration::CONTINUE;
            }
        });

        return result;
    }

    const char *bootloader()
    {
        auto *tag = find_tag<multiboot2_tag_string>(MULTIBOOT2_TAG_TYPE_BOOT_LOADER_NAME);

        if (tag)
        {
            return &tag->string[0];
        }
        else
        {
            return "unknown";
        }
    }

    template <typename Callback>
    void with_memory_map(Callback callback)
    {
        auto *tag = find_tag<multiboot2_tag_mmap>(MULTIBOOT2_TAG_TYPE_MMAP);

        if (tag)
        {
            for (multiboot2_memory_map_t *mmap = tag->entries;
                 (u8 *)mmap < (u8 *)tag + tag->size;
                 mmap = (multiboot2_memory_map_t *)((uintptr_t)mmap + tag->entry_size))
            {
                if (callback(MemoryMapEntry((uintptr_t)mmap->addr, (size_t)mmap->len, mmap->type)) == libruntime::Iteration::STOP)
                {
                    return;
                }
            }
        }
    }
};

}; // namespace x86
