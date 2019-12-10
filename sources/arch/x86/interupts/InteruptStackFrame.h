#pragma once

#include <libruntime/Macros.h>
#include <libruntime/Types.h>

namespace x86
{

struct __packed InteruptStackFrame
{
    u32 gs, fs, es, ds;
    u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
    u32 intno, err;
    u32 eip, cs, eflags;
};

} // namespace x86