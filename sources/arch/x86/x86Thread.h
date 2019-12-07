#pragma once

#include "arch/x86/InteruptStackFrame.h"
#include "system/tasking/Thread.h"
#include "system/tasking/Process.h"

namespace x86
{

class x86Thread : public system::tasking::Thread
{
public:
    x86Thread(
        system::tasking::Process &process,
        system::tasking::ThreadPromotion promotion,
        system::tasking::ThreadEntry entry) : Thread(process, promotion, entry) {}

    ~x86Thread() {}

    void prepare_task()
    {
        stack().push(0);
        stack().push(0);

        userstack().push(0);
        userstack().push(0);
    }

    void finalize_task()
    {
        InteruptStackFrame frame;

        if (promotion() == system::tasking::ThreadPromotion::USER)
        {
            stack().push((u32)0x20);
            stack().push(userstack().get_pointer());
            stack().push((u32)0x202);

            frame.cs = 0x18;
            frame.ds = 0x20;
            frame.es = 0x20;
            frame.fs = 0x20;
            frame.gs = 0x20;
        }
        else
        {
            frame.cs = 0x08;
            frame.ds = 0x10;
            frame.es = 0x10;
            frame.fs = 0x10;
            frame.gs = 0x10;
        }

        frame.eax = 0;
        frame.ebx = 0;
        frame.ecx = 0;
        frame.edx = 0;

        frame.edi = 0;
        frame.esi = 0;
        frame.ebp = stack().get_pointer();
        frame.esp = stack().get_pointer();
        frame.intno = 0;
        frame.err = 0;
        frame.eip = (u32)entry();
        frame.eflags = 0x202;

        stack().push(&frame);
    }
};

} // namespace  x86
