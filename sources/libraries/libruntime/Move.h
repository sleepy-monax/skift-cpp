#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

namespace libruntime
{

template <typename T>
T &&move(T &arg)
{
    return static_cast<T &&>(arg);
}

} // namespace libruntime
