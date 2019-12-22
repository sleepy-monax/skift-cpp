#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libruntime/Macros.h>

namespace system
{

void PANIC(const char *message) __noreturn;
void tick();
uint64_t get_tick();

} // namespace system
