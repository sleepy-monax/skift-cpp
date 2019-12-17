#include <libruntime/LinkedList.h>
#include <libsystem/Logger.h>
#include <libsystem/__plugs__.h>
#include <libruntime/SpinLock.h>

#include "arch/Arch.h"
#include "system/sheduling/Sheduling.h"

using namespace libruntime;
using namespace system::tasking;

namespace system::sheduling
{

static RefPtr<Thread> _running_thread;
static LinkedList<RefPtr<Thread>> *_threads;
libruntime::SpinLock _threads_lock;

void initialize()
{
    logger_info("Initializing sheduling");

    _threads = new LinkedList<RefPtr<Thread>>();
}

void register_thread(RefPtr<Thread> thread)
{
    _threads_lock.acquire();

    _threads->push_back(thread);

    if (_running_thread.necked() == NULL)
    {
        logger_info("Using {} has running thread", thread);
        _running_thread = thread;
    }

    _threads_lock.release();
}

void unregister_thread(RefPtr<Thread> thread)
{
    _threads_lock.acquire();

    _threads->remove_all(thread);

    _threads_lock.release();
}

libruntime::RefPtr<system::tasking::Thread> running_thread()
{
    assert(_running_thread.necked() != NULL);

    return _running_thread;
}

libruntime::RefPtr<system::tasking::Process> running_process()
{
    assert(_running_thread.necked() != NULL);

    return _running_thread->process();
}

bool can_shedule()
{
    return !__plugs__::memory_is_lock() &&
           !_threads_lock.is_acquired();
}

uintptr_t shedule(uintptr_t stack_pointer)
{
    if (_running_thread.necked() == NULL)
    {
        return stack_pointer;
    }

    if (can_shedule())
    {
        _running_thread->stack().set_pointer(stack_pointer);

        _running_thread = _threads->peek_and_pushback();

        return _running_thread->stack().get_pointer();
    }
    else
    {
        return stack_pointer;
    }
}

} // namespace system::sheduling
