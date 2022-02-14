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

#define EUCLID_QUALIFIER EUCLID_FORCEINLINE constexpr

namespace euclid {

#ifdef _MSC_VER
    using vec4 = __m128;
    using vec8 = __m256;
#else
    struct alignas(16) vec4 {
        constexpr vec4() noexcept : v(__m128{ 0, 0, 0, 0 }) {}
        constexpr vec4(const float a0, const float a1, const float a2, const float a3)
            noexcept : v(__m128{ a0, a1, a2, a3 }) {}
        constexpr vec4(const __m128 m) noexcept : v(m) {}
        constexpr operator __m128() const { return v; }
        __m128 v;
    };
    struct alignas(32) vec8 {
        constexpr vec8() noexcept : v(__m256{ 0, 0, 0, 0, 0, 0, 0, 0 }) {}
        constexpr vec8(const float a0, const float a1, const float a2, const float a3,
                       const float a4, const float a5, const float a6, const float a7)
            noexcept : v(__m256{ a0, a1, a2, a3, a4, a5, a6, a7 }) {}
        constexpr vec8(const __m256 m) noexcept : v(m) {}
        constexpr operator __m256() const { return v; }
        __m256 v;
    };
#endif

struct vec2;
struct vec3;
struct mat2;
struct mat3;
struct mat4;

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
concept vector_type = is_any_type_of<T, vec2, vec3, vec4, vec8>;

template<typename T>
concept matrix_type = is_any_type_of<T, mat2, mat3, mat4>;

template<typename T>
concept euclid_component = vector_type<T> || matrix_type<T>;

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

}

}