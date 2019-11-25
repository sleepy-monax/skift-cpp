#pragma once

#include <libsystem/Formattable.h>

namespace system::platform
{

class __packed PlatformContext : public libsystem::Formattable
{
private:
    /* data */
public:
    PlatformContext() {}
    ~PlatformContext() {}
};

} // namespace system::platform
