#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

namespace x86
{

void pic_remap();

void pic_ack(int intno);

} // namespace x86
