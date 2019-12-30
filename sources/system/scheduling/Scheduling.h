#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include "system/tasking/Process.h"
#include "system/tasking/Thread.h"

namespace system::scheduling
{

void initialize();

void update_thread_state(libruntime::RefPtr<system::tasking::Thread> thread, system::tasking::ThreadState new_state);

uintptr_t schedule(uintptr_t stack_pointer);

libruntime::RefPtr<system::tasking::Thread> running_thread();

libruntime::RefPtr<system::tasking::Process> running_process();

} // namespace system::scheduling
