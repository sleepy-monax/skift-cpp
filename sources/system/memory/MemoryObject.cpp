/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include "system/memory/MemoryObject.h"
#include "system/memory/Memory.h"

namespace system::memory
{

static volatile int _memory_object_id_counter = 0;

MemoryObject::MemoryObject(MemoryRegion region) : _id(__sync_add_and_fetch(&_memory_object_id_counter, 1)),
                                                  _region(region) {}

MemoryObject::~MemoryObject()
{
    free_region(_region);
}

} // namespace system::memory
