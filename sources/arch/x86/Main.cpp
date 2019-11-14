#include <libruntime/Types.h>
#include <libsystem/Format.h>

#include <arch/x86/CGAScreen.h>
#include <arch/x86/SerialStream.h>
#include <arch/x86/TerminalStream.h>
#include <arch/x86/Multiboot.h>

using namespace libsystem;

extern "C" void arch_main(u32 mboot_magic, multiboot_info_t *mboot_info)
{
    switch (mboot_info->framebuffer_type)
    {
    case MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT:
        break;

    case MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED:
        break;

    case MULTIBOOT_FRAMEBUFFER_TYPE_RGB:
        break;

    default:
        break;
    }

    auto serial = SerialStream(SerialPort::COM1);
    auto terminal = TerminalStream(CGAScreen((void *)0xB8000));

    print(terminal, "hjert kernel\n");
    print(terminal, "--------------------------------------------------------------------------------\n");
    print(terminal, "\n");
    print(terminal, "Hi! I'm the new skiftOS kernel :) \n");
    print(terminal, reinterpret_cast<const char *>(mboot_info->boot_loader_name));

    print(serial, "Hello world!\n");
    print(serial, "Hello % world!\n", "other");
}