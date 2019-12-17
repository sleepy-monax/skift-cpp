#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libruntime/Macros.h>
#include <libruntime/Types.h>

namespace x86
{

#define GDT_ENTRY_COUNT 6

#define GDT_PRESENT 0b10010000
#define GDT_USER 0b01100000
#define GDT_EXECUTABLE 0b00001000
#define GDT_READWRITE 0b00000010
#define GDT_ACCESSED 0b00000001

#define GDT_FLAGS 0b1100
#define TSS_FLAGS 0

struct __packed Tss
{
    u32 prev_tss;
    u32 esp0;
    u32 ss0;
    u32 esp1;
    u32 ss1;
    u32 esp2;
    u32 ss2;
    u32 cr3;
    u32 eip;
    u32 eflags;
    u32 eax;
    u32 ecx;
    u32 edx;
    u32 ebx;
    u32 esp;
    u32 ebp;
    u32 esi;
    u32 edi;
    u32 es;
    u32 cs;
    u32 ss;
    u32 ds;
    u32 fs;
    u32 gs;
    u32 ldt;
    u16 trap;
    u16 iomap_base;
};

struct __packed GdtDescriptor
{
    u16 size;
    u32 offset;
};

struct __packed GdtEntry
{
    u16 limit0_15;
    u16 base0_15;
    u8 base16_23;
    u8 acces;
    u8 limit16_19 : 4;
    u8 flags : 4;
    u8 base24_31;

    static GdtEntry create(u32 base, u32 limit, u8 acces, u8 flags)
    {
        GdtEntry entry;

        entry.base0_15 = base & 0xffff;
        entry.base16_23 = (base >> 16) & 0xff;
        entry.base24_31 = (base >> 24) & 0xff;
        entry.limit0_15 = limit & 0xffff;
        entry.limit16_19 = (limit >> 16) & 0x0f;
        entry.acces = (acces);
        entry.flags = (flags);

        return entry;
    }
};

void segmentation_initialize();

} // namespace x86
