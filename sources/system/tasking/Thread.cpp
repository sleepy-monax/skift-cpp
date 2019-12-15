#include <libsystem/Assert.h>
#include <libsystem/Formattable.h>
#include <libsystem/Logger.h>

#include "arch/Arch.h"
#include "system/sheduling/Sheduling.h"
#include "system/tasking/Thread.h"

namespace system::tasking
{

static volatile int _thread_id_counter;

Thread::Thread(libruntime::RefPtr<Process> process, ThreadEntry entry)
    : _id(__sync_add_and_fetch(&_thread_id_counter, 1)),
      _entry(entry),
      _process(process),
      _stack(16),
      _userstack(16),
      _started(false)
{
}

Thread::~Thread()
{
}

void Thread::start()
{
    assert(!_started);

    finalize();

    sheduling::register_thread(libruntime::RefPtr(*this));

    _started = true; // It's too late to change anything...
}

libruntime::ErrorOr<size_t> Thread::format(libsystem::Stream &stream, libsystem::FormatInfo &info)
{
    __unused(info);

    return libsystem::format(stream, "Thread({})", id());
}

libruntime::RefPtr<Thread> Thread::create(libruntime::RefPtr<Process> process, ThreadEntry entry)
{
    auto new_thread = arch::create_thread(process, entry);

    new_thread->prepare();

    return new_thread;
}

void Thread::exit()
{
    sheduling::unregister_thread(sheduling::running_thread());

    arch::yield();
    assert_not_reached();
}

} // namespace system::tasking
