#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

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
    uint16_t size;
    uint32_t offset;
};

struct __packed IdtEntry
{
    uint16_t offset0_15; // offset bits 0..15
    uint16_t selector;   // a code segment selector in GDT or LDT
    uint8_t zero;
    uint8_t type_attr;    // type and attributes
    uint16_t offset16_31; // offset bits 16..31

    static IdtEntry create(uintptr_t offset, uint32_t selector, uint8_t type)
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
