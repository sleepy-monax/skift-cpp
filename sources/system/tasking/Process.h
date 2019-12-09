#pragma once

#include <libruntime/LinkedList.h>
#include <libruntime/RefPtr.h>

#include "system/tasking/Thread.h"

namespace system::tasking
{

class Process : public libsystem::Formattable
{
private:
    int _id;
    ThreadPromotion _promotion;
    libruntime::LinkedList<libruntime::RefPtr<Thread>> _threads;
    Process *_parent;

public:
    Process(Process *parent);

    ~Process();

    libruntime::RefPtr<Thread> create_thread(ThreadEntry entry);

    libruntime::ErrorOr<size_t> format(libsystem::Stream &stream, libsystem::FormatInfo &info);
};

} // namespace system::tasking
