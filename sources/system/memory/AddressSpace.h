#pragma once

#include <libruntime/LinkedList.h>
#include <libruntime/RefPtr.h>

#include "system/memory/SharedObject.h"

namespace system::memory
{

class AddressSpace
{
private:
    libruntime::LinkedList<libruntime::RefPtr<SharedObject>> _objects;

public:
    AddressSpace() {}
    ~AddressSpace() {}
};

} // namespace system::memory
