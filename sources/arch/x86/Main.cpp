#include <libsystem/Logger.h>
#include <libsystem/Stdio.h>

#include "arch/x86/CGAScreen.h"
#include "arch/x86/Multiboot.h"
#include "arch/x86/SerialStream.h"
#include "arch/x86/TerminalStream.h"
#include "arch/Arch.h"

#include "system/memory/Memory.h"

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
    // These two should never go out of scope beause there are refcounted but not own by a refptr :/
    auto serial = SerialStream(SerialPort::COM1);
    auto terminal = TerminalStream(CGAScreen((void *)0xB8000));

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
        logger_info("Invalid bootload, how did you do that !?");
    }
    else
    {
        logger_info("Bootloader is {}", multiboot.bootloader());
    }

    const char *multiboot_memory_type_name[] = {
        "AVAILABLE",
        "RESERVED",
        "ACPI_RECLAIMABLE",
        "NVS",
        "BADRAM",
    };

    multiboot.with_memory_map([&](auto addr, auto size, auto type) {
        logger_info("Memory map entry: {#x}({}) {}", addr, size, multiboot_memory_type_name[type - 1]);

        if (type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            system::MemoryManager::free_region(addr, size);
        }
    });

    print("hjert kernel v0.0.1\n");
    print("--------------------------------------------------------------------------------\n");
    print("\nSystem halted!\n");

    assert_not_reached();
}