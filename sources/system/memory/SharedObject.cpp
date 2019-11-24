#include "system/memory/Memory.h"
#include "system/memory/SharedObject.h"

using namespace system::memory;

SharedObject::SharedObject(Region region) : _region(region) {}

SharedObject::~SharedObject()
{
    free_region(_region);
}
