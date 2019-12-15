#pragma once

#include <libruntime/RefCounted.h>

#include "system/platform/Context.h"
#include "system/tasking/Process.h"
#include "system/tasking/Stack.h"

namespace system::tasking
{

class Process;

typedef void (*ThreadEntry)(void);

class Thread : public libruntime::RefCounted<Thread>, public libsystem::Formattable
{
private:
    int _id;

    ThreadEntry _entry;

    libruntime::RefPtr<Process> _process;

    Stack _stack;
    Stack _userstack;

    bool _started;

public:
    Thread(libruntime::RefPtr<Process> process, ThreadEntry entry);

    virtual ~Thread();

    virtual void prepare() = 0;

    virtual void finalize() = 0;

    void start();

    int id() { return _id; }

    ThreadEntry entry() { return _entry; }

    Stack &stack() { return _stack; }

    Stack &userstack() { return _userstack; }

    libruntime::RefPtr<Process> process() { return _process; }

    Promotion promotion() { return _process->promotion(); }

    bool started() { return _started; }

    libruntime::ErrorOr<size_t> format(libsystem::Stream &stream, libsystem::FormatInfo &info);

    static libruntime::RefPtr<Thread> create(libruntime::RefPtr<Process> process, ThreadEntry entry);

    static void exit() __noreturn;
};

} // namespace system::tasking
