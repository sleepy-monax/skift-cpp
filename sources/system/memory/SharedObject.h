#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

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
