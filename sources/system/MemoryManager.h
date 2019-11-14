#pragma once

#include <libmath/MinMax.h>
#include <libruntime/Assert.h>
#include <libruntime/Types.h>
#include <libruntime/RefCounted.h>

namespace system
{

class FreeMemoryRegion : public libruntime::RefCounted<FreeMemoryRegion>
{
private:
    uintptr_t _addr;
    size_t _size;

public:
    FreeMemoryRegion(uintptr_t addr, size_t size)
    {
        _addr = addr;
        _size = size;
    }

    bool is_contiguous_with(FreeMemoryRegion &other)
    {
        return (_addr + _size) == other._addr || (other._addr + other._size) == _addr;
    }

    void merge_with(FreeMemoryRegion &other)
    {
        assert(is_contiguous_with(other));

        _addr = libmath::min(_addr, other._addr);
        _size = _size + other._size;
    }
};

class MemoryManager
{
    void allocphys();
};

} // namespace system
