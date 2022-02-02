#pragma once

#include "Core.h"
#include <cstdio>  // waiting for std::print

namespace euclid::debug {

namespace detail {

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4514)
// enable /Wall
// C4514 : Ignore the compiler's warning about removing unused inline functions
#endif

inline void printVecImpl(const vec4i m) noexcept {
#ifdef _MSC_VER
    std::printf("[%d, %d, %d, %d]\n",
        m.m128i_i32[0], m.m128i_i32[1], m.m128i_i32[2], m.m128i_i32[3]);
#else
    const int* p = reinterpret_cast<const int*>(&m);
    std::printf("[%d, %d, %d, %d]\n", *(p), *(p + 1), *(p + 2), *(p + 3));
#endif
}

inline void printVecImpl(const vec4f m) noexcept {
#ifdef _MSC_VER
    std::printf("[%.3f, %.3f, %.3f, %.3f]\n",
        m.m128_f32[0], m.m128_f32[1], m.m128_f32[2], m.m128_f32[3]);
#else
    std::printf("[%.3f, %.3f, %.3f, %.3f]\n", m[0], m[1], m[2], m[3]);
#endif
}

inline void printVecImpl(const vec4d m) noexcept {
#ifdef _MSC_VER
    std::printf("[%.3f, %.3f, %.3f, %.3f]\n",
        m.m256d_f64[0], m.m256d_f64[1], m.m256d_f64[2], m.m256d_f64[3]);
#else
    std::printf("[%.3f, %.3f, %.3f, %.3f]\n", m[0], m[1], m[2], m[3]);
#endif
}

inline void printVecImpl(const vec8i m) noexcept {
#ifdef _MSC_VER
    std::printf("[%d, %d, %d, %d, %d, %d, %d, %d]\n",
        m.m256i_i32[0], m.m256i_i32[1], m.m256i_i32[2], m.m256i_i32[3],
        m.m256i_i32[4], m.m256i_i32[5], m.m256i_i32[6], m.m256i_i32[7]);
#else
    const int* p = reinterpret_cast<const int*>(&m);
    std::printf("[%d, %d, %d, %d, %d, %d, %d, %d]\n",
        *(p),     *(p + 1), *(p + 2), *(p + 3),
        *(p + 4), *(p + 5), *(p + 6), *(p + 7));
#endif
}

inline void printVecImpl(const vec8f m) noexcept {
#ifdef _MSC_VER
    std::printf("[%.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f]\n",
        m.m256_f32[0], m.m256_f32[1], m.m256_f32[2], m.m256_f32[3],
        m.m256_f32[4], m.m256_f32[5], m.m256_f32[6], m.m256_f32[7]);
#else
    std::printf("[%.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f]\n",
        m[0], m[1], m[2], m[3],
        m[4], m[5], m[6], m[7]);
#endif
}

inline void printMatImpl(const mat2i m) noexcept {
    std::printf("%d, %d\n%d, %d\n", m(0, 0), m(0, 1), m(1, 0), m(1, 1));
}

inline void printMatImpl(const mat2f m) noexcept {
    std::printf("%.3f, %.3f\n%.3f, %.3f\n", m(0, 0), m(0, 1), m(1, 0), m(1, 1));
}

inline void printMatImpl(const mat4i m) noexcept {
    std::printf("%d, %d, %d, %d\n%d, %d, %d, %d\n%d, %d, %d, %d\n%d, %d, %d, %d\n",
        m(0, 0), m(0, 1), m(0, 2), m(0, 3),
        m(1, 0), m(1, 1), m(1, 2), m(1, 3),
        m(2, 0), m(2, 1), m(2, 2), m(2, 3),
        m(3, 0), m(3, 1), m(3, 2), m(3, 3)
    );
}

inline void printMatImpl(const mat4f m) noexcept {
    std::printf("%.3f, %.3f, %.3f, %.3f\n%.3f, %.3f, %.3f, %.3f\n%.3f, %.3f, %.3f, %.3f\n%.3f, %.3f, %.3f, %.3f\n",
        m(0, 0), m(0, 1), m(0, 2), m(0, 3),
        m(1, 0), m(1, 1), m(1, 2), m(1, 3),
        m(2, 0), m(2, 1), m(2, 2), m(2, 3),
        m(3, 0), m(3, 1), m(3, 2), m(3, 3)
    );
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

}

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif

template<trait::vector_type T, trait::vector_type... Rest>
void printVec(const T first, const Rest... vec) noexcept {
    detail::printVecImpl(first);
    if constexpr (sizeof...(vec) != 0) {
        debug::printVec(vec...);
    }
}

template<trait::matrix_type T, trait::matrix_type... Rest>
void printMat(const T first, const Rest... mat) noexcept {
    detail::printMatImpl(first);
    if constexpr (sizeof...(mat) != 0) {
        debug::printMat(mat...);
    }
}

template<trait::euclid_component T, trait::euclid_component... Rest>
void print(const T first, const Rest... rest) noexcept {
    if constexpr (trait::vector_type<T>) {
        printVec(first);
    } else {
        printMat(first);
    }
    if constexpr (sizeof...(rest) != 0) {
        print(rest...);
    }
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

}