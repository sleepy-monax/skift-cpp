#include <libsystem/Logger.h>

#include "arch/x86/interupts/InteruptStackFrame.h"
#include "arch/x86/interupts/Interupts.h"
#include "arch/x86/interupts/Pic.h"
#include "arch/x86/x86.h"

#include "arch/Arch.h"
#include "system/System.h"
#include "system/sheduling/Sheduling.h"

x86::IdtEntry idt_entries[IDT_ENTRY_COUNT] = {0};

x86::IdtDescriptor idt_descriptor = {
    .size = sizeof(x86::IdtEntry) * IDT_ENTRY_COUNT,
    .offset = (u32)&idt_entries[0],
};

extern "C" uintptr_t __interrupt_vector[];

void x86::interupts_initialise()
{
    logger_info("Remaping the PIC...");

    x86::pic_remap();

    logger_info("Populating the IDT...");

    // ISRs
    for (int i = 0; i < 32; i++)
    {
        idt_entries[i] = x86::IdtEntry::create(__interrupt_vector[i], 0x08, IDT_INTGATE);
    }

    // IRQs
    for (int i = 32; i < 48; i++)
    {
        idt_entries[i] = x86::IdtEntry::create(__interrupt_vector[i], 0x08, IDT_TRAPGATE);
    }

    // Syscalls
    idt_entries[128] = x86::IdtEntry::create(__interrupt_vector[48], 0x08, IDT_TRAPGATE);

    logger_info("Loading the IDT...");
    x86::load_idt((u32)&idt_descriptor);

    logger_info("We should get interupts now...");
    x86::sti();
}

extern "C" u32 interupts_handle(u32 esp, x86::InteruptStackFrame stackframe)
{

    if (stackframe.intno < 32)
    {
        logger_fatal("x86 CPU exception: {} error={}", stackframe.intno, stackframe.err);
        system::PANIC("CPU exception!");
    }

    if (stackframe.intno == 32)
    {
        esp = system::sheduling::shedule(esp);
    }

    x86::pic_ack(stackframe.intno);

    return esp;
}