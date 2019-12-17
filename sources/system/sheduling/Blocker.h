#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

namespace system::sheduling
{

class Blocker
{
private:
public:
    Blocker() {}

    virtual ~Blocker() {}

    virtual bool is_block() = 0;

    virtual void unblock() = 0;
};

} // namespace system::sheduling
