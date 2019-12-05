#pragma once

#include <libruntime/Types.h>
#include <libc/string.h>

#include "system/memory/Memory.h"

namespace system::tasking
{

class Stack
{
private:
    uintptr_t _ptr;
    memory::Region _region;

public:
    Stack(size_t how_many_pages)
    {
        _region = memory::alloc_region(how_many_pages);
    }

    ~Stack()
    {
        memory::free_region(_region);
        _ptr = _region.end_address();
    }

    template <typename T>
    uintptr_t push(T value)
    {
        return push<T>(&value);
    }

    template <typename T>
    uintptr_t push(T *object)
    {
        _ptr -= sizeof(T);
        libc::memcpy(reinterpret_cast<void *>(_ptr), object, sizeof(T));

        return _ptr;
    }

    uintptr_t get_pointer(void)
    {
        return _ptr;
    }

    void set_pointer(uintptr_t ptr)
    {
        _ptr = ptr;
    }
};

} // namespace system::tasking
