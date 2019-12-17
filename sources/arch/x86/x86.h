#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libruntime/Types.h>

namespace x86
{

static inline u8 in8(u16 port)
{
    u8 data;
    asm volatile("inb %1, %0"
                 : "=a"(data)
                 : "d"(port));
    return data;
}

static inline u16 in16(u16 port)
{
    u16 data;
    asm volatile("inw %1, %0"
                 : "=a"(data)
                 : "d"(port));
    return data;
}

static inline u32 in32(u16 port)
{
    u32 data;
    asm volatile("inl %1, %0"
                 : "=a"(data)
                 : "d"(port));
    return data;
}

static inline void out8(u16 port, u8 data)
{
    asm volatile("outb %0, %1"
                 :
                 : "a"(data), "d"(port));
}

static inline void out16(u16 port, u16 data)
{
    asm volatile("outw %0, %1" ::"a"(data), "d"(port));
}

static inline void out32(u16 port, u32 data)
{
    asm volatile("outl %0, %1"
                 :
                 : "a"(data), "d"(port));
}

static inline void raise_irq1()
{
    asm volatile("int $32");
}

static inline void cli(void) { asm volatile("cli"); }

static inline void sti(void) { asm volatile("sti"); }

static inline void hlt(void) { asm volatile("hlt"); }

extern "C" void load_gdt(u32 gdt);

extern "C" void load_idt(u32 idt);

} // namespace x86
