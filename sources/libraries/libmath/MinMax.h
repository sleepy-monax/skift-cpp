#pragma once

namespace libmath
{

template <typename T>
T min(T a, T b)
{
    return a < b ? a : b;
}

template <typename T>
T max(T a, T b)
{
    return a > b ? a : b;
}

template <typename T>
T clamp(T a, T min_value, T max_value)
{
    return max(min_value, min(max_value, a))
}

} // namespace libmath
