#include "system/memory/SharedMemoryObject.h"
#include "system/memory/MemoryManager.h"

using namespace system;

SharedMemoryObject::SharedMemoryObject(uintptr_t addr, size_t size)
    : _addr(addr),
      _size(size)
{
}

SharedMemoryObject::~SharedMemoryObject()
{
    MemoryManager::free_region(_addr, _size);
}
