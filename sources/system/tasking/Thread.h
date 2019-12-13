#pragma once

#include <libruntime/RefCounted.h>

#include "system/platform/Context.h"
#include "system/tasking/Stack.h"

namespace system::tasking
{

class Process;

typedef void (*ThreadEntry)(void);

enum class ThreadPromotion
{
    SUPERVISOR,
    USER,
};

class Thread : public libruntime::RefCounted<Thread>, public libsystem::Formattable
{
private:
    int _id;

    ThreadEntry _entry;
    ThreadPromotion _promotion;

    Process &_process;

    Stack _stack;
    Stack _userstack;

    bool _started;

public:
    Thread(Process &process, ThreadPromotion promotion, ThreadEntry entry);

    virtual ~Thread();

    virtual void prepare() = 0;

    virtual void finalize() = 0;

    void start();

    int id() { return _id; }

    ThreadEntry entry() { return _entry; }

    Stack &stack() { return _stack; }

    Stack &userstack() { return _userstack; }

    ThreadPromotion promotion() { return _promotion; }

    bool started() { return _started; }

    libruntime::ErrorOr<size_t> format(libsystem::Stream &stream, libsystem::FormatInfo &info);
};

} // namespace system::tasking
