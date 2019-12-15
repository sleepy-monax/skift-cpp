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
}

Process::~Process()
{
}

libruntime::ErrorOr<size_t> Process::format(libsystem::Stream &stream, libsystem::FormatInfo &info)
{
    __unused(info);

    return libsystem::format(stream, "Process({})", _id);
}

} // namespace system::tasking
