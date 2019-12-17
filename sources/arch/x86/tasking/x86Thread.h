#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include "arch/x86/interupts/InteruptStackFrame.h"
#include "system/tasking/Process.h"
#include "system/tasking/Thread.h"

namespace x86
{

class x86Thread : public system::tasking::Thread
{
public:
    x86Thread(
        libruntime::RefPtr<system::tasking::Process> process,
        system::tasking::ThreadEntry entry) : Thread(process, entry) {}

    ~x86Thread() {}

    void prepare()
    {
        stack().push(0);
        stack().push(0);

        userstack().push(0);
        userstack().push(0);
    }

    void finalize()
    {
        InteruptStackFrame frame;

        if (promotion() == system::tasking::Promotion::USER)
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

        frame.ebp = stack().get_pointer();
        frame.esp = stack().get_pointer();
        frame.eip = (u32)entry();
        frame.eflags = 0x202;

        stack().push(&frame);
    }
};

} // namespace  x86
