/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libsystem/Assert.h>
#include <libsystem/Formattable.h>
#include <libsystem/Logger.h>

#include "arch/Arch.h"
#include "system/sheduling/BlockerJoin.h"
#include "system/sheduling/BlockerSleep.h"
#include "system/sheduling/Sheduling.h"
#include "system/tasking/Thread.h"

namespace system::tasking
{

static volatile int _thread_id_counter;

const char *state_string();

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

// void Thread::set_policy(libruntime::OwnPtr<system::sheduling::Policy> policy)
// {
// }

void Thread::block(libruntime::OwnPtr<system::sheduling::Blocker> blocker)
{
    _blocker = blocker;
    switch_state(ThreadState::BLOCKED);
    arch::yield();
}

void Thread::switch_state(ThreadState new_state)
{
    sheduling::update_thread_state(*this, new_state);
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

    return new_thread;
}

void Thread::exit()
{
    logger_info("{} kill himself.", sheduling::running_thread());
    sheduling::update_thread_state(sheduling::running_thread(), ThreadState::STOPPED);
    arch::yield();
    assert_not_reached();
}

void Thread::sleep(libsystem::Millisecond time)
{
    logger_info("{} is going to sleep for {}ms", sheduling::running_thread(), time);
    sheduling::running_thread()->block(new system::sheduling::BlockerSleep(time));
}

void Thread::join(libruntime::RefPtr<Thread> thread_to_join)
{
    logger_info("{} is joining {}", sheduling::running_thread(), thread_to_join);
    sheduling::running_thread()->block(new system::sheduling::BlockerJoin(thread_to_join));
}

} // namespace system::tasking
