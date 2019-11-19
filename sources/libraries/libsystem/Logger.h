#pragma once

#include <libsystem/Stdio.h>
#include <libsystem/Format.h>

namespace libsystem
{

enum class LogLevel
{
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
};

#define logger_trace(__args...) log(LogLevel::TRACE, __FILE__, __LINE__, __args)

#define logger_debug(__args...) log(LogLevel::DEBUG, __FILE__, __LINE__, __args)

#define logger_info(__args...) log(LogLevel::INFO, __FILE__, __LINE__, __args)

#define logger_warn(__args...) log(LogLevel::WARN, __FILE__, __LINE__, __args)

#define logger_error(__args...) log(LogLevel::ERROR, __FILE__, __LINE__, __args)

#define logger_fatal(__args...) log(LogLevel::FATAL, __FILE__, __LINE__, __args)

template <typename... Args>
void log(LogLevel level, const char *file, int line, Args... args)
{
    const char *level_names[] = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};
    const char *level_colors[] = {"\e[34m", "\e[36m", "\e[32m", "\e[22m", "\e[31m", "\e[35m"};

    format(*stdlog, "{}{}\e[0m ", level_colors[static_cast<int>(level)], level_names[static_cast<int>(level)]);
    format(*stdlog, "{}:{}: \e[37;1m", file, line);
    format(*stdlog, args...);
    format(*stdlog, "\e[0m\n");
}

} // namespace libsystem
