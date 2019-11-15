#include <libruntime/Types.h>
#include <libsystem/Format.h>

#include <arch/x86/CGAScreen.h>
#include <arch/x86/SerialStream.h>
#include <arch/x86/TerminalStream.h>
#include <arch/x86/Multiboot.h>

using namespace libsystem;

extern "C" void arch_main(u32 mboot_magic, multiboot_info_t *mboot_info)
{
    auto serial = SerialStream(SerialPort::COM1);
    auto terminal = TerminalStream(CGAScreen((void *)0xB8000));

    if (mboot_magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        format(serial, "Does't look good :/");
    }

    format(terminal, "Bootloader is: %\n", reinterpret_cast<const char *>(mboot_info->boot_loader_name));

    switch (mboot_info->framebuffer_type)
    {
    case MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT:
        format(terminal, "Framebuffer is: MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT\n");
        break;

    case MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED:
        format(terminal, "Framebuffer is: MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED\n");
        break;

    case MULTIBOOT_FRAMEBUFFER_TYPE_RGB:
        format(terminal, "Framebuffer is: MULTIBOOT_FRAMEBUFFER_TYPE_RGB\n");
        break;

    default:
        break;
    }

    format(terminal, "hjert kernel\n");
    format(terminal, "--------------------------------------------------------------------------------\n");
    format(terminal, "\n");
    format(terminal, "Hi! I'm the new skiftOS kernel :) \n");

    format(serial, "Hello world!\n");
    format(serial, "Hello % world!\n", "other");
}