#include <libruntime/Macros.h>
#include <libsystem/__plugs__.h>
#include <libsystem/Logger.h>

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
