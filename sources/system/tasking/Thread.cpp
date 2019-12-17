#include <libsystem/Assert.h>
#include <libsystem/Formattable.h>
#include <libsystem/Logger.h>

#include "arch/Arch.h"
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

    sheduling::register_thread(libruntime::RefPtr(*this));

    _state = ThreadState::READY;
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
    sheduling::unregister_thread(sheduling::running_thread());
    arch::yield();
    assert_not_reached();
}

} // namespace system::tasking
