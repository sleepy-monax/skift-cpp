#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include "system/System.h"
#include "system/scheduling/Blocker.h"

namespace system::scheduling
{

class BlockerSleep : public Blocker
{
private:
    uint32_t _wakeup_tick;

public:
    BlockerSleep(uint32_t how_long)
    {
        _wakeup_tick = system::get_tick() + how_long;
    }

    ~BlockerSleep() {}

    bool should_unblock()
    {
        return _wakeup_tick <= system::get_tick();
    }

    void unblock()
    {
        // do nothing...
    }
};

} // namespace system::scheduling
