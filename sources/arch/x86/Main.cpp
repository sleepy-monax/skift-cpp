/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libsystem/Logger.h>
#include <libsystem/Stdio.h>

#include "arch/Arch.h"
#include "arch/x86/boot/Multiboot.h"
#include "arch/x86/device/CGATerminal.h"
#include "arch/x86/device/SerialStream.h"
#include "arch/x86/interupts/Interupts.h"
#include "arch/x86/segmentation/Segmentation.h"

#include "system/System.h"
#include "system/memory/Memory.h"
#include "system/sheduling/Sheduling.h"
#include "system/tasking/Process.h"
#include "system/tasking/Tasking.h"

using namespace system;
using namespace libruntime;
using namespace libsystem;

RefPtr<Stream> libsystem::stdin;
RefPtr<Stream> libsystem::stdout;
RefPtr<Stream> libsystem::stderr;
RefPtr<Stream> libsystem::stdlog;

void taskA()
{
    for (int i = 0; i < 32; i++)
    {
        stderr->write("A", 1);
        arch::halt();
    }

    system::tasking::Thread::exit();
}

void taskB()
{
    for (int i = 0; i < 16; i++)
    {
        stderr->write("B", 1);
        arch::halt();
    }

    tasking::Thread::exit();
}

extern "C" void arch_main(uint32_t multiboot_magic, uintptr_t multiboot_addr)
{
    auto serial = SerialStream(SerialPort::COM1);
    auto terminal = Stream(); // Create a dummy stream for the terminal

    // We don't want RefPtr to try to delete these.
    serial.make_orphan();
    terminal.make_orphan();

    //libsystem::stdin = nullptr;
    libsystem::stderr = adopt(serial);
    libsystem::stdout = adopt(terminal);
    libsystem::stdlog = adopt(serial);

    logger_info("Booting...");
    logger_info("hjert kernel ({} {})", __BUILD_TARGET__, __BUILD_GITREF__);
    logger_info("Kernel build on \"{}\"", __BUILD_UNAME__);

    auto multiboot = x86::Multiboot(multiboot_magic, multiboot_addr);

    if (!multiboot.is_valid())
    {
        logger_fatal("Invalid bootloader, how did you do that !?");
        PANIC("Invalid bootloader, how did you do that !?");
    }
    else
    {
        logger_info("Bootloader is {}", multiboot.bootloader());
    }

    multiboot.with_memory_map([](auto entry) {
        if (entry.is_available())
        {
            logger_info("Marking {} as free usable memory by the kernel...", entry);
            memory::free_region(entry.region());
        }
        else if (entry.is_bad())
        {
            logger_warn("Badram at {}, skipping...", entry.region());
        }
        else
        {
            logger_info("Skipping {}...", entry);
        }

        return Iteration::CONTINUE;
    });

    if (!memory::is_bootstraped())
    {
        PANIC("Failled to bootstrap the memory manager!");
    }

    x86::segmentation_initialize();
    x86::interupts_initialise();

    libsystem::stdout = make<x86::CGATerminal>(reinterpret_cast<void *>(0xB8000));

    sheduling::initialize();
    tasking::initialize();

    auto task_a = tasking::Thread::create(tasking::kernel_process(), reinterpret_cast<tasking::ThreadEntry>(taskA));
    task_a->start();
    tasking::Thread::create(tasking::kernel_process(), reinterpret_cast<tasking::ThreadEntry>(taskB))->start();

    print(" ☺ hjert kernel ({} {})\n", __BUILD_TARGET__, __BUILD_GITREF__);
    print("════════════════════════════════════════════════════════════════════════════════\n");

    print(u8"~ \e[94mµ\e[m ");

    tasking::Thread::join(task_a);

    do
    {
        stderr->write("K", 1);

        arch::halt();
    } while (true);

    asm volatile("int $0x80");

    arch::idle();
}
