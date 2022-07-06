#pragma once

#include "Core.h"
#include "Utility.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4514 5246)
// enable /Wall
// C4514: remove unused inline functions
// C5246: the initialization of a subobject should be wrapped in braces
#endif

namespace euclid {

EUCLID_QUALIFIER float EUCLID_CALL getVec4Data(const Vec4& a, std::size_t pos) noexcept {
#if defined(_MSC_VER) && !defined(__clang__)
  return a.m128_f32[pos];
#else
  return a.v[pos];
#endif
}

EUCLID_QUALIFIER float& EUCLID_CALL getVec4Data(Vec4& a, std::size_t pos) noexcept {
#if defined(_MSC_VER) && !defined(__clang__)  
  return a.m128_f32[pos];
#elif __clang__
  return *(reinterpret_cast<float*>(&a) + pos);
#else
  return a.v[pos];
#endif
}

EUCLID_QUALIFIER float EUCLID_CALL getVec4X(Vec4 a) noexcept {
#ifndef __clang__
  if (__builtin_is_constant_evaluated()) {
    return getVec4Data(a, 0);
  }
#endif
  return _mm_cvtss_f32(a);
}

EUCLID_QUALIFIER float EUCLID_CALL getVec4Y(Vec4 a) noexcept {
#ifndef __clang__
  if (__builtin_is_constant_evaluated()) {
    return getVec4Data(a, 1);
  }
#endif
  return _mm_cvtss_f32(_mm_permute_ps(a, _MM_SHUFFLE(1, 1, 1, 1)));
}

EUCLID_QUALIFIER float EUCLID_CALL getVec4Z(Vec4 a) noexcept {
#ifndef __clang__
  if (__builtin_is_constant_evaluated()) {
    return getVec4Data(a, 2);
  }
#endif
  return _mm_cvtss_f32(_mm_permute_ps(a, _MM_SHUFFLE(2, 2, 2, 2)));
}

EUCLID_QUALIFIER float EUCLID_CALL getVec4W(Vec4 a) noexcept {
#ifndef __clang__
  if (__builtin_is_constant_evaluated()) {
    return getVec4Data(a, 3);
  }
#endif
  return _mm_cvtss_f32(_mm_permute_ps(a, _MM_SHUFFLE(3, 3, 3, 3)));
}

// it's recommended to use this function to create
// Vec4 rather than using the list-initialization
// directly return { x,y,z,w } causes perfromance issue 
constexpr Vec4 EUCLID_CALL setVec4(float x, float y, float z, float w) noexcept {
  if (__builtin_is_constant_evaluated()) {
    return { x,y,z,w };
  }
  return { _mm_set_ps(w, z, y, x) };
}

constexpr Vec4 EUCLID_CALL set1Vec4(float v) noexcept {
  if (__builtin_is_constant_evaluated()) {
    return { v,v,v,v };
  }
  return { _mm_set1_ps(v) };
}

constexpr Vec4 EUCLID_CALL setZeroVec4() noexcept {
  if (__builtin_is_constant_evaluated()) {
    return { 0,0,0,0 };
  }
  return { _mm_setzero_ps() };
}

constexpr Vec4 EUCLID_CALL operator+(Vec4 a, Vec4 b) noexcept {
#if defined(__clang__) || defined(__GNUC__)
  return { a.v + b.v };
#else
  if (__builtin_is_constant_evaluated()) {
    return {
      getVec4Data(a, 0) + getVec4Data(b, 0),
      getVec4Data(a, 1) + getVec4Data(b, 1),
      getVec4Data(a, 2) + getVec4Data(b, 2),
      getVec4Data(a, 3) + getVec4Data(b, 3)
    };
  }
  return { _mm_add_ps(a, b) };
#endif
}

constexpr Vec4 EUCLID_CALL operator-(Vec4 a, Vec4 b) noexcept {
#if defined(__clang__) || defined(__GNUC__)
  return { a.v - b.v };
#else
  if (__builtin_is_constant_evaluated()) {
    return {
      getVec4Data(a, 0) - getVec4Data(b, 0),
      getVec4Data(a, 1) - getVec4Data(b, 1),
      getVec4Data(a, 2) - getVec4Data(b, 2),
      getVec4Data(a, 3) - getVec4Data(b, 3)
    };
  }
  return { _mm_sub_ps(a, b) };
#endif
}

constexpr Vec4 EUCLID_CALL operator*(Vec4 a, Vec4 b) noexcept {
#if defined(__clang__) || defined(__GNUC__)
  return { a.v * b.v };
#else
  if (__builtin_is_constant_evaluated()) {
    return {
      getVec4Data(a, 0) * getVec4Data(b, 0),
      getVec4Data(a, 1) * getVec4Data(b, 1),
      getVec4Data(a, 2) * getVec4Data(b, 2),
      getVec4Data(a, 3) * getVec4Data(b, 3)
    };
  }
  return { _mm_mul_ps(a, b) };
#endif
}

constexpr Vec4 EUCLID_CALL operator*(Vec4 a, float v) noexcept {
  return a * set1Vec4(v);
}

constexpr Vec4 EUCLID_CALL operator*(float v, Vec4 a) noexcept {
  return a * v;
}

constexpr Vec4 EUCLID_CALL operator/(Vec4 a, Vec4 b) noexcept {
#if defined(__clang__) || defined(__GNUC__)
  return { a.v / b.v };
#else
  if (__builtin_is_constant_evaluated()) {
    return {
      getVec4Data(a, 0) / getVec4Data(b, 0),
      getVec4Data(a, 1) / getVec4Data(b, 1),
      getVec4Data(a, 2) / getVec4Data(b, 2),
      getVec4Data(a, 3) / getVec4Data(b, 3)
    };
  }
  return { _mm_div_ps(a, b) };
#endif
}

constexpr Vec4 EUCLID_CALL operator/(Vec4 a, float v) noexcept {
  return a / set1Vec4(v);
}

constexpr Vec4 EUCLID_CALL operator-(Vec4 a) noexcept {
  return setZeroVec4() - a;
}

constexpr Vec4& EUCLID_CALL operator+=(Vec4& a, Vec4 b) noexcept {
  a = a + b;
  return a;
}

constexpr Vec4& EUCLID_CALL operator-=(Vec4& a, Vec4 b) noexcept {
  a = a - b;
  return a;
}

constexpr Vec4& EUCLID_CALL operator*=(Vec4& a, Vec4 b) noexcept {
  a = a * b;
  return a;
}

constexpr Vec4& EUCLID_CALL operator*=(Vec4& a, float v) noexcept {
  a = a * v;
  return a;
}

constexpr Vec4& EUCLID_CALL operator/=(Vec4& a, Vec4 b) noexcept {
  a = a / b;
  return a;
}

constexpr Vec4& EUCLID_CALL operator/=(Vec4& a, float v) noexcept {
  a = a / v;
  return a;
}

EUCLID_QUALIFIER bool EUCLID_CALL operator==(Vec4 a, Vec4 b) noexcept {
  if (__builtin_is_constant_evaluated()) {
    return
      getVec4Data(a, 0) == getVec4Data(b, 0) &&
      getVec4Data(a, 1) == getVec4Data(b, 1) &&
      getVec4Data(a, 2) == getVec4Data(b, 2) &&
      getVec4Data(a, 3) == getVec4Data(b, 3);
  }
  return _mm_movemask_ps(_mm_cmpeq_ps(a, b)) == 0xf;
}

EUCLID_QUALIFIER bool EUCLID_CALL operator!=(Vec4 a, Vec4 b) noexcept {
  return !(a == b);
}

EUCLID_QUALIFIER bool EUCLID_CALL equals(Vec4 a, Vec4 b) noexcept {
  if (__builtin_is_constant_evaluated()) {
    return
      detail::floating_point_equals(getVec4Data(a, 0), getVec4Data(b, 0)) &&
      detail::floating_point_equals(getVec4Data(a, 1), getVec4Data(b, 1)) &&
      detail::floating_point_equals(getVec4Data(a, 2), getVec4Data(b, 2)) &&
      detail::floating_point_equals(getVec4Data(a, 3), getVec4Data(b, 3));
  }
  return a == b;
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL clamp(Vec4 v, Vec4 min, Vec4 max) noexcept {
  if (__builtin_is_constant_evaluated()) {
    return {
      detail::clamp(getVec4Data(v, 0), getVec4Data(min, 0), getVec4Data(max, 0)),
      detail::clamp(getVec4Data(v, 1), getVec4Data(min, 1), getVec4Data(max, 1)),
      detail::clamp(getVec4Data(v, 2), getVec4Data(min, 2), getVec4Data(max, 2)),
      detail::clamp(getVec4Data(v, 3), getVec4Data(min, 3), getVec4Data(max, 3))
    };
  }
  return { _mm_max_ps(_mm_min_ps(v, max), min) };
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL clamp(Vec4 v, float min, float max) noexcept {
  return clamp(v, set1Vec4(min), set1Vec4(max));
}

constexpr Vec4 EUCLID_CALL lerp(Vec4 a, Vec4 b, Vec4 t) noexcept {
  if (__builtin_is_constant_evaluated()) {
    #if defined (_MSC_VER) && !defined(__clang__)
      return {
        detail::lerp(getVec4Data(a, 0), getVec4Data(b, 0), getVec4Data(t, 0)),
        detail::lerp(getVec4Data(a, 1), getVec4Data(b, 1), getVec4Data(t, 1)),
        detail::lerp(getVec4Data(a, 2), getVec4Data(b, 2), getVec4Data(t, 2)),
        detail::lerp(getVec4Data(a, 3), getVec4Data(b, 3), getVec4Data(t, 3))
      };
    #else
      return { (b.v - a.v) * t.v + a.v };
    #endif
  }
  return { _mm_fmadd_ps(t, _mm_sub_ps(b, a), a) };
}

constexpr Vec4 EUCLID_CALL lerp(Vec4 a, Vec4 b, float t) noexcept {
  return lerp(a, b, set1Vec4(t));
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL saturate(Vec4 a) noexcept {
  if (__builtin_is_constant_evaluated()) {
    return {
      detail::saturate(getVec4Data(a, 0)),
      detail::saturate(getVec4Data(a, 1)),
      detail::saturate(getVec4Data(a, 2)),
      detail::saturate(getVec4Data(a, 3))
    };
  }
  return { _mm_max_ps(_mm_min_ps(a, _mm_set1_ps(1)), _mm_setzero_ps()) };
}

} // namespace euclid

#ifdef _MSC_VER
#pragma warning(pop)
#endif