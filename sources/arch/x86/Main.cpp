#include <stddef.h>
#include <stdint.h>

#include <libsystem/Format.h>

#include <arch/x86/CGAScreen.h>
#include <arch/x86/SerialStream.h>
#include <arch/x86/TerminalStream.h>

extern "C" void arch_main(void)
{
    auto serial = SerialStream(SerialPort::COM1);
    auto terminal = TerminalStream(CGAScreen((void *)0xB8000));

    while (true)
    {
        libsystem::print(terminal, "Hello world!\n");
        libsystem::print(serial, "Hello world!\n");

        libsystem::print(terminal, "Hello % world!\n", "other");
        libsystem::print(serial, "Hello % world!\n", "other");
    }
}