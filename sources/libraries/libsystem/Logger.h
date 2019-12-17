#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libsystem/Format.h>
#include <libsystem/Stdio.h>

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

#define logger_trace(__args...) libsystem::log(libsystem::LogLevel::TRACE, __FILE__, __LINE__, __args)

#define logger_debug(__args...) libsystem::log(libsystem::LogLevel::DEBUG, __FILE__, __LINE__, __args)

#define logger_info(__args...) libsystem::log(libsystem::LogLevel::INFO, __FILE__, __LINE__, __args)

#define logger_warn(__args...) libsystem::log(libsystem::LogLevel::WARN, __FILE__, __LINE__, __args)

#define logger_error(__args...) libsystem::log(libsystem::LogLevel::ERROR, __FILE__, __LINE__, __args)

#define logger_fatal(__args...) libsystem::log(libsystem::LogLevel::FATAL, __FILE__, __LINE__, __args)

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
