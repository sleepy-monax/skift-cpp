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

libruntime::RefPtr<system::tasking::Thread> create_thread(system::tasking::Process &process, system::tasking::ThreadPromotion promotion, system::tasking::ThreadEntry entry)
{
    return libruntime::make<x86::x86Thread>(process, promotion, entry);
}

} // namespace arch
