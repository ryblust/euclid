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

#if defined(_MSC_VER) && !defined(EUCLID_NO_OPERATOR_OVERLOAD) 
    #define EUCLID_HAS_OPERATOR_OVERLOAD
#endif

namespace euclid {

using vec4i = __m128i;
using vec4f = __m128;
using vec4d = __m256d;

using vec8i = __m256i;
using vec8f = __m256;

template<typename Ty1, typename Ty2>
concept same_type = std::is_same_v<Ty1, Ty2>;

template<typename Ty>
concept floating_point = std::is_floating_point_v<Ty>;

template<typename First, typename... Rest>
concept is_any_type_of = std::disjunction_v<std::is_same<First, Rest>...>;

template<typename Ty>
concept euclid_type = is_any_type_of<Ty, int, float>;

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif

template<typename T>
concept vector_type = is_any_type_of<T, vec4i, vec4f, vec4d, vec8i, vec8f>;

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

}