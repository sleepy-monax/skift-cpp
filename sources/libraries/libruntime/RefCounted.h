#pragma once

#include <libsystem/Assert.h>

namespace libruntime
{

template <typename T>
class RefCounted
{
    // FIXME: make ref and deref atomics

private:
    int _refcount;

public:
    RefCounted()
    {
        _refcount = 1;
    }

    virtual ~RefCounted()
    {
        assert(_refcount == 0);
    }

    void ref()
    {
        assert(_refcount > 0);

        _refcount++;
    }

    void deref()
    {
        assert(_refcount > 0);

        _refcount--;

        if (_refcount == 0)
        {
            delete static_cast<T *>(this);
        }
    }

    int refcount()
    {
        return _refcount;
    }
};

} // namespace libruntime
