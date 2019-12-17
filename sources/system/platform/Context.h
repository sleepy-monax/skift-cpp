#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libsystem/Formattable.h>

namespace system::platform
{

class Context : public libsystem::Formattable
{
private:
    /* data */
public:
    Context() {}
    ~Context() {}
};

} // namespace system::platform
