#pragma once

namespace libmath
{

#define PI (3.141592654)

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

double sqrt(double x);

double mod(double left, double right);

double sin(double x);

double cos(double value);

double tan(double x);

} // namespace libmath
