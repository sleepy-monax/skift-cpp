#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libsystem/Assert.h>

namespace libruntime
{

template <typename T>
class RefCounted
{
    // FIXME: make ref and deref atomics

private:
    int _refcount;
    bool _orphan;

public:
    RefCounted()
    {
        _refcount = 1;
        _orphan = false;
    }

    virtual ~RefCounted()
    {
        if (!_orphan)
        {
            assert(_refcount == 0);
        }
    }

    // FIXME: Find a better name for this...
    void make_orphan()
    {
        _orphan = true;
    }

    void ref()
    {
        if (!_orphan)
        {
            assert(_refcount > 0);

            _refcount++;
        }
    }

    void deref()
    {
        if (!_orphan)
        {
            assert(_refcount > 0);

            _refcount--;

            if (_refcount == 0)
            {
                delete static_cast<T *>(this);
            }
        }
    }

    int refcount()
    {
        if (!_orphan)
        {
            return _refcount;
        }
        else
        {
            return 0;
        }
    }
};

} // namespace libruntime
