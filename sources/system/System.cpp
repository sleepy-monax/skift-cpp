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

static uint64_t _current_tick;

void PANIC(const char *message)
{
    logger_fatal("KERNEL PANIC: {}", message);
    print("\nKERNEL PANIC: {}\n", message);

    arch::stop();
}

void tick()
{
    __atomic_add_fetch(&_current_tick, 1, __ATOMIC_SEQ_CST);
}

uint64_t get_tick()
{
    return __atomic_load_n(&_current_tick, __ATOMIC_SEQ_CST);
}

} // namespace system