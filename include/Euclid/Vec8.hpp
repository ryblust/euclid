#pragma once

#include "Vec4.hpp"

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(push)
#pragma warning(disable: 4514 5246)
#endif // _MSC_VER && !__clang__

namespace euclid {

#if defined(_MSC_VER) && !defined(__clang__)
using Vec8 = __m256;
#else
struct alignas(32) Vec8 final {
  constexpr EUCLID_CALL operator __m256() const noexcept {
    return data;
  }
  __m256 data;
};
#endif // _MSC_VER && !__clang__

EUCLID_QUALIFIER float getVec8Data(const Vec8& a, std::size_t i) noexcept {
#if defined(_MSC_VER) && !defined(__clang__)
  return a.m256_f32[i];
#else // __clang__ || __GNUC__
  return a.data[i];
#endif
}

EUCLID_QUALIFIER float& getVec8Data(Vec8& a, std::size_t i) noexcept {
#if defined(_MSC_VER) && !defined(__clang__)
  return a.m256_f32[i];
#elif __clang__
  return *(reinterpret_cast<float*>(&a) + i);
#else // __GNUC__
  return a.data[i];
#endif
}

// it's recommended to use this function to create
// Vec8 rather than using the list-initialization
constexpr Vec8 EUCLID_CALL setVec8(float x, float y, float z, float w, float a, float b, float c, float d) noexcept {
  if (std::is_constant_evaluated()) {
    return Vec8{ x,y,z,w,a,b,c,d };
  }
  return { _mm256_set_ps(d,c,b,a,w,z,y,x) };
}

constexpr Vec8 EUCLID_CALL set1Vec8(float v) noexcept {
  if (std::is_constant_evaluated()) {
    return Vec8{ v,v,v,v,v,v,v,v };
  }
  return { _mm256_set1_ps(v) };
}

constexpr Vec8 EUCLID_CALL setZeroVec8() noexcept {
  if (std::is_constant_evaluated()) {
    return Vec8{ 0,0,0,0,0,0,0,0 };
  }
  return { _mm256_setzero_ps() };
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL getVec8LowerVec4(Vec8 a) noexcept {
#ifndef __clang__
  if (std::is_constant_evaluated()) {
    return Vec4 { 
      getVec8Data(a, 0),
      getVec8Data(a, 1),
      getVec8Data(a, 2),
      getVec8Data(a, 3)
    };
  }
#endif // __clang__
  return { _mm256_extractf128_ps(a, 0) };
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL getVec8UpperVec4(Vec8 a) noexcept {
#ifndef __clang__
  if (std::is_constant_evaluated()) {
    return Vec4 {
      getVec8Data(a, 4),
      getVec8Data(a, 5),
      getVec8Data(a, 6),
      getVec8Data(a, 7)
    };
  }
#endif // __clang__
  return { _mm256_extractf128_ps(a, 1) };
}

EUCLID_QUALIFIER Vec8 EUCLID_CALL toVec8(Vec4 a, Vec4 b) noexcept {
#ifndef __clang__
  if (std::is_constant_evaluated()) {
    return Vec8 {
      getVec4Data(a, 0), getVec4Data(a, 1),
      getVec4Data(a, 2), getVec4Data(a, 3),
      getVec4Data(b, 0), getVec4Data(b, 1),
      getVec4Data(b, 2), getVec4Data(b, 3)
    };
  }
#endif // __clang__
  return { _mm256_set_m128(a, b) };
}

constexpr Vec8 EUCLID_CALL operator+(Vec8 a, Vec8 b) noexcept {
#if defined(__clang__) || defined(__GNUC__)
  return { a.data + b.data };
#else
  if (std::is_constant_evaluated()) {
    return Vec8 {
      getVec8Data(a, 0) + getVec8Data(b, 0),
      getVec8Data(a, 1) + getVec8Data(b, 1),
      getVec8Data(a, 2) + getVec8Data(b, 2),
      getVec8Data(a, 3) + getVec8Data(b, 3),
      getVec8Data(a, 4) + getVec8Data(b, 4),
      getVec8Data(a, 5) + getVec8Data(b, 5),
      getVec8Data(a, 6) + getVec8Data(b, 6),
      getVec8Data(a, 7) + getVec8Data(b, 7)
    };
  }
  return { _mm256_add_ps(a, b) };
#endif // __clang__ || __GNUC__
}

EUCLID_QUALIFIER Vec8 EUCLID_CALL operator-(Vec8 a, Vec8 b) noexcept {
#if defined(__clang__) || defined(__GNUC__)
  return { a.data - b.data };
#else
  if (std::is_constant_evaluated()) {
    return {
      getVec8Data(a, 0) - getVec8Data(b, 0),
      getVec8Data(a, 1) - getVec8Data(b, 1),
      getVec8Data(a, 2) - getVec8Data(b, 2),
      getVec8Data(a, 3) - getVec8Data(b, 3),
      getVec8Data(a, 4) - getVec8Data(b, 4),
      getVec8Data(a, 5) - getVec8Data(b, 5),
      getVec8Data(a, 6) - getVec8Data(b, 6),
      getVec8Data(a, 7) - getVec8Data(b, 7)
    };
  }
  return { _mm256_sub_ps(a, b) };
#endif // __clang__ || __GNUC__
}

EUCLID_QUALIFIER Vec8 EUCLID_CALL operator*(Vec8 a, Vec8 b) noexcept {
#if defined(__clang__) || defined(__GNUC__)
  return { a.data * b.data };
#else
  if (std::is_constant_evaluated()) {
    return {
      getVec8Data(a, 0) * getVec8Data(b, 0),
      getVec8Data(a, 1) * getVec8Data(b, 1),
      getVec8Data(a, 2) * getVec8Data(b, 2),
      getVec8Data(a, 3) * getVec8Data(b, 3),
      getVec8Data(a, 4) * getVec8Data(b, 4),
      getVec8Data(a, 5) * getVec8Data(b, 5),
      getVec8Data(a, 6) * getVec8Data(b, 6),
      getVec8Data(a, 7) * getVec8Data(b, 7)
    };
  }
  return { _mm256_mul_ps(a, b) };
#endif // __clang__ || __GNUC__
}

EUCLID_QUALIFIER Vec8 EUCLID_CALL operator*(Vec8 a, float v) noexcept {
  return a * set1Vec8(v);
}

EUCLID_QUALIFIER Vec8 EUCLID_CALL operator*(float v, Vec8 a) noexcept {
  return a * v;
}

EUCLID_QUALIFIER Vec8 EUCLID_CALL operator/(Vec8 a, Vec8 b) noexcept {
#if defined(__clang__) || defined(__GNUC__)
  return { a.data / b.data };
#else
  if (std::is_constant_evaluated()) {
    return {
      getVec8Data(a, 0) / getVec8Data(b, 0),
      getVec8Data(a, 1) / getVec8Data(b, 1),
      getVec8Data(a, 2) / getVec8Data(b, 2),
      getVec8Data(a, 3) / getVec8Data(b, 3),
      getVec8Data(a, 4) / getVec8Data(b, 4),
      getVec8Data(a, 5) / getVec8Data(b, 5),
      getVec8Data(a, 6) / getVec8Data(b, 6),
      getVec8Data(a, 7) / getVec8Data(b, 7)
    };
  }
  return { _mm256_div_ps(a, b) };
#endif // __clang__ || __GNUC__
}

EUCLID_QUALIFIER Vec8 EUCLID_CALL operator/(Vec8 a, float v) noexcept {
  return a / set1Vec8(v);
}

EUCLID_QUALIFIER Vec8 EUCLID_CALL operator-(Vec8 a) noexcept {
  return setZeroVec8() - a;
}

EUCLID_QUALIFIER Vec8& EUCLID_CALL operator+=(Vec8& a, Vec8 b) noexcept {
  a = a + b;
  return a;
}

EUCLID_QUALIFIER Vec8& EUCLID_CALL operator-=(Vec8& a, Vec8 b) noexcept {
  a = a - b;
  return a;
}

EUCLID_QUALIFIER Vec8& EUCLID_CALL operator*=(Vec8& a, Vec8 b) noexcept {
  a = a * b;
  return a;
}

EUCLID_QUALIFIER Vec8& EUCLID_CALL operator*=(Vec8& a, float v) noexcept {
  a = a * v;
  return a;
}

EUCLID_QUALIFIER Vec8& EUCLID_CALL operator/=(Vec8& a, Vec8 b) noexcept {
  a = a / b;
  return a;
}

EUCLID_QUALIFIER Vec8& EUCLID_CALL operator/=(Vec8& a, float v) noexcept {
  a = a / v;
  return a;
}

EUCLID_QUALIFIER bool EUCLID_CALL operator==(Vec8 a, Vec8 b) noexcept {
#ifndef __clang__
  if (std::is_constant_evaluated()) {
    return
      getVec8Data(a, 0) == getVec8Data(b, 0) && 
      getVec8Data(a, 1) == getVec8Data(b, 1) &&
      getVec8Data(a, 2) == getVec8Data(b, 2) &&
      getVec8Data(a, 3) == getVec8Data(b, 3) &&
      getVec8Data(a, 4) == getVec8Data(b, 4) &&
      getVec8Data(a, 5) == getVec8Data(b, 5) &&
      getVec8Data(a, 6) == getVec8Data(b, 6) &&
      getVec8Data(a, 7) == getVec8Data(b, 7);
  }
#endif // __clang__
  return _mm256_movemask_ps(_mm256_cmp_ps(a, b, 0)) == 0xff;
}

EUCLID_QUALIFIER bool EUCLID_CALL operator!=(Vec8 a, Vec8 b) noexcept {
  return !(a == b);
}

EUCLID_QUALIFIER bool EUCLID_CALL equals(Vec8 a, Vec8 b) noexcept {
  return
    math::nearly_equal(getVec8Data(a, 0), getVec8Data(b, 0)) &&
    math::nearly_equal(getVec8Data(a, 1), getVec8Data(b, 1)) &&
    math::nearly_equal(getVec8Data(a, 2), getVec8Data(b, 2)) &&
    math::nearly_equal(getVec8Data(a, 3), getVec8Data(b, 3)) &&
    math::nearly_equal(getVec8Data(a, 4), getVec8Data(b, 4)) &&
    math::nearly_equal(getVec8Data(a, 5), getVec8Data(b, 5)) &&
    math::nearly_equal(getVec8Data(a, 6), getVec8Data(b, 6)) &&
    math::nearly_equal(getVec8Data(a, 7), getVec8Data(b, 7));
}

} // namespace euclid

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(pop)
#endif // _MSC_VER && !__clang__