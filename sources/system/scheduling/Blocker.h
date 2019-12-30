#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

namespace system::tasking
{
class Thread;
} // namespace system::tasking

namespace system::scheduling
{

class Blocker
{
private:
public:
    Blocker() {}

    virtual ~Blocker() {}

    virtual bool should_unblock() = 0;

    virtual void unblock() = 0;
};

} // namespace system::scheduling
