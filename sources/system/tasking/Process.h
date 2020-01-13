#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libruntime/LinkedList.h>
#include <libruntime/RefPtr.h>
#include <libruntime/String.h>
#include <libruntime/LinkedList.h>
#include <libsystem/Formattable.h>

#include "system/memory/AddressSpace.h"
#include "system/tasking/Promotion.h"

namespace system::tasking
{

class Process : public libsystem::Formattable,
                public libruntime::RefCounted<Process>
{
private:
    int _id;
    libruntime::String _name;
    Promotion _promotion = Promotion::SUPERVISOR;
    Process *_parent;

    libruntime::OwnPtr<memory::AddressSpace> _address_space;

public:
    int id() { return _id; }
    libruntime::String name() { return _name; }
    Promotion promotion() { return _promotion; }
    memory::AddressSpace &address_space() { return *_address_space; }

    Process(Process *parent, libruntime::String name);
    ~Process();

    libruntime::ErrorOr<size_t> format(libsystem::Stream &stream, libsystem::FormatInfo &info);
};

} // namespace system::tasking
