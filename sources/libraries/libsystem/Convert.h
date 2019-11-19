#pragma once

#include <libruntime/Types.h>

namespace libsystem
{

uint convert_string_to_uint(const char *str, int base);
int convert_uint_to_string(uint value, char *str, int base);

} // namespace libsystem
