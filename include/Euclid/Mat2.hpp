#pragma once

#include "Vec4.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4514)
// enable /Wall
// C4514 : Ignore the compiler's warning about removing unused inline functions
#endif

namespace euclid {

struct alignas(16) mat2 {
    using value_type = float;

    static constexpr mat2 EUCLID_CALL identity() noexcept {
        return { { 1,0,0,1 } };
    }

    constexpr float& operator()(const std::size_t row, const std::size_t col) noexcept {
        return getVec4RefData(mat, 2 * row + col);
    }

    constexpr float  operator()(const std::size_t row, const std::size_t col) const noexcept {
        return getVec4Data(mat, 2 * row + col);
    }

    vec4 mat;
};

EUCLID_QUALIFIER mat2 EUCLID_CALL transpose(const mat2 m) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return { 
            { m(0, 0), m(1, 0),
              m(0, 1), m(1, 1) }
        };
    }
    return { _mm_shuffle_ps(m.mat, m.mat, 0b11011000) };
}

EUCLID_QUALIFIER mat2 EUCLID_CALL operator+(const mat2 a, const mat2 b) noexcept {
    return { a.mat + b.mat };
}

EUCLID_QUALIFIER mat2 EUCLID_CALL operator-(const mat2 a, const mat2 b) noexcept {
    return { a.mat - b.mat };
}

EUCLID_QUALIFIER mat2 EUCLID_CALL operator*(const mat2 a, const mat2 b) noexcept {
    if (__builtin_is_constant_evaluated()) {
        const float v0 = a(0, 0) * b(0, 0) + a(0, 1) * b(1, 0);
        const float v1 = a(0, 0) * b(0, 1) + a(0, 1) * b(1, 1);
        const float v2 = a(1, 0) * b(0, 0) + a(1, 1) * b(1, 0);
        const float v3 = a(1, 0) * b(1, 0) + a(1, 1) * b(1, 1);
        return { { v0, v1, v2, v3 } };
    }
    const vec4 mul1 = _mm_shuffle_ps(b.mat, b.mat, EUCLID_SHUFFLE_MASK(0, 2, 0, 2));
    const vec4 mul2 = _mm_shuffle_ps(b.mat, b.mat, EUCLID_SHUFFLE_MASK(1, 3, 1, 3));
    const vec4 res1 = _mm_mul_ps(a.mat, mul1);
    const vec4 res2 = _mm_mul_ps(a.mat, mul2);
    const vec4 res3 = _mm_hadd_ps(res1, res2);
    const vec4 res = _mm_shuffle_ps(res3, res3, EUCLID_SHUFFLE_MASK(0, 2, 1, 3));
    return { res };
}

EUCLID_QUALIFIER mat2 EUCLID_CALL operator*(const mat2 a, const float v) noexcept {
    return { a.mat * v };
}

EUCLID_QUALIFIER mat2 EUCLID_CALL operator*(const float v, const mat2 a) noexcept {
    return { a.mat * v };
}

EUCLID_QUALIFIER mat2& EUCLID_CALL operator+=(mat2& a, const mat2 b) noexcept {
    a.mat += b.mat;
    return a;
}

EUCLID_QUALIFIER mat2& EUCLID_CALL operator-=(mat2& a, const mat2 b) noexcept {
    a.mat -= b.mat;
    return a;
}

EUCLID_QUALIFIER mat2& EUCLID_CALL operator*=(mat2& a, const float v) noexcept {
    a.mat *= v;
    return a;
}

EUCLID_QUALIFIER mat2& EUCLID_CALL transposed(mat2& m) noexcept {
    m = transpose(m);
    return m;
}

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif