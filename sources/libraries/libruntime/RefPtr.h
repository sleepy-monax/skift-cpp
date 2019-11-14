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

    RefPtr() : _ptr(nullptr) {}
    RefPtr(T &object) : _ptr(const_cast<T *>(&object)) { _ptr->ref(); }
    RefPtr(AdoptTag, T &object) : _ptr(const_cast<T *>(&object)) {}

    RefPtr(RefPtr &other) : _ptr(other.necked()) { _ptr->ref(); }
    RefPtr(AdoptTag, RefPtr &other) : _ptr(other.give_ref()) {}

    RefPtr(RefPtr &&other) : _ptr(other.necked()) { _ptr->ref(); }
    RefPtr(AdoptTag, RefPtr &&other) : _ptr(other.give_ref()) {}

    ~RefPtr()
    {
        if (_ptr)
            _ptr->deref();
    }

    T *operator->()
    {
        return _ptr;
    }

    RefPtr &operator=(RefPtr &&other)
    {
        if (necked() != other.necked())
        {
            if (_ptr)
            {
                necked()->deref();
            }

            if (other.necked() != nullptr)
            {
                other.necked()->ref();
            }

            _ptr = other.necked();
        }

        return *this;
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

    T *necked()
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
    return RefPtr<Type>(adopt(*new Type(args...)));
}

} // namespace libruntime
