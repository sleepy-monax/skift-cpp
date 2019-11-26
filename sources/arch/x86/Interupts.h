#pragma once

#include <libruntime/Types.h>
#include <libruntime/Macros.h>

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
    u16 _offset0_15; // offset bits 0..15
    u16 _selector;   // a code segment selector in GDT or LDT
    u8 _zero;
    u8 _type_attr;    // type and attributes
    u16 _offset16_31; // offset bits 16..31

    IdtEntry() {}

    IdtEntry(uintptr_t offset, u32 selector, u8 type)
    {
        _offset0_15 = (offset)&0xffff;
        _selector = (selector);
        _type_attr = (type);
        _offset16_31 = ((offset) >> 16) & 0xffff;

        _zero = 0;
    }
};

void interupts_initialise();

extern "C" void interupts_handle();

} // namespace x86
