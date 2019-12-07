#include <libruntime/LinkedList.h>
#include <libsystem/Logger.h>

#include "arch/Arch.h"
#include "system/sheduling/Sheduling.h"

using namespace libruntime;
using namespace system::tasking;

namespace system::sheduling
{

static RefPtr<Thread> _running_thread;
static LinkedList<RefPtr<Thread>> *_threads;

void initialize()
{
    logger_info("Initializing sheduling");

    _threads = new LinkedList<RefPtr<Thread>>();

    logger_trace("Creating the kernel thread");

    logger_info("The kernel thread has been created");
}

void register_thread(RefPtr<Thread> thread)
{
    _threads->push_back(thread);
}

void unregister_thread(RefPtr<Thread> thread)
{
    _threads->remove_all(thread);
}

} // namespace system::sheduling
