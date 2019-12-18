#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libruntime/LinkedList.h>
#include <libruntime/RefPtr.h>
#include <libruntime/String.h>

#include "system/tasking/Promotion.h"

namespace system::tasking
{

class Process : public libsystem::Formattable, public libruntime::RefCounted<Process>
{
private:
    int _id;
    Promotion _promotion = Promotion::SUPERVISOR;
    Process *_parent;
    libruntime::String _name;

public:
    Process(Process *parent, libruntime::String name);

    ~Process();

    libruntime::ErrorOr<size_t> format(libsystem::Stream &stream, libsystem::FormatInfo &info);

    Promotion promotion() { return _promotion; }
};

} // namespace system::tasking
