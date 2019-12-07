#include <libsystem/Logger.h>

#include "arch/Arch.h"
#include "system/tasking/Process.h"

namespace system::tasking
{

static volatile int _process_id_counter;

Process::Process(Process *parent)
    : _id(__sync_add_and_fetch(&_process_id_counter, 1)),
      _parent(parent)
{
    logger_info("Constructing process");

    _threads = libruntime::LinkedList<libruntime::RefPtr<Thread>>();
}

Process::~Process()
{
    logger_info("Destructing process");
}

libruntime::RefPtr<Thread> Process::create_thread(ThreadEntry entry)
{
    auto new_thread = arch::create_thread(*this, _promotion, entry);

    logger_info("Attaching {}", new_thread.necked());
    _threads.push_back(new_thread);
    logger_info("{} Attached", new_thread.necked());

    return new_thread;
}

} // namespace system::tasking
