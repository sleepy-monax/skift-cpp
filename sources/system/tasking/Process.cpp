#include <libsystem/Logger.h>

#include "arch/Arch.h"
#include "system/tasking/Process.h"

namespace system::tasking
{

static volatile int _process_id_counter = -1;

Process::Process(Process *parent)
    : _id(__sync_add_and_fetch(&_process_id_counter, 1)),
      _parent(parent)
{
    _threads = libruntime::LinkedList<libruntime::RefPtr<Thread>>();
}

Process::~Process()
{
}

libruntime::RefPtr<Thread> Process::create_thread(ThreadEntry entry)
{
    auto new_thread = arch::create_thread(*this, _promotion, entry);
    new_thread->prepare();

    _threads.push_back(new_thread);

    return new_thread;
}

libruntime::ErrorOr<size_t> Process::format(libsystem::Stream &stream, libsystem::FormatInfo &info)
{
    __unused(info);

    return libsystem::format(stream, "Process({})", _id);
}

} // namespace system::tasking
