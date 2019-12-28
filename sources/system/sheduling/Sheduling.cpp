/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libsystem/__plugs__.h>

#include <libruntime/LinkedList.h>
#include <libruntime/SpinLock.h>
#include <libsystem/Logger.h>

#include "arch/Arch.h"
#include "system/sheduling/Sheduling.h"

using namespace libruntime;
using namespace system::tasking;

namespace system::sheduling
{

static RefPtr<Thread> _running_thread;
static LinkedList<RefPtr<Thread>> *_ready_threads;
static LinkedList<RefPtr<Thread>> *_blocked_threads;

libruntime::SpinLock _threads_lock;

void initialize()
{
    logger_info("Initializing sheduling");

    _ready_threads = new LinkedList<RefPtr<Thread>>();
    _blocked_threads = new LinkedList<RefPtr<Thread>>();
}

void update_thread_state(RefPtr<Thread> thread, ThreadState new_state)
{
    _threads_lock.acquire();

    if (thread->state() == ThreadState::EMBRYO)
    {
        if (_running_thread == nullptr)
        {
            logger_info("Using {} has running thread", thread);
            _running_thread = thread;
        }
    }
    else if (thread->state() == ThreadState::BLOCKED)
    {
        _blocked_threads->remove_first(thread);
    }
    else if (thread->state() == ThreadState::READY)
    {
        _ready_threads->remove_first(thread);
    }

    thread->set_state(new_state);

    if (thread->state() == ThreadState::BLOCKED)
    {
        _blocked_threads->push_back(thread);
    }
    else if (thread->state() == ThreadState::READY)
    {
        _ready_threads->push_back(thread);
    }
    else if (thread->state() == ThreadState::STOPPED)
    {
    }

    _threads_lock.release();
}

libruntime::RefPtr<system::tasking::Thread> running_thread()
{
    assert(_running_thread != nullptr);

    return _running_thread;
}

libruntime::RefPtr<system::tasking::Process> running_process()
{
    assert(_running_thread != nullptr);

    return _running_thread->process();
}

bool can_shedule()
{
    return !__plugs__::memory_is_lock() &&
           !_threads_lock.is_acquired();
}

static void unblock_blocked_thread()
{
    _blocked_threads->foreach ([](auto thread) {
        if (thread->should_unblock())
        {
            logger_info("Unblocking {}", thread);
            thread->unblock();
            thread->set_state(ThreadState::READY);
            _blocked_threads->remove_first(thread);
            _ready_threads->push_back(thread);
        }

        return Iteration::CONTINUE;
    });
}

uintptr_t shedule(uintptr_t stack_pointer)
{
    if (_running_thread == nullptr)
    {
        return stack_pointer;
    }

    if (can_shedule())
    {
        _threads_lock.acquire();

        _running_thread->stack().set_pointer(stack_pointer);

        unblock_blocked_thread();

        if (_running_thread->state() == ThreadState::RUNNING)
        {
            _ready_threads->push_back(_running_thread);
            _running_thread->set_state(ThreadState::READY);
        }

        _running_thread = RefPtr(*_ready_threads->pop());
        _running_thread->set_state(ThreadState::RUNNING);

        _threads_lock.release();

        return _running_thread->stack().get_pointer();
    }
    else
    {
        return stack_pointer;
    }
}

} // namespace system::sheduling
