#pragma once

#include <libsystem/Formattable.h>

namespace system::platform
{

class __packed Context : public libsystem::Formattable
{
private:
    /* data */
public:
    Context() {}
    ~Context() {}
};

} // namespace system::platform
