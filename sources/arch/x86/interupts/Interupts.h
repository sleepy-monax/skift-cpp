#pragma once

#include <libruntime/Macros.h>
#include <libruntime/Types.h>

namespace x86
{

#define IDT_DPL_KERNEL 0b00000000
#define IDT_DPL_USER 0b01100000

#define IDT_INTGATE 0x8E
#define IDT_TRAPGATE 0x8F
#define IDT_ENTRY_COUNT 256

struct __packed IdtDescriptor
{
    u16 size;
    u32 offset;
};

struct __packed IdtEntry
{
    u16 offset0_15; // offset bits 0..15
    u16 selector;   // a code segment selector in GDT or LDT
    u8 zero;
    u8 type_attr;    // type and attributes
    u16 offset16_31; // offset bits 16..31

    static IdtEntry create(uintptr_t offset, u32 selector, u8 type)
    {
        IdtEntry entry;

        entry.offset0_15 = offset & 0xffff;
        entry.selector = selector;
        entry.type_attr = type;
        entry.offset16_31 = (offset >> 16) & 0xffff;
        entry.zero = 0;

        return entry;
    }
};

void interupts_initialise();

} // namespace x86
