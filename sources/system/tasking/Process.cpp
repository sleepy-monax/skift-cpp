/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libsystem/Logger.h>

#include "arch/Arch.h"
#include "system/tasking/Process.h"

namespace system::tasking
{

static volatile int _process_id_counter = -1;

Process::Process(Process *parent, libruntime::String name)
    : _id(__sync_add_and_fetch(&_process_id_counter, 1)),
      _parent(parent),
      _name(name)
{
}

Process::~Process()
{
}

libruntime::ErrorOr<size_t> Process::format(libsystem::Stream &stream, libsystem::FormatInfo &info)
{
    __unused(info);

    return libsystem::format(stream, "{}({})", _name, _id);
}

} // namespace system::tasking
