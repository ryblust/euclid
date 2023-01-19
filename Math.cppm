module;
#include "Core.hpp"

#include <bit>
#include <cmath>
#include <limits>
#include <numbers>

export module Euclid.Math;

export namespace Euclid::Math {

template<typename T>
concept arithmetic = std::is_arithmetic_v<T>;

template<typename T>
concept floating_point = std::is_floating_point_v<T>;

template<floating_point FloatT>
inline constexpr auto Radian = std::numbers::pi_v<FloatT> / 180;

// https://en.wikipedia.org/wiki/Fast_inverse_square_root
constexpr float RSqrt(float num) noexcept
{
    const auto x = std::bit_cast<unsigned int>(num);
    const auto y = std::bit_cast<float>(0x5f1ffff9 -  (x >> 1));
    return 0.703952253f * (2.38924456f - num * y * y) * y;
}

constexpr float Sqrt(float num) noexcept
{
    if (std::is_constant_evaluated())
        return RSqrt(num) * num;

    return std::sqrt(num);
}

constexpr float Cos(float angle) noexcept
{
    if (std::is_constant_evaluated())
    {
        return 0;
    }

    return std::cos(angle * Radian<float>);
}

constexpr float Sin(float angle) noexcept
{
    if (std::is_constant_evaluated())
    {
        return 0;
    }

    return std::sin(angle * Radian<float>);
}

constexpr float Tan(float angle) noexcept
{
    if (std::is_constant_evaluated())
    {
        return 0;
    }

    return std::tan(angle * Radian<float>);
}

template<arithmetic Ty>
constexpr Ty Clamp(Ty num, Ty min, Ty max) noexcept
{
    return num < min ? min : max < num ? max : num;
}

template<floating_point FloatT>
constexpr FloatT Lerp(FloatT a, FloatT b, FloatT t) noexcept
{
    return a + t * (b - a);
}

constexpr auto Saturate(arithmetic auto num) noexcept
{
    return num > 1 ? 1 : num < 0 ? 0 : num;
}

constexpr auto Absolute(arithmetic auto num) noexcept
{
    return num < 0 ? -num : num;
}

template<floating_point FloatT>
constexpr bool NearlyEqual(FloatT a, FloatT b) noexcept
{
    return Absolute(a - b) < std::numeric_limits<FloatT>::epsilon();
}

} // namespace Euclid::Math