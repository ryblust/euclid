#pragma once

#include <bit>
#include <cmath>
#include <limits>
#include <numbers>

namespace euclid::math {

inline constexpr float epsilon = 1.e-8f;
inline constexpr float onediv2pi = 0.159154943f;

// https://en.wikipedia.org/wiki/Fast_inverse_square_root
constexpr float InverseSqrt(float num) noexcept
{
    static_assert(std::numeric_limits<float>::is_iec559);

    const auto x = std::bit_cast<std::uint32_t>(num);
    const auto y = std::bit_cast<float>(0x5f1ffff9 -  (x >> 1));

    return y * (0.703952253f * (2.38924456f - num * y * y));
}

// From DirectXMath::XMScalarCos
constexpr float Cos(float angle) noexcept
{
    return 0; // Not implemented yet.
}

// From DirectXMath::XMScalarSin
constexpr float Sin(float angle) noexcept
{
    return 0; // Not implemented yet.
}

// From DirectXMath::XMScalarTan
constexpr float Tan(float angle) noexcept
{
    return 0; // Not implemented yet.
}

template<typename T> requires std::is_arithmetic_v<T>
constexpr T Lerp(T a, T b, T t) noexcept
{
    return a + t * (b - a);
}

template<typename T> requires std::is_arithmetic_v<T>
constexpr T Clamp(T val, T min, T max) noexcept
{
    return val < min ? min : val < max ? val : max;
}

} // namespace euclid::math