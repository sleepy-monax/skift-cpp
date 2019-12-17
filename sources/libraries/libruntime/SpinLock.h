#pragma once

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
        _locked = false;
        __sync_synchronize();
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
