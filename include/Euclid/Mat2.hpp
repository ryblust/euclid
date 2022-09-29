#pragma once

#include "Vec2.hpp"
#include "Vec4.hpp"

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(push)
#pragma warning(disable: 4514 5246)
#endif // _MSC_VER && !__clang__

namespace euclid {

struct alignas(16) Mat2 {
  static constexpr Mat2 EUCLID_CALL identity() noexcept {
    return {{ 1,0,0,1 }};
  }

  EUCLID_CONSTEXPR float& operator()(std::size_t row, std::size_t col) noexcept {
    return getVec4Data(mat, 2 * row + col);
  }

  EUCLID_CONSTEXPR float operator()(std::size_t row, std::size_t col) const noexcept {
    return const_cast<Mat2*>(this)->operator()(row, col);
  }

  Vec4 mat;
};

template<std::size_t Index> requires (Index < 2)
EUCLID_QUALIFIER Vec2 EUCLID_CALL getRowVec2(Mat2 m) noexcept {
  if constexpr (Index == 0) {
    return { getVec4Data(m.mat, 0), getVec4Data(m.mat, 1) };
  } else {
    return { getVec4Data(m.mat, 2), getVec4Data(m.mat, 3) };
  }
}

// make it easy
template<std::size_t Index> requires (Index < 2)
EUCLID_QUALIFIER Vec2 EUCLID_CALL getColVec2(Mat2 m) noexcept {
  if constexpr (Index == 0) {
    return { getVec4Data(m.mat, 0), getVec4Data(m.mat, 2) };
  } else {
    return { getVec4Data(m.mat, 1), getVec4Data(m.mat, 3) };
  }
}

EUCLID_QUALIFIER Vec2 EUCLID_CALL getRowVec2(Mat2 m, std::size_t index) noexcept {
  // comment_assert(index < 2);
  switch (index) {
    case 0: return { getVec4Data(m.mat, 0), getVec4Data(m.mat, 1) };
    case 1: return { getVec4Data(m.mat, 2), getVec4Data(m.mat, 3) };
  }
  EUCLID_UNREACHABLE();
}

EUCLID_QUALIFIER Vec2 EUCLID_CALL getColVec2(Mat2 m, std::size_t index) noexcept {
  // comment_assert(index < 2);
  switch (index) {
    case 0: return { getVec4Data(m.mat, 0), getVec4Data(m.mat, 2) };
    case 1: return { getVec4Data(m.mat, 1), getVec4Data(m.mat, 3) };
  }
  EUCLID_UNREACHABLE();
}

EUCLID_QUALIFIER Mat2 EUCLID_CALL transpose(Mat2 m) noexcept {
#ifndef __clang__
  if (std::is_constant_evaluated()) {
    return {
      m(0, 0), m(1, 0),
      m(0, 1), m(1, 1)
    };
  }
#endif // __clang__
  return { _mm_permute_ps(m.mat, _MM_SHUFFLE(3, 1, 2, 0)) };
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
#ifndef __clang__
  if (std::is_constant_evaluated()) {
    return {
      a(0, 0) * b(0, 0) + a(0, 1) * b(1, 0),
      a(0, 0) * b(0, 1) + a(0, 1) * b(1, 1),
      a(1, 0) * b(0, 0) + a(1, 1) * b(1, 0),
      a(1, 0) * b(1, 0) + a(1, 1) * b(1, 1)
    };
  }
#endif // __clang__
  const __m128 xzxz = _mm_permute_ps(b.mat, _MM_SHUFFLE(2, 0, 2, 0));
  const __m128 ywyw = _mm_permute_ps(b.mat, _MM_SHUFFLE(3, 1, 3, 1));
  const __m128 mul1 = _mm_mul_ps(a.mat, xzxz);
  const __m128 mul2 = _mm_mul_ps(a.mat, ywyw);
  const __m128 hadd = _mm_hadd_ps(mul1, mul2);
  return { _mm_permute_ps(hadd, _MM_SHUFFLE(3, 1, 2, 0)) };
}

EUCLID_QUALIFIER Vec2 EUCLID_CALL operator*(Mat2 a, Vec2 b) noexcept {
#ifndef __clang__
  if (std::is_constant_evaluated()) {
    return {
      a(0, 0) * b.x + a(0, 1) * b.y,
      a(1, 0) * b.x + a(1, 1) * b.y
    };
  }
#endif // __clang__
  Vec2 res;
  const __m128 x  = _mm_set1_ps(b.x);
  const __m128 y  = _mm_set1_ps(b.y);
  const __m128 v1 = _mm_mul_ps(a.mat, x);
  const __m128 v2 = _mm_mul_ps(a.mat, y);
  const __m128 v3 = _mm_permute_ps(v2, _MM_SHUFFLE(2, 3, 0, 1));
  const __m128 v4 = _mm_add_ps(v1, v3);
  detail::storeVec2(res, v4);
  return res;
}

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

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(pop)
#endif // _MSC_VER && !__clang__