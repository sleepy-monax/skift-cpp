#include "arch/x86/Interupts.h"
#include "arch/x86/pic.h"

x86::IdtDescriptor idt_descriptor;
x86::IdtEntry idt_entries[IDT_ENTRY_COUNT];

extern "C" uintptr_t __interrupt_vector[];

void x86::interupts_initialise()
{
    x86::pic_remap();

    /* --- ISRs ------------------------------------------------------------- */

    for (int i = 0; i < 32; i++)
    {
        idt_entries[i] = x86::IdtEntry(__interrupt_vector[i], 0x08, IDT_INTGATE);
    }

    /* --- IRQs ------------------------------------------------------------- */

    for (int i = 32; i < 48; i++)
    {
        idt_entries[i] = x86::IdtEntry(__interrupt_vector[i], 0x08, IDT_TRAPGATE);
    }

    /* --- Syscalls --------------------------------------------------------- */

    idt_entries[128] = x86::IdtEntry(__interrupt_vector[48], 0x08, IDT_TRAPGATE);
}

extern "C" void x86::interupts_handle() {}