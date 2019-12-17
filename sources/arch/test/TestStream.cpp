/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <cassert>

#include <libruntime/Macros.h>
#include <libsystem/Format.h>
#include <libsystem/Stdio.h>

#include "TestFileStream.h"

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
