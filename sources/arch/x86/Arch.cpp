/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <arch/Arch.h>

#include "arch/x86/tasking/x86Thread.h"
#include "arch/x86/x86.h"

using namespace system::memory;

extern int __kernel_start;
extern int __kernel_end;

namespace arch
{

void stop()
{
    do
    {
        x86::cli();
        x86::hlt();
    } while (1);
}

void idle()
{
    do
    {
        x86::hlt();
    } while (1);
}

void halt()
{
    x86::hlt();
}

void yield()
{
    x86::raise_irq1();
}

size_t get_page_size()
{
    return 4096;
}

system::memory::Region get_kernel_region()
{
    uintptr_t addr = reinterpret_cast<uintptr_t>(&__kernel_start);
    size_t size = &__kernel_end - &__kernel_start;

    return Region::create_around_non_aligned_address(addr, size);
}

libruntime::RefPtr<system::tasking::Thread> create_thread(libruntime::RefPtr<system::tasking::Process> process, system::tasking::ThreadEntry entry)
{
    return libruntime::make<x86::x86Thread>(process, entry);
}

} // namespace arch
