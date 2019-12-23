
#include "system/tasking/Tasking.h"
#include "arch/Arch.h"
#include "system/tasking/Process.h"

namespace system::tasking
{

static void idle_task_code()
{
    do
    {
        arch::idle();
    } while (true);
}

static libruntime::RefPtr<Process> _kernel_process;

void initialize()
{
    _kernel_process = libruntime::make<Process>(nullptr, "Kernel");

    // Create the main kernel thread...
    tasking::Thread::create(_kernel_process, nullptr)->start();

    // Create the idle task
    tasking::Thread::create(_kernel_process, idle_task_code)->start();
}

libruntime::RefPtr<Process> kernel_process()
{
    return _kernel_process;
}

} // namespace system::tasking
