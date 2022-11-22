#pragma once

#include "Math.hpp"

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(push)
#pragma warning(disable: 4514 5246)
#endif // _MSC_VER && !__clang__

namespace euclid {

#if defined(_MSC_VER) && !defined(__clang__)
using Vec4 = __m128;
#else
struct alignas(16) Vec4 final {
  constexpr EUCLID_CALL operator __m128() const noexcept {
    return data;
  }
  __m128 data;
};
#endif // _MSC_VER && !__clang__

EUCLID_QUALIFIER float EUCLID_CALL getVec4Data(const Vec4& a, std::size_t i) noexcept {
#if defined(_MSC_VER) && !defined(__clang__)
  return a.m128_f32[i];
#else // __clang__ || __GNUC__
  return a.data[i];
#endif
}

EUCLID_QUALIFIER float& EUCLID_CALL getVec4Data(Vec4& a, std::size_t i) noexcept {
#if defined(_MSC_VER) && !defined(__clang__)
  return a.m128_f32[i];
#elif __clang__
  return *(reinterpret_cast<float*>(&a) + i);
#else // __GNUC__
  return a.data[i];
#endif
}

EUCLID_QUALIFIER float EUCLID_CALL getVec4X(Vec4 a) noexcept {
#ifndef __clang__
  if (std::is_constant_evaluated()) {
    return getVec4Data(a, 0);
  }
#endif
  return _mm_cvtss_f32(a);
}

EUCLID_QUALIFIER float EUCLID_CALL getVec4Y(Vec4 a) noexcept {
#ifndef __clang__
  if (std::is_constant_evaluated()) {
    return getVec4Data(a, 1);
  }
#endif
  return _mm_cvtss_f32(_mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 1, 1, 1)));
}

EUCLID_QUALIFIER float EUCLID_CALL getVec4Z(Vec4 a) noexcept {
#ifndef __clang__
  if (std::is_constant_evaluated()) {
    return getVec4Data(a, 2);
  }
#endif
  return _mm_cvtss_f32(_mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 2, 2, 2)));
}

EUCLID_QUALIFIER float EUCLID_CALL getVec4W(Vec4 a) noexcept {
#ifndef __clang__
  if (std::is_constant_evaluated()) {
    return getVec4Data(a, 3);
  }
#endif
  return _mm_cvtss_f32(_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 3, 3, 3)));
}

// it's recommended to use `setVec4` to create
// `Vec4` rather than using the list-initialization
constexpr Vec4 EUCLID_CALL setVec4(float x, float y, float z, float w) noexcept {
  if (std::is_constant_evaluated()) {
    return Vec4{ x,y,z,w };
  }
  return { _mm_set_ps(w, z, y, x) };
}

constexpr Vec4 EUCLID_CALL set1Vec4(float v) noexcept {
  if (std::is_constant_evaluated()) {
    return Vec4{ v,v,v,v };
  }
  return { _mm_set1_ps(v) };
}

constexpr Vec4 EUCLID_CALL setZeroVec4() noexcept {
  if (std::is_constant_evaluated()) {
    return Vec4{ 0,0,0,0 };
  }
  return { _mm_setzero_ps() };
}

constexpr Vec4 EUCLID_CALL operator+(Vec4 a, Vec4 b) noexcept {
#if defined(__clang__) || defined(__GNUC__)
  return { a.data + b.data };
#else
  if (std::is_constant_evaluated()) {
    return Vec4 {
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
  return { a.data - b.data };
#else
  if (std::is_constant_evaluated()) {
    return Vec4 {
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
  return { a.data * b.data };
#else
  if (std::is_constant_evaluated()) {
    return Vec4 {
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
  return { a.data / b.data };
#else
  if (std::is_constant_evaluated()) {
    return Vec4 {
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
#ifndef __clang__
  if (std::is_constant_evaluated()) {
    return
      getVec4Data(a, 0) == getVec4Data(b, 0) &&
      getVec4Data(a, 1) == getVec4Data(b, 1) &&
      getVec4Data(a, 2) == getVec4Data(b, 2) &&
      getVec4Data(a, 3) == getVec4Data(b, 3);
  }
#endif
  return _mm_movemask_ps(_mm_cmpeq_ps(a, b)) == 0xf;
}

EUCLID_QUALIFIER bool EUCLID_CALL operator!=(Vec4 a, Vec4 b) noexcept {
  return !(a == b);
}

EUCLID_QUALIFIER bool EUCLID_CALL equals(Vec4 a, Vec4 b) noexcept {
  return
    math::nearly_equal(getVec4Data(a, 0), getVec4Data(b, 0)) &&
    math::nearly_equal(getVec4Data(a, 1), getVec4Data(b, 1)) &&
    math::nearly_equal(getVec4Data(a, 2), getVec4Data(b, 2)) &&
    math::nearly_equal(getVec4Data(a, 3), getVec4Data(b, 3));
}

} // namespace euclid

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(pop)
#endif // _MSC_VER && !__clang__