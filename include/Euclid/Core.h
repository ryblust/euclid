#pragma once

#include <immintrin.h>
#include <type_traits>

#ifdef _MSC_VER
    #define EUCLID_FORCEINLINE [[msvc::forceinline]]
    #define EUCLID_CALL __vectorcall
#else
    #define EUCLID_FORCEINLINE inline __attribute__((always_inline))
    #define EUCLID_CALL
    #define EUCLID_MSVC_SAFEBUFFERS
#endif

#ifndef __clang__
    #define EUCLID_CONSTEXPR constexpr
#else
    #define EUCLID_CONSTEXPR
#endif

#define EUCLID_QUALIFIER EUCLID_FORCEINLINE EUCLID_CONSTEXPR

namespace euclid {

#if defined (_MSC_VER) && !defined(__clang__) 
using Vec4 = __m128;
using Vec8 = __m256;
#else
struct alignas(16) Vec4 final {
    constexpr operator __m128() const noexcept { return v; }
    __m128 v;
};
struct alignas(32) Vec8 final {
    constexpr operator __m256() const noexcept { return v; }
    __m256 v;
};
#endif

struct Vec2;
struct Vec3;
struct Mat2;
struct Mat3;
struct Mat4;

template<typename Ty1, typename Ty2>
concept same_type = std::is_same_v<Ty1, Ty2>;

template<typename Ty>
concept arithmetic = std::is_arithmetic_v<Ty>;

template<typename Ty>
concept floating_point = std::is_floating_point_v<Ty>;

template<typename First, typename... Rest>
concept is_any_type_of = std::disjunction_v<std::is_same<First, Rest>...>;

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif

template<typename T>
concept vector_type = is_any_type_of<T, Vec2, Vec3, Vec4, Vec8>;

template<typename T>
concept matrix_type = is_any_type_of<T, Mat2, Mat3, Mat4>;

template<typename T>
concept euclid_component = vector_type<T> || matrix_type<T>;

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

}