#pragma once

#include "Core.h"
#include <limits>

namespace euclid::detail {

template<arithmetic T>
constexpr T clamp(T val, T min, T max) noexcept {
  return val < min ? min : max < val ? max : val;
}

template<floating_point T>
constexpr T lerp(T a, T b, T t) noexcept {
  return a + t * (b - a);
}

template<arithmetic T>
constexpr T saturate(T val) noexcept {
  return val > 1 ? 1 : val < 0 ? 0 : val;
}

// until C++23 constexpr std::abs
constexpr auto absolute(arithmetic auto value) noexcept {
  return value < 0 ? -value : value;
}

// for internal use
template<floating_point T>
constexpr bool floating_point_equals(T a, T b) noexcept {
  return absolute(a - b) < std::numeric_limits<T>::epsilon();
}

} // namespace euclid::detail