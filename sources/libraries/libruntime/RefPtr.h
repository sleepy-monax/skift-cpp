#pragma once

#include <libruntime/Macros.h>

namespace libruntime
{

template <typename T>
class RefPtr
{
private:
    T *_ptr;

public:
    enum AdoptTag
    {
        Adopt
    };

    RefPtr(T &object) : _ptr(const_cast<T *>(&object)) { _ptr->ref(); }
    RefPtr(AdoptTag, T &object) : _ptr(const_cast<T *>(&object)) {}

    RefPtr(RefPtr &other) : _ptr(other.ptr()) { _ptr->ref(); }
    RefPtr(AdoptTag, RefPtr &other) : _ptr(other.give_ref()) {}

    RefPtr(RefPtr &&other) : _ptr(other.ptr()) { _ptr->ref(); }
    RefPtr(AdoptTag, RefPtr &&other) : _ptr(other.give_ref()) {}

    ~RefPtr()
    {
        _ptr->deref();
    }

    T *operator->()
    {
        return _ptr;
    }

    int refcount()
    {
        if (_ptr)
        {
            return _ptr->refcount();
        }
        else
        {
            return 0;
        }
    }

    T &give_ref()
    {
        T *ptr = _ptr;
        _ptr = nullptr;
        return *ptr;
    }

    T *ptr()
    {
        return _ptr;
    }
};

template <typename T>
RefPtr<T> adopt(T &object)
{
    return RefPtr<T>(RefPtr<T>::Adopt, object);
}

template <typename Type, typename... Args>
RefPtr<Type> make(Args... args)
{
    RefPtr<Type> refptr(adopt(*new Type(args...)));
    return refptr;
}

} // namespace libruntime
