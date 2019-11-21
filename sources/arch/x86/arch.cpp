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

extern int __kernel_start;
extern int __kernel_end;

uintptr_t arch::get_kernel_base_address()
{
    return (uintptr_t)&__kernel_start;
}

size_t arch::get_kernel_size()
{
    return &__kernel_end - &__kernel_start;
}