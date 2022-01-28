#pragma once

#include "Core.h"
#include <cstdio>

namespace euclid::debug {

namespace detail {

EUCLID_FORCEINLINE void EUCLID_CALL print_i32x4(const vec4i m) noexcept {
#ifdef _MSC_VER
    std::printf("%d, %d, %d, %d\n",
        m.m128i_i32[0], m.m128i_i32[1], m.m128i_i32[2], m.m128i_i32[3]);
#else
    const int* p = reinterpret_cast<const int*>(&m);
    std::printf("%d, %d, %d, %d\n", *(p), *(p + 1), *(p + 2), *(p + 3));
#endif
}

EUCLID_FORCEINLINE void EUCLID_CALL print_f32x4(const vec4f m) noexcept {
#ifdef _MSC_VER
    std::printf("%.3f, %.3f, %.3f, %.3f\n",
        m.m128_f32[0], m.m128_f32[1], m.m128_f32[2], m.m128_f32[3]);
#else
    std::printf("%.3f, %.3f, %.3f, %.3f\n", m[0], m[1], m[2], m[3]);
#endif
}

EUCLID_FORCEINLINE void EUCLID_CALL print_d32x4(const vec4d m) noexcept {
#ifdef _MSC_VER
    std::printf("%.3f, %.3f, %.3f, %.3f\n",
        m.m256d_f64[0], m.m256d_f64[1], m.m256d_f64[2], m.m256d_f64[3]);
#else
    std::printf("%.3f, %.3f, %.3f, %.3f\n", m[0], m[1], m[2], m[3]);
#endif
}

EUCLID_FORCEINLINE void EUCLID_CALL print_i32x8(const vec8i m) noexcept {
#ifdef _MSC_VER
    std::printf("%d, %d, %d, %d, %d, %d, %d, %d\n",
        m.m256i_i32[0], m.m256i_i32[1], m.m256i_i32[2], m.m256i_i32[3],
        m.m256i_i32[4], m.m256i_i32[5], m.m256i_i32[6], m.m256i_i32[7]);
#else
    const int* p = reinterpret_cast<const int*>(&m);
    std::printf("%d, %d, %d, %d, %d, %d, %d, %d\n",
        *(p),     *(p + 1), *(p + 2), *(p + 3),
        *(p + 4), *(p + 5), *(p + 6), *(p + 7));
#endif
}

EUCLID_FORCEINLINE void EUCLID_CALL print_f32x8(const vec8f m) noexcept {
#ifdef _MSC_VER
    std::printf("%.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f\n",
        m.m256_f32[0], m.m256_f32[1], m.m256_f32[2], m.m256_f32[3],
        m.m256_f32[4], m.m256_f32[5], m.m256_f32[6], m.m256_f32[7]);
#else
    std::printf("%.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f\n",
        m[0], m[1], m[2], m[3],
        m[4], m[5], m[6], m[7]);
#endif
}

}

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif

template<vector_type First, vector_type... Rest>
inline void printVec(const First first, const Rest... vec) noexcept {
    if constexpr (same_type<First, vec4i>) {
        detail::print_i32x4(first);
    } else if constexpr (same_type<First, vec4f>) {
        detail::print_f32x4(first);
    } else if constexpr (same_type<First, vec4d>) {
        detail::print_d32x4(first);
    } else if constexpr (same_type<First, vec8i>) {
        detail::print_i32x8(first);
    } else if constexpr (same_type<First, vec8f>) {
        detail::print_f32x8(first);
    }
    if constexpr (sizeof...(vec) != 0) {
        debug::printVec(vec...);
    }
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

}