#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

namespace libruntime
{

class SpinLock
{
private:
    volatile bool _locked = false;

public:
    SpinLock() {}
    ~SpinLock() {}

    void acquire()
    {
        while (!__sync_bool_compare_and_swap(&_locked, false, true))
        {
            //FIXME: don't burn  the CPU
        }

        __sync_synchronize();
    }

    void release()
    {
        __sync_synchronize();
        _locked = false;
    }

    bool is_acquired()
    {
        return _locked;
    }

    bool try_acquire()
    {
        if (__sync_bool_compare_and_swap(&_locked, 0, 1))
        {
            __sync_synchronize();
            return true;
        }
        else
        {
            return false;
        }
    }
};

} // namespace libruntime
