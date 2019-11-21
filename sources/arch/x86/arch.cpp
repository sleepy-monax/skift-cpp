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
