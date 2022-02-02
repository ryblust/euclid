#pragma once

#include <immintrin.h>
#include <type_traits>

#ifdef _MSC_VER
    #define EUCLID_FORCEINLINE [[msvc::forceinline]]
    #define EUCLID_CALL __vectorcall
    #define EUCLID_MSVC_CONSTEXPR constexpr
#else
    #define EUCLID_FORCEINLINE inline __attribute__((always_inline))
    #define EUCLID_CALL
    #define EUCLID_MSVC_CONSTEXPR
#endif

// MSVC has better constexpr support for simd intrinsics
#define EUCLID_FUNC_QUALIFIER EUCLID_FORCEINLINE EUCLID_MSVC_CONSTEXPR

#if defined(_MSC_VER) && !defined(EUCLID_NO_GLOBAL_OPERATOR_OVERLOAD) 
    #define EUCLID_HAS_GLOBAL_OPERATOR_OVERLOAD
#endif

namespace euclid {

using vec4i = __m128i;
using vec4f = __m128;
using vec4d = __m256d;
using vec8i = __m256i;
using vec8f = __m256;

struct vec2i;
struct vec2f;
struct vec3i;
struct vec3f;
struct mat2i;
struct mat2f;
struct mat3i;
struct mat3f;
struct mat4i;
struct mat4f;

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
concept vector_type = is_any_type_of<T, vec2i, vec2f, vec3i, vec3f,                    
                                        vec4i, vec4f, vec4d, vec8i, vec8f>;

template<typename T>
concept matrix_type = is_any_type_of<T, mat2i, mat2f, mat3i, mat3f, mat4i, mat4f>;

template<typename T>
concept euclid_component = vector_type<T> || matrix_type<T>;

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

}

}