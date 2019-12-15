#pragma once

#include "system/tasking/Process.h"
#include "system/tasking/Thread.h"

namespace system::sheduling
{

void initialize();

void register_thread(libruntime::RefPtr<system::tasking::Thread> thread);

void unregister_thread(libruntime::RefPtr<system::tasking::Thread> thread);

uintptr_t shedule(uintptr_t stack_pointer);

libruntime::RefPtr<system::tasking::Thread> running_thread();

libruntime::RefPtr<system::tasking::Process> running_process();

} // namespace system::sheduling
