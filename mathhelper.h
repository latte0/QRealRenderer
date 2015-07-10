#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <cstdint>
#include <cfloat>
#include <type_traits>

namespace RegenUtil{

template <typename T>
struct MathConstants;

static constexpr int One = 1;

template<>
struct MathConstants<float>
{
    static constexpr float Pi()         { return 3.1415926535f; }
    static constexpr float TwoPi()      { return 6.2831853071f; }
    static constexpr float OneOverPi()  { return 0.3183098861f; }
    static constexpr float OneOver2Pi() { return 0.1591549430f; }
    static constexpr float PiOver2()    { return 1.5707963267f; }
    static constexpr float PiOver4()    { return 0.7853981633f; }
};

template <>
struct MathConstants<double>
{
    static constexpr double Pi()         { return 3.14159265358979323846; }
    static constexpr double TwoPi()      { return 6.28318530717958647692; }
    static constexpr double OneOverPi()  { return 0.31830988618379067154; }
    static constexpr double OneOver2Pi() { return 0.15915494309189533576; }
    static constexpr double PiOver2()    { return 1.57079632679489661923; }
    static constexpr double PiOver4()    { return 0.78539816339744830962; }
};

template <typename T>
T Clamp(T const& x, T const& min, T const& max)
{
    if (x < min)  return min;
    if (x > max)  return max;
    return x;
}


template <typename T>
T Saturate(T const& x)
{
    return Clamp(x, T{0}, T{1});
}


template <typename T>
T Lerp(T const& source1, T const& source2, T const& amount)
{
    return source1 + amount * (source2 - source1);
}


template <typename T>
T SmoothStep(T const& min, T const& max, T const& amount)
{
    auto x = Saturate(amount);
    auto scale = x * x * (T{3} - T{2} * x);
    return min + scale * (max - min);
}

template <typename T>
T ToRad(T const& degrees)
{
    constexpr auto scaleFactor =  MathConstants<T>::Pi() / 180;
    return degrees * scaleFactor;
}

template <typename T>
T ToDeg(T const& radians)
{
    constexpr auto scaleFactor = 180 / MathConstants<T>::Pi();
    return radians * scaleFactor;
}


}

#endif // MATHHELPER_H
