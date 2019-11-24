#pragma once

#include <libruntime/RefCounted.h>
#include <libruntime/Types.h>

#include "system/memory/Region.h"

namespace system::memory
{

class SharedObject : public libruntime::RefCounted<SharedObject>
{
private:
    Region _region;

public:
    SharedObject(Region region);
    ~SharedObject();

    Region region() { return _region; }
};

} // namespace system::memory
