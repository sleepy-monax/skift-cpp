#include <cassert>
#include "TestFileStream.h"

#include <libsystem/Format.h>
#include <libsystem/Stdio.h>
#include <libruntime/Macros.h>

libsystem::Stream *libsystem::stdin;
libsystem::Stream *libsystem::stdout;
libsystem::Stream *libsystem::stderr;
libsystem::Stream *libsystem::stdlog;

int main(int argc, char const *argv[])
{
    __unused(argc);
    __unused(argv);

    libsystem::stdin = new TestFileStream(stdin);
    libsystem::stdout = new TestFileStream(stdout);
    libsystem::stderr = new TestFileStream(stderr);
    libsystem::stdlog = new TestFileStream(stderr);

    assert(libsystem::format(libsystem::stdout, "Hello, world!\n") == 14);
    assert(libsystem::format(libsystem::stdout, "Hello, {}!\n", "world") == 14);
    assert(libsystem::format(libsystem::stdout, "{}, {}!\n", "Hello", "world") == 14);

    return 0;
}
