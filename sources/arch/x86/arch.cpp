#include <arch/Arch.h>

void arch::stop()
{
    do
    {
        asm volatile("hlt");
    } while (1);
}

size_t arch::get_page_size()
{
    return 4096;
}

using namespace system::memory;

extern int __kernel_start;
extern int __kernel_end;

system::memory::Region arch::get_kernel_region()
{
    uintptr_t addr = (uintptr_t)&__kernel_start;
    size_t size = &__kernel_end - &__kernel_start;

    return Region::create_around_non_aligned_address(addr, size);
}