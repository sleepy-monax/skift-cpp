#pragma once

#include <libruntime/Error.h>

namespace libruntime
{

template <typename T>
class ErrorOr
{
private:
    Error _error;
    T _value;

public:
    ErrorOr(Error error) : _error(error) {}

    ErrorOr(T value) : _error(Error::SUCCEED), _value(value) {}

    ErrorOr(Error error, T value) : _error(error), _value(value) {}

    template <typename U>
    ErrorOr(ErrorOr<U> error, T value) : _error(error.error()), _value(value) {}

    bool succeed() { return _error == Error::SUCCEED; }

    T value() { return _value; }

    Error error() { return _error; }

    bool operator==(Error err)
    {
        return _error == err;
    }

    bool operator!=(Error err)
    {
        return _error != err;
    }

    bool operator==(T value)
    {
        return _value == value;
    }

    bool operator!=(T value)
    {
        return _value != value;
    }
};

} // namespace libruntime
