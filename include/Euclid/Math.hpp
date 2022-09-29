#pragma once

#include "Core.hpp"
#include <bit>
#include <cmath>
#include <limits>
#include <numbers>

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(push)
#pragma warning(disable: 4514)
#endif // _MSC_VER && !__clang__

namespace euclid::math {

template<floating_point Ty>
inline constexpr Ty radian = std::numbers::pi_v<Ty> / 180;
inline constexpr float radianf = radian<float>;
inline constexpr double radiand = radian<double>;
inline constexpr long double radianld = radian<long double>;

// https://en.wikipedia.org/wiki/Fast_inverse_square_root
constexpr float rsqrt(float num) noexcept {
  auto x = std::bit_cast<unsigned int>(num);
  auto y = std::bit_cast<float>(0x5f1ffff9 -  (x >> 1));
  return y *= 0.703952253f * (2.38924456f - num * y * y);
}

constexpr float sqrt(float num) noexcept {
  if (std::is_constant_evaluated()) {
    return rsqrt(num) * num;
  }
  return std::sqrt(num);
}

constexpr float cos(float angle) noexcept {
  if (std::is_constant_evaluated()) {
    const float first  = angle * radianf * angle * radianf;
    const float second = first * first;
    const float third  = first * second;
    return
      1.f - 0.5f * first + second *
      0.04166666791f - third * 0.001361971023f;
  }
  return std::cos(angle * radianf);
}

constexpr float sin(float angle) noexcept {
  if (std::is_constant_evaluated()) {
    const float first  = angle  * radianf;
    const float square = first  * first;
    const float second = first  * square;
    const float third  = second * square;
    const float forth  = third  * square;
    return
      first - second * 0.1666666716f + 
      third * 0.008333333768f -
      forth * 0.0001984127011f;
  }
  return std::sin(angle * radianf);
}

constexpr float tan(float angle) noexcept {
  if (std::is_constant_evaluated()) {
    const float first  = (90 - angle) * radianf;
    const float square = first  * first;
    const float second = first  * square;
    const float third  = second * square;
    const float forth  = third  * square;
    return
      -(-1.f / first + first / 3.f + second /
      45.f + third * 2.f / 945.f + forth / 4725.f);
  }
  return std::tan(angle * radianf);
}

template<arithmetic T>
constexpr T clamp(T num, T min, T max) noexcept {
  return num < min ? min : max < num ? max : num;
}

template<floating_point T>
constexpr T lerp(T a, T b, T t) noexcept {
  return a + t * (b - a);
}

constexpr auto saturate(arithmetic auto num) noexcept {
  return num > 1 ? 1 : num < 0 ? 0 : num;
}

// until C++23 constexpr std::abs
constexpr auto absolute(arithmetic auto num) noexcept {
  return num < 0 ? -num : num;
}

template<floating_point T>
constexpr bool nearly_equal(T a, T b) noexcept {
  return absolute(a - b) < std::numeric_limits<T>::epsilon();
}

} // namespace euclid::math

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(pop)
#endif // _MSC_VER && !__clang__