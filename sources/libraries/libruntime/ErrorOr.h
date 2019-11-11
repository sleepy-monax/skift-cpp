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
    ErrorOr(T value) : _value(value), _error(Error::SUCCEED) {}
    ErrorOr(Error error, T value) : _error(error), _value(value) {}

    bool succeed() { return _error == Error::SUCCEED; }
    T value() { return _value; }
    Error error() { return _error; }
};

} // namespace libruntime
