#pragma once

#include <libruntime/RefCounted.h>

#include "system/platform/Context.h"
#include "system/tasking/Stack.h"

namespace system::tasking
{

typedef void (*ThreadEntry)(void);

enum class ThreadPromotion
{
    SUPERVISOR,
    USER,
};

class Thread : public libruntime::RefCounted<Thread>
{
private:
    ThreadEntry _entry;

    Stack _stack;
    Stack _userstack;

    ThreadPromotion _promotion;

    bool _started;

public:
    Thread(ThreadPromotion promotion, ThreadEntry entry);

    virtual ~Thread();

    virtual void prepare_thread(){};

    virtual void finalize_thread(){};

    void start();

    Stack &stack() { return _stack; }

    Stack &userstack() { return _userstack; }

    ThreadEntry entry() { return _entry; }

    ThreadPromotion promotion() { return _promotion; }
};

} // namespace system::tasking
