#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libruntime/Types.h>

namespace libsystem
{

uint convert_string_to_uint(const char *str, int base);
int convert_uint_to_string(uint value, char *str, int base);

} // namespace libsystem
