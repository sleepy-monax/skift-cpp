#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libruntime/OwnPtr.h>
#include <libruntime/RefCounted.h>

#include "system/platform/Context.h"
#include "system/sheduling/Blocker.h"
#include "system/sheduling/Policy.h"
#include "system/tasking/Process.h"
#include "system/tasking/Stack.h"

namespace system::tasking
{

class Process;

typedef void (*ThreadEntry)(void);

#define THREAD_STATE_LIST(__ENTRY) \
    __ENTRY(EMBRYO)                \
    __ENTRY(RUNNING)               \
    __ENTRY(READY)                 \
    __ENTRY(BLOCKED)               \
    __ENTRY(STOPPED)

#define THREAD_STATE_ENUM_ENTRY(__x) __x,

enum class ThreadState
{
    THREAD_STATE_LIST(THREAD_STATE_ENUM_ENTRY) __COUNT
};

class Thread : public libruntime::RefCounted<Thread>, public libsystem::Formattable
{
private:
    int _id;
    ThreadEntry _entry;
    ThreadState _state;
    libruntime::RefPtr<Process> _process;
    Stack _stack;
    Stack _userstack;

    libruntime::OwnPtr<system::sheduling::Policy> _policy;
    libruntime::OwnPtr<system::sheduling::Blocker> _blocker;

public:
    int id() { return _id; }
    ThreadEntry entry() { return _entry; }
    ThreadState state() { return _state; }
    const char *state_string();
    Stack &stack() { return _stack; }
    Stack &userstack() { return _userstack; }
    libruntime::RefPtr<Process> process() { return _process; }
    Promotion promotion() { return _process->promotion(); }

    Thread(libruntime::RefPtr<Process> process, ThreadEntry entry);
    virtual ~Thread();

    virtual void prepare() = 0;
    virtual void finalize() = 0;

    void start();
    void set_policy(libruntime::OwnPtr<system::sheduling::Policy> policy);
    void set_block(libruntime::OwnPtr<system::sheduling::Blocker> blocker);

    static libruntime::RefPtr<Thread> create(libruntime::RefPtr<Process> process, ThreadEntry entry);
    static void exit() __noreturn;

    libruntime::ErrorOr<size_t> format(libsystem::Stream &stream, libsystem::FormatInfo &info);
};

} // namespace system::tasking
