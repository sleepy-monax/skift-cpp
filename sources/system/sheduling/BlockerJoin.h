#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libruntime/RefPtr.h>

#include "system/sheduling/Blocker.h"
#include "system/tasking/Thread.h"

namespace system::sheduling
{

class BlockerJoin : public Blocker
{
private:
    libruntime::RefPtr<system::tasking::Thread> _joined_thread;

public:
    BlockerJoin(libruntime::RefPtr<system::tasking::Thread> joinded_thread) : _joined_thread(joinded_thread) {}

    ~BlockerJoin() {}

    bool should_unblock()
    {
        return _joined_thread->state() == system::tasking::ThreadState::STOPPED;
    }

    void unblock()
    {
        system::tasking::Thread::cleanup(_joined_thread);
        _joined_thread.clear();
    }
};

} // namespace system::sheduling
