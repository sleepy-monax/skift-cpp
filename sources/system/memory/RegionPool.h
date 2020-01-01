#pragma once

#include <libruntime/OwnPtr.h>
#include <libruntime/Vector.h>
#include <libsystem/Assert.h>

#include "system/memory/Region.h"

namespace system::memory
{

class RegionPool
{
private:
    size_t _quantity;
    libruntime::Vector<Region> _regions;

public:
    size_t quantity() { return _quantity; }

    RegionPool() {}

    ~RegionPool() {}

    Region take(size_t how_many_pages);

    void take(Region region);

    void put(Region region);
};

} // namespace system::memory
