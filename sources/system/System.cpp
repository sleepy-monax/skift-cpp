#include <libsystem/Stdio.h>
#include <libsystem/Logger.h>
#include <libsystem/Assert.h>

#include "arch/Arch.h"
#include "system/System.h"

using namespace libsystem;

namespace system
{

void PANIC(const char *message)
{
    logger_fatal("KERNE PANIC: {}", message);
    print("\nKERNEL PANIC: {}\n", message);

    arch::stop();
}

} // namespace system