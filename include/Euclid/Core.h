#pragma once

#include <immintrin.h>
#include <type_traits>

#ifdef _MSC_VER
    #define EUCLID_FORCEINLINE [[msvc::forceinline]]
    #define EUCLID_CALL __vectorcall
#else
    #define EUCLID_FORCEINLINE inline __attribute__((always_inline))
    #define EUCLID_CALL
#endif

#ifndef __clang__
    #define EUCLID_CONSTEXPR constexpr
#else
    #define EUCLID_CONSTEXPR
#endif

#define EUCLID_QUALIFIER EUCLID_FORCEINLINE EUCLID_CONSTEXPR
#define EUCLID_SHUFFLE_MASK(x, y, z, w) (((w) << 6) | ((z) << 4) | ((y) << 2) | ((x)))
#define EUCLID_PERMUTE_VEC4(vec, x, y, z, w) _mm_shuffle_ps((vec), (vec), EUCLID_SHUFFLE_MASK((x), (y), (z), (w)))

namespace euclid {

#ifdef _MSC_VER
using Vec4 = __m128;
using Vec8 = __m256;
#else
struct alignas(16) Vec4 final {
    constexpr Vec4() noexcept : v(__m128{ 0, 0, 0, 0 }) {}
    constexpr Vec4(const float x, const float y, const float z, const float w) noexcept : v(__m128{ x,y,z,w }) {}
    constexpr Vec4(const __m128 m) noexcept : v(m) {}
    constexpr operator __m128() const { return v; }
    __m128 v;
};
struct alignas(32) Vec8 final {
    constexpr Vec8() noexcept : v(__m256{ 0, 0, 0, 0, 0, 0, 0, 0 }) {}
    constexpr Vec8(const float a0, const float a1, const float a2, const float a3,
                   const float a4, const float a5, const float a6, const float a7) noexcept : v(__m256{ a0,a1,a2,a3,a4,a5,a6,a7 }) {}
    constexpr Vec8(const __m256 m) noexcept : v(m) {}
    constexpr operator __m256() const { return v; }
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

namespace trait {

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

}
