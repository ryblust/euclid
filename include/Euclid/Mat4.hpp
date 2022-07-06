#pragma once

#include "Vec8.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4514 5045)
// enable /Wall
// C4514: remove unused inline functions
// C5045: spectre mitigation
#endif

namespace euclid {

struct alignas(32) Mat4 {

  constexpr Mat4() noexcept : mat{} {}

  constexpr Mat4(float a0, float a1, float a2, float a3,
                 float b0, float b1, float b2, float b3,
                 float c0, float c1, float c2, float c3,
                 float d0, float d1, float d2, float d3)
    noexcept : mat{{ a0,a1,a2,a3,b0,b1,b2,b3 }, { c0,c1,c2,c3,d0,d1,d2,d3 }} {}

  constexpr Mat4(Vec4 a, Vec4 b, Vec4 c, Vec4 d)
    noexcept : mat{ toVec8(a, b), toVec8(c, d) } {}

  constexpr Mat4(Vec8 a, Vec8 b)
    noexcept : mat{ a,b } {}

  static constexpr Mat4 EUCLID_CALL identity() noexcept {
    return {
      1, 0, 0, 0,
      0, 1, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1
    };
  }

  EUCLID_CONSTEXPR float& operator()(std::size_t row, std::size_t col) noexcept {
    if (__builtin_is_constant_evaluated()) {
      if (row > 1) {
        return getVec8Data(mat[1], 4 * row + col - 8);
      }
      return getVec8Data(mat[0], 4 * row + col);
    }
    // comment_assert((4 * row + col) < 16);
    return *(reinterpret_cast<float*>(this) + 4 * row + col);
  }

  EUCLID_CONSTEXPR float operator()(std::size_t row, std::size_t col) const noexcept {
    return const_cast<Mat4*>(this)->operator()(row, col);
  }

  EUCLID_CONSTEXPR Vec4 EUCLID_CALL operator[](std::size_t pos) const noexcept {
    // comment_assert(pos < 4);
    switch (pos) {
      case 0: return getVec8LowerVec4(mat[0]);
      case 1: return getVec8UpperVec4(mat[0]);
      case 2: return getVec8LowerVec4(mat[1]);
      case 3: return getVec8UpperVec4(mat[1]);
    }
    // C++23 std::unreachable
    #if defined(_MSC_VER) && !defined(__clang__)
      __assume(false);
    #else
      __builtin_unreachable();
    #endif
  }

  EUCLID_CONSTEXPR Vec4 EUCLID_CALL getColumnVec4(std::size_t pos) const noexcept {
    #ifndef __clang__
      if (__builtin_is_constant_evaluated()) {
        return {
          getVec8Data(mat[0], pos),
          getVec8Data(mat[0], pos + 4),
          getVec8Data(mat[1], pos),
          getVec8Data(mat[1], pos + 4)
        };
      }
    #endif
    // Todo: do not mix sse and avx
    // comment_assert(pos < 4);
    switch (pos) {
      case 0: {
        const __m256 unpackl = _mm256_unpacklo_ps(mat[0], mat[1]);
        const __m128 cast128 = _mm256_castps256_ps128(unpackl);
        const __m128 extract = _mm256_extractf128_ps(unpackl, 1);
        return { _mm_unpacklo_ps(cast128, extract) };
      }
      case 1: {
        const __m256 unpackl = _mm256_unpacklo_ps(mat[0], mat[1]);
        const __m128 cast128 = _mm256_castps256_ps128(unpackl);
        const __m128 extract = _mm256_extractf128_ps(unpackl, 1);
        return { _mm_unpackhi_ps(cast128, extract) };
      }
      case 2: {
        const __m256 unpackh = _mm256_unpackhi_ps(mat[0], mat[1]);
        const __m128 cast128 = _mm256_castps256_ps128(unpackh);
        const __m128 extract = _mm256_extractf128_ps(unpackh, 1);
        return {_mm_unpacklo_ps(cast128, extract)};
      }
      case 3: {
        const __m256 unpackh = _mm256_unpackhi_ps(mat[0], mat[1]);
        const __m128 cast128 = _mm256_castps256_ps128(unpackh);
        const __m128 extract = _mm256_extractf128_ps(unpackh, 1);
        return { _mm_unpackhi_ps(cast128, extract) };
      }
    }
    __builtin_unreachable();
  }

  Vec8 mat[2];
};

constexpr Mat4 EUCLID_CALL setMat4(float a0, float a1, float a2, float a3,
                                   float b0, float b1, float b2, float b3,
                                   float c0, float c1, float c2, float c3,
                                   float d0, float d1, float d2, float d3) noexcept {
  if (__builtin_is_constant_evaluated()) {
    return Mat4 {
      a0, a1, a2, a3,
      b0, b1, b2, b3,
      c0, c1, c2, c3,
      d0, d1, d2, d3
    };
  }
  return {
    { _mm256_set_ps(b3, b2, b1, b0, a3, a2, a1, a0) },
    { _mm256_set_ps(d3, d2, d1, d0, c3, c2, c1, c0) }
  };
}

EUCLID_QUALIFIER Mat4 EUCLID_CALL transpose(Mat4 a) noexcept {
  if (__builtin_is_constant_evaluated()) {
    return {
      a(0,0), a(1,0), a(2,0), a(3,0),
      a(0,1), a(1,1), a(2,1), a(3,1),
      a(0,2), a(1,2), a(2,2), a(3,2),
      a(0,3), a(1,3), a(2,3), a(3,3)
    };
  }
  const __m256i perindex = _mm256_set_epi32(7,3,6,2,5,1,4,0);
  const __m256  unpacklo = _mm256_unpacklo_ps(a.mat[0], a.mat[1]);
  const __m256  unpackhi = _mm256_unpackhi_ps(a.mat[0], a.mat[1]);
  const __m256  permutel = _mm256_permutevar8x32_ps(unpacklo, perindex);
  const __m256  permuteh = _mm256_permutevar8x32_ps(unpackhi, perindex);
  return {{ permutel }, { permuteh }};
}

EUCLID_QUALIFIER Mat4 EUCLID_CALL operator-(Mat4 a) noexcept {
  return { -a.mat[0], -a.mat[1] };
}

EUCLID_QUALIFIER Mat4 EUCLID_CALL operator+(Mat4 a, Mat4 b) noexcept {
  return { a.mat[0] + b.mat[0], a.mat[1] + b.mat[1] };
}

EUCLID_QUALIFIER Mat4 EUCLID_CALL operator-(Mat4 a, Mat4 b) noexcept {
  return { a.mat[0] - b.mat[0], a.mat[1] - b.mat[1] };
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL operator*(Mat4 m, Vec4 v) noexcept {
  if (__builtin_is_constant_evaluated()) {
    float a[4]{};
    for (std::size_t i = 0; i < 4; ++i) {
      a[0] += getVec8Data(m.mat[0], i)     * getVec4Data(v, i);
      a[1] += getVec8Data(m.mat[0], i + 4) * getVec4Data(v, i);
      a[2] += getVec8Data(m.mat[1], i)     * getVec4Data(v, i);
      a[3] += getVec8Data(m.mat[1], i + 4) * getVec4Data(v, i);
    }
    return { a[0],a[1],a[2],a[3] };
  }
  const __m256i mask = _mm256_set_epi32(0,0,0,0,5,1,4,0);
  const __m256  data = _mm256_set_m128(v, v);
  const __m256  mul1 = _mm256_mul_ps(m.mat[0], data);
  const __m256  mul2 = _mm256_mul_ps(m.mat[1], data);
  const __m256  res1 = _mm256_hadd_ps(mul1, mul2);
  const __m256  res2 = _mm256_hadd_ps(res1, res1);
  const __m256  res3 = _mm256_permutevar8x32_ps(res2, mask);
  return { _mm256_castps256_ps128(res3) };
}

EUCLID_QUALIFIER Mat4 EUCLID_CALL operator*(Mat4 a, Mat4 b) noexcept {
  if (__builtin_is_constant_evaluated()) {
    const Vec4 row[4] {
      a * b.getColumnVec4(0),
      a * b.getColumnVec4(1),
      a * b.getColumnVec4(2),
      a * b.getColumnVec4(3)
    };
    return transpose({ toVec8(row[0], row[1]), toVec8(row[2], row[3]) });
  }
  __m256 a0 = _mm256_permute_ps(a.mat[0], 0);
  __m256 a1 = _mm256_permute_ps(a.mat[1], 0);
  __m256 b0 = _mm256_permute2f128_ps(b.mat[0], b.mat[0], 0x00);
  __m256 c0 = _mm256_mul_ps(a0, b0);
  __m256 c1 = _mm256_mul_ps(a1, b0);
  a0 = _mm256_permute_ps(a.mat[0], 85);
  a1 = _mm256_permute_ps(a.mat[1], 85);
  b0 = _mm256_permute2f128_ps(b.mat[0], b.mat[0], 0x11);
  __m256 c2 = _mm256_fmadd_ps(a0, b0, c0);
  __m256 c3 = _mm256_fmadd_ps(a1, b0, c1);
  a0 = _mm256_permute_ps(a.mat[0], 170);
  a1 = _mm256_permute_ps(a.mat[1], 170);
  __m256 b1 = _mm256_permute2f128_ps(b.mat[1], b.mat[1], 0x00);
  __m256 c4 = _mm256_mul_ps(a0, b1);
  __m256 c5 = _mm256_mul_ps(a1, b1);
  a0 = _mm256_permute_ps(a.mat[0], 255);
  a1 = _mm256_permute_ps(a.mat[1], 255);
  b1 = _mm256_permute2f128_ps(b.mat[1], b.mat[1], 0x11);
  __m256 c6 = _mm256_fmadd_ps(a0, b1, c4);
  __m256 c7 = _mm256_fmadd_ps(a1, b1, c5);
  return {{ _mm256_add_ps(c2, c6) }, { _mm256_add_ps(c3, c7) }};
}

EUCLID_QUALIFIER Mat4 EUCLID_CALL operator*(Mat4 a, float v) noexcept {
  return { a.mat[0] * v, a.mat[1] * v };
}

EUCLID_QUALIFIER Mat4 EUCLID_CALL operator*(float v, Mat4 a) noexcept {
  return { a.mat[0] * v, a.mat[1] * v };
}

EUCLID_QUALIFIER Mat4 EUCLID_CALL operator/(Mat4 a, float v) noexcept {
  return { a.mat[0] / v, a.mat[1] / v };
}

EUCLID_QUALIFIER Mat4& EUCLID_CALL operator+=(Mat4& a, Mat4 b) noexcept {
  a = a + b;
  return a;
}

EUCLID_QUALIFIER Mat4& EUCLID_CALL operator-=(Mat4& a, Mat4 b) noexcept {
  a = a - b;
  return a;
}

EUCLID_QUALIFIER Mat4& EUCLID_CALL operator*=(Mat4& a, float v) noexcept {
  a = a * v;
  return a;
}

EUCLID_QUALIFIER bool EUCLID_CALL operator==(Mat4 a, Mat4 b) noexcept {
  return a.mat[0] == b.mat[0] && a.mat[1] == b.mat[1];
}

EUCLID_QUALIFIER bool EUCLID_CALL operator!=(Mat4 a, Mat4 b) noexcept {
  return !(a == b);
}

EUCLID_QUALIFIER bool EUCLID_CALL equals(Mat4 a, Mat4 b) noexcept {
  return equals(a.mat[0], b.mat[0]) && equals(a.mat[1], b.mat[1]);
}

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif