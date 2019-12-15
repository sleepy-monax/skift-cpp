#pragma once

#include <libruntime/LinkedList.h>
#include <libruntime/RefPtr.h>

#include "system/tasking/Promotion.h"

namespace system::tasking
{

class Process : public libsystem::Formattable, public libruntime::RefCounted<Process>
{
private:
    int _id;
    Promotion _promotion = Promotion::SUPERVISOR;
    Process *_parent;

public:
    Process(Process *parent);

    ~Process();

    libruntime::ErrorOr<size_t> format(libsystem::Stream &stream, libsystem::FormatInfo &info);

    Promotion promotion() { return _promotion; }
};

} // namespace system::tasking
