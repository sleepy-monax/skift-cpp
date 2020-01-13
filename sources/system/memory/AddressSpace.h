#pragma once

#include <libruntime/LinkedList.h>
#include <libruntime/RefPtr.h>

#include "system/memory/MemoryObject.h"

namespace system::memory
{

class AddressSpace
{
private:
    libruntime::LinkedList<libruntime::RefPtr<MemoryObject>> _objects;

public:
    AddressSpace() {}
    ~AddressSpace() {}
};

} // namespace system::memory
