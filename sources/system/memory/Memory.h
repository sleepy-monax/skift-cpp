#pragma once

#include <libruntime/Types.h>
#include <libruntime/RefPtr.h>

#include "system/memory/Region.h"

namespace system::memory
{

Region alloc_region(size_t how_many_pages);

void free_region(Region region);

} // namespace system::memory