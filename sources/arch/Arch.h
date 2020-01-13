#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libruntime/Macros.h>
#include <libruntime/RefPtr.h>
#include <libruntime/Types.h>

#include "system/memory/MemoryRegion.h"
#include "system/memory/AddressSpace.h"
#include "system/tasking/Thread.h"

namespace arch
{

void stop() __noreturn;

void idle() __noreturn;

void halt();

void yield();

size_t get_page_size();

system::memory::MemoryRegion get_kernel_region();

libruntime::RefPtr<system::tasking::Thread> create_thread(
    libruntime::RefPtr<system::tasking::Process> process,
    system::tasking::ThreadEntry entry);

libruntime::OwnPtr<system::memory::AddressSpace> create_address_space();

} // namespace arch
