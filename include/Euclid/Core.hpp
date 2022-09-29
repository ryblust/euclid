#pragma once

#include <immintrin.h>
#include <type_traits>

#if defined(_MSC_VER) && !defined(__clang__)
  #define EUCLID_FORCEINLINE [[msvc::forceinline]]
#else
  #define EUCLID_FORCEINLINE inline __attribute__((always_inline))
#endif

#ifndef __clang__
  #define EUCLID_CONSTEXPR constexpr
#else
  // clang does not support get __m128's field at compile-time
  #define EUCLID_CONSTEXPR
#endif

#ifdef _MSC_VER
  #define EUCLID_CALL __vectorcall
#else
  #define EUCLID_CALL
#endif

#define EUCLID_QUALIFIER EUCLID_FORCEINLINE EUCLID_CONSTEXPR

#if defined(_MSC_VER) && !defined(__clang__)
  #define EUCLID_UNREACHABLE() __assume(false)
#else
  #define EUCLID_UNREACHABLE() __builtin_unreachable()
#endif

namespace euclid {

#if defined(_MSC_VER) && !defined(__clang__)
using Vec4  = __m128;
using Vec8  = __m256;
using Vec4d = __m256d;
#else
struct alignas(32) Vec8 final {
  constexpr EUCLID_CALL operator __m256() const noexcept {
    return v;
  }
  __m256 v;
};
struct alignas(32) Vec4d final {
  constexpr EUCLID_CALL operator __m256d() const noexcept {
    return v;
  }
  __m256d v;
};
#endif

struct Vec2;
struct Vec3;
struct Vec4;
struct Mat2;
struct Mat3;
struct Mat4;

template<typename T1, typename T2>
concept same_type = std::is_same_v<T1, T2>;

template<typename T>
concept arithmetic = std::is_arithmetic_v<T>;

template<typename T>
concept floating_point = std::is_floating_point_v<T>;

template<typename First, typename... Rest>
concept is_any_type_of = std::disjunction_v<std::is_same<First, Rest>...>;

template<typename T>
concept vector_type = is_any_type_of<std::remove_cv_t<T>, Vec2, Vec3, Vec4, Vec4d, Vec8>;

template<typename T>
concept matrix_type = is_any_type_of<std::remove_cv_t<T>, Mat2, Mat3, Mat4>;

template<typename T>
concept euclid_component = vector_type<T> || matrix_type<T>;

} // namespace euclid