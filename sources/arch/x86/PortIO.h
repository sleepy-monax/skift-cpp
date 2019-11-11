#pragma once

#include <libruntime/Types.h>

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