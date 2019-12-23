#pragma once

#include "system/tasking/Process.h"

namespace system::tasking
{

libruntime::RefPtr<Process> kernel_process();

void initialize();

} // namespace system::tasking
