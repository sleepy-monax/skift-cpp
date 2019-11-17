#pragma once

namespace libruntime
{

template <typename T>
T &&move(T &arg)
{
    return static_cast<T &&>(arg);
}

} // namespace libruntime
