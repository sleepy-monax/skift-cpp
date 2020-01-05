/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libruntime/SpinLock.h>
#include <libsystem/Assert.h>
#include <libsystem/Formattable.h>
#include <libsystem/Logger.h>

#include "arch/Arch.h"
#include "system/scheduling/BlockerJoin.h"
#include "system/scheduling/BlockerSleep.h"
#include "system/scheduling/Scheduling.h"
#include "system/tasking/Thread.h"

namespace system::tasking
{

static libruntime::SpinLock _threads_lock;
static libruntime::LinkedList<libruntime::RefPtr<Thread>> _threads;
static volatile int _thread_id_counter;

Thread::Thread(libruntime::RefPtr<Process> process, ThreadEntry entry)
    : _id(__sync_add_and_fetch(&_thread_id_counter, 1)),
      _entry(entry),
      _state(ThreadState::EMBRYO),
      _process(process),
      _stack(16),
      _userstack(16)
{
}

Thread::~Thread()
{
    logger_info("Destructing {}...", this);
}

#define THREAD_STATE_STRING_ENTRY(__x) #__x,

static const char *_state_string[] = {THREAD_STATE_LIST(THREAD_STATE_STRING_ENTRY)};

const char *Thread::state_string()
{
    return _state_string[static_cast<int>(_state)];
}

void Thread::start()
{
    assert(_state == ThreadState::EMBRYO);

    finalize();

    switch_state(ThreadState::READY);
}

// void Thread::set_policy(libruntime::OwnPtr<system::scheduling::Policy> policy)
// {
// }

void Thread::block(libruntime::OwnPtr<system::scheduling::Blocker> blocker)
{
    _blocker = blocker;
    switch_state(ThreadState::BLOCKED);
    arch::yield();
}

void Thread::switch_state(ThreadState new_state)
{
    scheduling::update_thread_state(*this, new_state);
}

bool Thread::should_unblock()
{
    assert(_blocker);

    return _blocker->should_unblock();
}

void Thread::unblock()
{
    assert(_blocker);
    assert(_blocker->should_unblock());

    _blocker->unblock();
}

libruntime::ErrorOr<size_t> Thread::format(libsystem::Stream &stream, libsystem::FormatInfo &info)
{
    __unused(info);

    return libsystem::format(stream, "Thread(id={}, state={}, process={})", id(), state_string(), process());
}

libruntime::RefPtr<Thread> Thread::create(libruntime::RefPtr<Process> process, ThreadEntry entry)
{
    auto new_thread = arch::create_thread(process, entry);

    new_thread->prepare();

    _threads_lock.acquire();
    _threads.push_back(new_thread);
    _threads_lock.release();

    return new_thread;
}

void Thread::exit()
{
    logger_info("{} kill himself.", scheduling::running_thread());
    scheduling::update_thread_state(scheduling::running_thread(), ThreadState::STOPPED);
    arch::yield();
    assert_not_reached();
}

void Thread::sleep(libsystem::Millisecond time)
{
    if (time > 0)
    {
        logger_info("{} is going to sleep for {}ms", scheduling::running_thread(), time);
        scheduling::running_thread()->block(new system::scheduling::BlockerSleep(time));
    }
}

void Thread::join(libruntime::RefPtr<Thread> thread_to_join)
{
    assert(thread_to_join != nullptr);

    if (thread_to_join->state() != ThreadState::STOPPED)
    {
        logger_info("{} is joining {}", scheduling::running_thread(), thread_to_join);
        scheduling::running_thread()->block(new system::scheduling::BlockerJoin(thread_to_join));
    }
}

libruntime::RefPtr<Thread> Thread::by_id(int id)
{
    libruntime::RefPtr<Thread> result;

    _threads_lock.acquire();

    _threads.foreach ([&](auto thread) {
        if (thread->id() == id)
        {
            result = thread;

            return libruntime::Iteration::STOP;
        }

        return libruntime::Iteration::CONTINUE;
    });

    _threads_lock.release();

    return result;
}

void Thread::cleanup(libruntime::RefPtr<Thread> thread)
{
    assert(thread != nullptr);

    _threads_lock.acquire();
    _threads.remove(thread);
    _threads_lock.release();
}

void Thread::foreach (libruntime::Callback<libruntime::Iteration(libruntime::RefPtr<Thread>)> callback)
{
    _threads_lock.acquire();

    _threads.foreach ([&](auto thread) {
        return callback(thread);
    });

    _threads_lock.release();
}

} // namespace system::tasking
