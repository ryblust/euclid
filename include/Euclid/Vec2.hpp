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

EUCLID_FORCEINLINE __m128 EUCLID_CALL loadVec2(const Vec2& a) noexcept {
  return _mm_castpd_ps(_mm_load_sd(reinterpret_cast<const double*>(&a)));
}

EUCLID_FORCEINLINE void EUCLID_CALL storeVec2(Vec2& dst, const __m128 src) noexcept {
  _mm_store_sd(reinterpret_cast<double*>(&dst), _mm_castps_pd(src));
}

#ifdef _MSC_VER
  using Vec2ArgType = const Vec2&;
#else
  using Vec2ArgType = const Vec2;
#endif

}

constexpr Vec2 EUCLID_CALL set1Vec2(const float val) noexcept {
  return { val,val };
}

constexpr Vec2 EUCLID_CALL setZeroVec2() noexcept {
  return { 0,0 };
}

constexpr Vec2 operator+(detail::Vec2ArgType a, detail::Vec2ArgType b) noexcept {
#ifdef _MSC_VER
  if (__builtin_is_constant_evaluated()) {
#endif
    return { a.x + b.x, a.y + b.y };
#ifdef _MSC_VER
  }
  Vec2 res;
  const __m128 v1 = detail::loadVec2(a);
  const __m128 v2 = detail::loadVec2(b);
  const __m128 v3 = _mm_add_ps(v1, v2);
  detail::storeVec2(res, v3);
  return res;
#endif
}

constexpr Vec2 operator-(detail::Vec2ArgType a, detail::Vec2ArgType b) noexcept {
#ifdef _MSC_VER
  if (__builtin_is_constant_evaluated()) {
#endif
    return { a.x - b.x, a.y - b.y };
#ifdef _MSC_VER
  }
  Vec2 res;
  const __m128 v1 = detail::loadVec2(a);
  const __m128 v2 = detail::loadVec2(b);
  const __m128 v3 = _mm_sub_ps(v1, v2);
  detail::storeVec2(res, v3);
  return res;
#endif
}

constexpr Vec2 operator*(detail::Vec2ArgType a, detail::Vec2ArgType b) noexcept {
#ifdef _MSC_VER
  if (__builtin_is_constant_evaluated()) {
#endif
    return { a.x * b.x, a.y * b.y };
#ifdef _MSC_VER
  }
  Vec2 res;
  const __m128 v1 = detail::loadVec2(a);
  const __m128 v2 = detail::loadVec2(b);
  const __m128 v3 = _mm_mul_ps(v1, v2);
  detail::storeVec2(res, v3);
  return res;
#endif
}

constexpr Vec2 operator/(detail::Vec2ArgType a, detail::Vec2ArgType b) noexcept {
#ifdef _MSC_VER
  if (__builtin_is_constant_evaluated()) {
#endif
    return { a.x / b.x, a.y / b.y };
#ifdef _MSC_VER
  }
  Vec2 res;
  const __m128 v1 = detail::loadVec2(a);
  const __m128 v2 = detail::loadVec2(b);
  const __m128 v3 = _mm_div_ps(v1, v2);
  detail::storeVec2(res, v3);
  return res;
#endif
}

constexpr Vec2 operator*(detail::Vec2ArgType a, const float scale) noexcept {
  return a * set1Vec2(scale);
}

constexpr Vec2 operator*(const float scale, detail::Vec2ArgType a) noexcept {
  return a * scale;
}

constexpr Vec2 operator/(detail::Vec2ArgType a, const float scale) noexcept {
  return a / set1Vec2(scale);
}

constexpr Vec2 operator-(detail::Vec2ArgType a) noexcept {
#ifdef _MSC_VER
  if (__builtin_is_constant_evaluated()) {
#endif
    return { -a.x, -a.y };
#ifdef _MSC_VER
  }
  Vec2 res;
  const __m128 v1 = detail::loadVec2(a);
  const __m128 v2 = _mm_setzero_ps();
  const __m128 v3 = _mm_sub_ps(v2, v1);
  detail::storeVec2(res, v3);
  return res;
#endif
}

constexpr Vec2& operator+=(Vec2& a, detail::Vec2ArgType b) noexcept {
  a = a + b;
  return a;
}

constexpr Vec2& operator-=(Vec2& a, detail::Vec2ArgType b) noexcept {
  a = a - b;
  return a;
}

constexpr Vec2& operator*=(Vec2& a, detail::Vec2ArgType b) noexcept {
  a = a * b;
  return a;
}

constexpr Vec2& operator*=(Vec2& a, const float scale) noexcept {
  a = a * scale;
  return a;
}

constexpr Vec2& operator/=(Vec2& a, detail::Vec2ArgType b) noexcept {
  a = a / b;
  return a;
}

constexpr Vec2& operator/=(Vec2& a, const float scale) noexcept {
  a = a / scale;
  return a;
}

constexpr bool operator==(detail::Vec2ArgType a, detail::Vec2ArgType b) noexcept {
#ifdef _MSC_VER
  if (__builtin_is_constant_evaluated()) {
#endif
    return a.x == b.x && a.y == b.y;
#ifdef _MSC_VER
  }
  const __m128 v1 = detail::loadVec2(a);
  const __m128 v2 = detail::loadVec2(b);
  return _mm_movemask_ps(_mm_cmpeq_ps(v1, v2)) == 0xf;
#endif
}

constexpr bool operator!=(detail::Vec2ArgType a, detail::Vec2ArgType b) noexcept {
  return !(a == b);
}

constexpr bool equals(detail::Vec2ArgType a, detail::Vec2ArgType b) noexcept {
  if (__builtin_is_constant_evaluated()) {
    return
      detail::floating_point_equals(a.x, b.x) &&
      detail::floating_point_equals(a.y, b.y);
  }
  return a == b;
}

constexpr Vec2 clamp(detail::Vec2ArgType v, detail::Vec2ArgType min, detail::Vec2ArgType max) noexcept {
  if (__builtin_is_constant_evaluated()) {
    return {
      detail::clamp(v.x, min.x, max.x),
      detail::clamp(v.y, min.y, max.y)
    };
  }
#ifdef _MSC_VER
  Vec2 res;
  const __m128 v1 = detail::loadVec2(v);
  const __m128 v2 = detail::loadVec2(min);
  const __m128 v3 = detail::loadVec2(max);
  const __m128 v4 = _mm_max_ps(_mm_min_ps(v1, v3), v2);
  detail::storeVec2(res, v4);
  return res;
#else
  Vec2 res;
  asm("minps xmm0 xmm1\n\t" "maxps xmm0 xmm2" : "=x"(res));
  return res;
#endif
}

constexpr Vec2 clamp(detail::Vec2ArgType v, const float min, const float max) noexcept {
  return clamp(v, set1Vec2(min), set1Vec2(max));
}

constexpr Vec2 lerp(detail::Vec2ArgType a, detail::Vec2ArgType b, detail::Vec2ArgType t) noexcept {
  if (__builtin_is_constant_evaluated()) {
    return {
      detail::lerp(a.x, b.x, t.x),
      detail::lerp(a.y, b.y, t.y)
    };
  }
  Vec2 res;
  const __m128 v1 = detail::loadVec2(a);
  const __m128 v2 = detail::loadVec2(b);
  const __m128 v3 = detail::loadVec2(t);
  const __m128 v4 = _mm_fmadd_ps(v3, _mm_sub_ps(v2, v1), v1);
  detail::storeVec2(res, v4);
  return res;
}

constexpr Vec2 lerp(detail::Vec2ArgType a, detail::Vec2ArgType b, const float t) noexcept {
  return lerp(a, b, set1Vec2(t));
}

constexpr Vec2 saturate(detail::Vec2ArgType a) noexcept {
  if (__builtin_is_constant_evaluated()) {
    return {
      detail::saturate(a.x),
      detail::saturate(a.y)
    };
  }
  Vec2 res;
  const __m128 v1 = detail::loadVec2(a);
  const __m128 v2 = _mm_max_ps(_mm_min_ps(v1, _mm_set1_ps(1)), _mm_setzero_ps());
  detail::storeVec2(res, v2);
  return res;
}

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif