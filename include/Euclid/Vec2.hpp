#pragma once

#include "Core.h"
#include "Utility.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4514)
// enable /Wall
// C4514: remove unused inline functions
#endif

namespace euclid {

struct Vec2 {
  float x, y;
};

namespace detail {

inline __m128 EUCLID_CALL loadVec2(const Vec2& a) noexcept {
  return _mm_castpd_ps(_mm_load_sd(reinterpret_cast<const double*>(&a)));
}

inline void EUCLID_CALL storeVec2(Vec2& dst, __m128 src) noexcept {
  _mm_store_sd(reinterpret_cast<double*>(&dst), _mm_castps_pd(src));
}

}

constexpr Vec2 EUCLID_CALL set1Vec2(const float val) noexcept {
  return { val,val };
}

constexpr Vec2 EUCLID_CALL setZeroVec2() noexcept {
  return { 0,0 };
}

constexpr Vec2 operator+(Vec2 a, Vec2 b) noexcept {
  return { a.x + b.x, a.y + b.y };
}

constexpr Vec2 operator-(Vec2 a, Vec2 b) noexcept {
  return { a.x - b.x, a.y - b.y };
}

constexpr Vec2 operator*(Vec2 a, Vec2 b) noexcept {
  return { a.x * b.x, a.y * b.y };
}

constexpr Vec2 operator/(Vec2 a, Vec2 b) noexcept {
  return { a.x / b.x, a.y / b.y };
}

constexpr Vec2 operator*(Vec2 a, float scale) noexcept {
  return a * set1Vec2(scale);
}

constexpr Vec2 operator*(float scale, Vec2 a) noexcept {
  return a * scale;
}

constexpr Vec2 operator/(Vec2 a, float scale) noexcept {
  return a / set1Vec2(scale);
}

constexpr Vec2 operator-(Vec2 a) noexcept {
  return { -a.x, -a.y };
}

constexpr Vec2& operator+=(Vec2& a, Vec2 b) noexcept {
  a = a + b;
  return a;
}

constexpr Vec2& operator-=(Vec2& a, Vec2 b) noexcept {
  a = a - b;
  return a;
}

constexpr Vec2& operator*=(Vec2& a, Vec2 b) noexcept {
  a = a * b;
  return a;
}

constexpr Vec2& operator*=(Vec2& a, float scale) noexcept {
  a = a * scale;
  return a;
}

constexpr Vec2& operator/=(Vec2& a, Vec2 b) noexcept {
  a = a / b;
  return a;
}

constexpr Vec2& operator/=(Vec2& a, float scale) noexcept {
  a = a / scale;
  return a;
}

constexpr bool operator==(Vec2 a, Vec2 b) noexcept {
  return a.x == b.x && a.y == b.y;
}

constexpr bool operator!=(Vec2 a, Vec2 b) noexcept {
  return !(a == b);
}

constexpr bool equals(Vec2 a, Vec2 b) noexcept {
  return
    detail::floating_point_equals(a.x, b.x) &&
    detail::floating_point_equals(a.y, b.y);
}

constexpr Vec2 clamp(Vec2 v, Vec2 min, Vec2 max) noexcept {
  return {
    detail::clamp(v.x, min.x, max.x),
    detail::clamp(v.y, min.y, max.y)
  };
}

constexpr Vec2 clamp(Vec2 v, float min, float max) noexcept {
  return clamp(v, set1Vec2(min), set1Vec2(max));
}

constexpr Vec2 lerp(Vec2 a, Vec2 b, Vec2 t) noexcept {
  return {
    detail::lerp(a.x, b.x, t.x),
    detail::lerp(a.y, b.y, t.y)
  };
}

constexpr Vec2 lerp(Vec2 a, Vec2 b, float t) noexcept {
  return lerp(a, b, set1Vec2(t));
}

constexpr Vec2 saturate(Vec2 a) noexcept {
  return {
    detail::saturate(a.x),
    detail::saturate(a.y)
  };
}

} // namespace euclid

#ifdef _MSC_VER
#pragma warning(pop)
#endif