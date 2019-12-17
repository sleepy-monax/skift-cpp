/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include "system/memory/SharedObject.h"
#include "system/memory/Memory.h"

using namespace system::memory;

SharedObject::SharedObject(Region region) : _region(region) {}

SharedObject::~SharedObject()
{
    free_region(_region);
}
