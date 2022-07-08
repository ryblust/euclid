#pragma once

#include "Vec2.hpp"
#include "Vec4.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4514 5246)
// enable /Wall
// C4514: remove unused inline functions
// C5246: the initialization of a subobject should be wrapped in braces
#endif

namespace euclid {

struct alignas(16) Mat2 {

  static constexpr Mat2 EUCLID_CALL identity() noexcept {
    return { 1,0,0,1 };
  }

  EUCLID_CONSTEXPR float& operator()(std::size_t row, std::size_t col) noexcept {
    return getVec4Data(mat, 2 * row + col);
  }

  EUCLID_CONSTEXPR float operator()(std::size_t row, std::size_t col) const noexcept {
    return const_cast<Mat2*>(this)->operator()(row, col);
  }

  Vec4 mat;
};

EUCLID_QUALIFIER Mat2 EUCLID_CALL transpose(Mat2 m) noexcept {
#ifndef __clang__  
  if consteval {
    return {
      m(0, 0), m(1, 0),
      m(0, 1), m(1, 1)
    };
  }
#endif
  return { _mm_shuffle_ps(m.mat, m.mat, _MM_SHUFFLE(3, 1, 2, 0)) };
}

constexpr Mat2 EUCLID_CALL operator-(Mat2 a) noexcept {
  return { -a.mat };
}

constexpr Mat2 EUCLID_CALL operator+(Mat2 a, Mat2 b) noexcept {
  return { a.mat + b.mat };
}

constexpr Mat2 EUCLID_CALL operator-(Mat2 a, Mat2 b) noexcept {
  return { a.mat - b.mat };
}

EUCLID_QUALIFIER Mat2 EUCLID_CALL operator*(Mat2 a, Mat2 b) noexcept {
  if (__builtin_is_constant_evaluated()) {
    return {
      a(0, 0) * b(0, 0) + a(0, 1) * b(1, 0),
      a(0, 0) * b(0, 1) + a(0, 1) * b(1, 1),
      a(1, 0) * b(0, 0) + a(1, 1) * b(1, 0),
      a(1, 0) * b(1, 0) + a(1, 1) * b(1, 1)
    };
  }
  const __m128 xzxz = _mm_shuffle_ps(b.mat, b.mat, _MM_SHUFFLE(2, 0, 2, 0));
  const __m128 ywyw = _mm_shuffle_ps(b.mat, b.mat, _MM_SHUFFLE(3, 1, 3, 1));
  const __m128 mul1 = _mm_mul_ps(a.mat, xzxz);
  const __m128 mul2 = _mm_mul_ps(a.mat, ywyw);
  const __m128 hadd = _mm_hadd_ps(mul1, mul2);
  return { _mm_shuffle_ps(hadd, hadd, _MM_SHUFFLE(3, 1, 2, 0)) };
}

// EUCLID_QUALIFIER Mat2 EUCLID_CALL operator*(Mat2 a, Vec2 b) noexcept {
//     if (__builtin_is_constant_evaluated()) {
//
//     }
//
// }

constexpr Mat2 EUCLID_CALL operator*(Mat2 a, float v) noexcept {
  return { a.mat * v };
}

constexpr Mat2 EUCLID_CALL operator*(float v, Mat2 a) noexcept {
  return { a.mat * v };
}

constexpr Mat2 EUCLID_CALL operator/(Mat2 a, float v) noexcept {
  return { a.mat / v };
}

constexpr Mat2& EUCLID_CALL operator+=(Mat2& a, Mat2 b) noexcept {
  a.mat += b.mat;
  return a;
}

constexpr Mat2& EUCLID_CALL operator-=(Mat2& a, Mat2 b) noexcept {
  a.mat -= b.mat;
  return a;
}

constexpr Mat2& EUCLID_CALL operator*=(Mat2& a, float v) noexcept {
  a.mat *= v;
  return a;
}

EUCLID_QUALIFIER bool EUCLID_CALL operator==(Mat2 a, Mat2 b) noexcept {
  return a.mat == b.mat;
}

EUCLID_QUALIFIER bool EUCLID_CALL operator!=(Mat2 a, Mat2 b) noexcept {
  return !(a == b);
}

EUCLID_QUALIFIER bool EUCLID_CALL equals(Mat2 a, Mat2 b) noexcept {
  return equals(a.mat, b.mat);
}

} // namespace euclid

#ifdef _MSC_VER
#pragma warning(pop)
#endif