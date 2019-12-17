/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libruntime/Macros.h>
#include <libsystem/Logger.h>
#include <libsystem/__plugs__.h>

void libsystem::__assert_failled(const char *file, int line, const char *expr)
{
    libsystem::log(libsystem::LogLevel::FATAL, file, line, "Assert \"{}\" failled!", expr);

    __plugs__::assert_failled();
}

void libsystem::__assert_not_reached_reached(const char *file, int line)
{
    libsystem::log(libsystem::LogLevel::FATAL, file, line, "Assert_not_reached() reached!");

    __plugs__::assert_not_reached_reached();
}
