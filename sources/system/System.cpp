/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libsystem/Assert.h>
#include <libsystem/Logger.h>
#include <libsystem/Stdio.h>

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