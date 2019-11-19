#pragma once

namespace libsystem
{

#ifndef assert
#define assert(__expr)
#endif

#define assert_not_reached() \
    do                       \
    {                        \
    } while (1);
} // namespace libsystem
