#pragma once

#include <libruntime/RefCounted.h>
#include <libruntime/Types.h>

namespace system
{

class SharedMemoryObject : public libruntime::RefCounted<SharedMemoryObject>
{
private:
    uintptr_t _addr;
    size_t _size;

public:
    SharedMemoryObject(uintptr_t addr, size_t size);
    ~SharedMemoryObject();

    uintptr_t address() { return _addr; }
    size_t size() { return _size; }
};

} // namespace system
