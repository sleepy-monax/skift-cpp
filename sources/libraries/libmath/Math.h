#pragma once

namespace libmath
{

template <typename T, typename U>
T lerp(T from, T to, U transition)
{
    return from + (to - from) * transition;
}

template <typename T>
T abs(T value)
{
    return value < 0 ? -value : value;
}

} // namespace libmath
