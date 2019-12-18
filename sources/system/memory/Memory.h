#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libruntime/RefPtr.h>
#include <libruntime/Types.h>

#include "system/memory/Region.h"

namespace system::memory
{

Region alloc_region(size_t how_many_pages);

void free_region(Region region);

bool is_bootstraped();

} // namespace system::memory