#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libruntime/RefCounted.h>
#include <libruntime/Types.h>

#include "system/memory/MemoryRegion.h"

namespace system::memory
{

class MemoryObject : public libruntime::RefCounted<MemoryObject>
{
private:
    int _id;
    MemoryRegion _region;

public:
    int id() { return _id; }
    MemoryRegion region() { return _region; }

    MemoryObject(MemoryRegion region);
    ~MemoryObject();
};

} // namespace system::memory
