#include <libsystem/Assert.h>

#include "arch/Arch.h"
#include "system/tasking/Thread.h"

namespace system::tasking
{

Thread::Thread(ThreadPromotion promotion, ThreadEntry entry)
    : _entry(entry),
      _stack(16),
      _userstack(16),
      _promotion(promotion),
      _started(false)
{
    prepare_thread();
}

Thread::~Thread()
{
}

void Thread::start()
{
    assert(!_started);

    finalize_thread();

    _started = true; // It's too late to change anything...
}

} // namespace system::tasking
