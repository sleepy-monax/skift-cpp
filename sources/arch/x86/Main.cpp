#include <libsystem/Logger.h>
#include <libsystem/Stdio.h>

#include "arch/Arch.h"
#include "arch/x86/CGAScreen.h"
#include "arch/x86/Interupts.h"
#include "arch/x86/Multiboot.h"
#include "arch/x86/Segmentation.h"
#include "arch/x86/SerialStream.h"
#include "arch/x86/TerminalStream.h"

#include "system/memory/Memory.h"
#include "system/sheduling/Sheduling.h"

using namespace x86;
using namespace system;
using namespace libruntime;
using namespace libsystem;

RefPtr<Stream> libsystem::stdin;
RefPtr<Stream> libsystem::stdout;
RefPtr<Stream> libsystem::stderr;
RefPtr<Stream> libsystem::stdlog;

extern "C" void arch_main(u32 multiboot_magic, multiboot_info_t *multiboot_info)
{
    auto serial = SerialStream(SerialPort::COM1);
    auto terminal = TerminalStream(CGAScreen((void *)0xB8000));

    // We don't went the ref count to try to free these.
    serial.make_orphan();
    terminal.make_orphan();

    //libsystem::stdin = nullptr;
    libsystem::stderr = adopt(serial);
    libsystem::stdout = adopt(terminal);
    libsystem::stdlog = adopt(serial);

    logger_info("Booting...");
    logger_info("hjert kernel ({})", __BUILD_TARGET__);
    logger_info("Kernel build on \"{}\"", __BUILD_UNAME__);

    auto multiboot = Multiboot(multiboot_magic, multiboot_info);

    if (!multiboot.is_valid())
    {
        logger_info("Invalid bootloader, how did you do that !?");
    }
    else
    {
        logger_info("Bootloader is {}", multiboot.bootloader());
    }

    multiboot.with_memory_map([](auto entry) {
        logger_trace("Memory map entry: {#x} {}kib", entry.address(), entry.size() / 1024);

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

    x86::segmentation_initialize();
    x86::interupts_initialise();

    sheduling::initialize();

    print("hjert kernel v0.0.1\n");
    print("--------------------------------------------------------------------------------\n");
    print("System halted!\n");

    asm volatile("int $0x80");

    while (1)
    {
        /* code */
    }
}