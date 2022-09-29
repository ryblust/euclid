#pragma once

#include "Core.hpp"
#include "Math.hpp"

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(push)
#pragma warning(disable: 4514)
#endif // _MSC_VER && !__clang__

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

} // namespace euclid::detail

constexpr Vec2 set1Vec2(float val) noexcept {
  return { val,val };
}

constexpr Vec2 setZeroVec2() noexcept {
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
    math::nearly_equal(a.x, b.x) &&
    math::nearly_equal(a.y, b.y);
}

constexpr Vec2 clamp(Vec2 v, Vec2 min, Vec2 max) noexcept {
  return {
    math::clamp(v.x, min.x, max.x),
    math::clamp(v.y, min.y, max.y)
  };
}

constexpr Vec2 clamp(Vec2 v, float min, float max) noexcept {
  return clamp(v, set1Vec2(min), set1Vec2(max));
}

constexpr Vec2 lerp(Vec2 a, Vec2 b, Vec2 t) noexcept {
  return {
    math::lerp(a.x, b.x, t.x),
    math::lerp(a.y, b.y, t.y)
  };
}

constexpr Vec2 lerp(Vec2 a, Vec2 b, float t) noexcept {
  return lerp(a, b, set1Vec2(t));
}

constexpr Vec2 saturate(Vec2 a) noexcept {
  return {
    math::saturate(a.x),
    math::saturate(a.y)
  };
}

} // namespace euclid

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(pop)
#endif // _MSC_VER && !__clang__