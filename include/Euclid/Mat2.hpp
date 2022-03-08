#pragma once

#include "Vec2.hpp"
#include "Vec4.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4514)
// enable /Wall
// C4514 : Ignore the compiler's warning about removing unused inline functions
#endif

namespace euclid {

struct alignas(16) Mat2 {

    static constexpr Mat2 EUCLID_CALL identity() noexcept {
        return { { 1,0,0,1 } };
    }

    EUCLID_CONSTEXPR float& operator()(const std::size_t row, const std::size_t col) noexcept {
        return getVec4RefData(mat, 2 * row + col);
    }

    EUCLID_CONSTEXPR float operator()(const std::size_t row, const std::size_t col) const noexcept {
        return getVec4Data(mat, 2 * row + col);
    }

    Vec4 mat;
};

EUCLID_QUALIFIER Mat2 EUCLID_CALL transpose(const Mat2 m) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return { {
            m(0, 0), m(1, 0),
            m(0, 1), m(1, 1)
        } };
    }
    return { _mm_permute_ps(m.mat, _MM_SHUFFLE(3, 1, 2, 0)) };
}

EUCLID_QUALIFIER Mat2 EUCLID_CALL operator-(const Mat2 a) noexcept {
    return { -a.mat };
}

EUCLID_QUALIFIER Mat2 EUCLID_CALL operator+(const Mat2 a, const Mat2 b) noexcept {
    return { a.mat + b.mat };
}

EUCLID_QUALIFIER Mat2 EUCLID_CALL operator-(const Mat2 a, const Mat2 b) noexcept {
    return { a.mat - b.mat };
}

EUCLID_QUALIFIER Mat2 EUCLID_CALL operator*(const Mat2 a, const Mat2 b) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return { {
            a(0, 0) * b(0, 0) + a(0, 1) * b(1, 0),
            a(0, 0) * b(0, 1) + a(0, 1) * b(1, 1),
            a(1, 0) * b(0, 0) + a(1, 1) * b(1, 0),
            a(1, 0) * b(1, 0) + a(1, 1) * b(1, 1)
        } };
    }
    const Vec4 mul1 = _mm_permute_ps(b.mat, _MM_SHUFFLE(2, 0, 2, 0));
    const Vec4 mul2 = _mm_permute_ps(b.mat, _MM_SHUFFLE(3, 1, 3, 1));
    const Vec4 res1 = _mm_mul_ps(a.mat, mul1);
    const Vec4 res2 = _mm_mul_ps(a.mat, mul2);
    const Vec4 res3 = _mm_hadd_ps(res1, res2);
    return { _mm_permute_ps(res3, _MM_SHUFFLE(3, 1, 2, 0)) };
}

EUCLID_QUALIFIER Mat2 EUCLID_CALL operator*(const Mat2 a, const float v) noexcept {
    return { a.mat * v };
}

EUCLID_QUALIFIER Mat2 EUCLID_CALL operator*(const float v, const Mat2 a) noexcept {
    return { a.mat * v };
}

EUCLID_QUALIFIER Mat2& EUCLID_CALL operator+=(Mat2& a, const Mat2 b) noexcept {
    a.mat += b.mat;
    return a;
}

EUCLID_QUALIFIER Mat2& EUCLID_CALL operator-=(Mat2& a, const Mat2 b) noexcept {
    a.mat -= b.mat;
    return a;
}

EUCLID_QUALIFIER Mat2& EUCLID_CALL operator*=(Mat2& a, const float v) noexcept {
    a.mat *= v;
    return a;
}

EUCLID_QUALIFIER bool EUCLID_CALL operator==(const Mat2 a, const Mat2 b) noexcept {
    return a.mat == b.mat;
}

EUCLID_QUALIFIER bool EUCLID_CALL operator!=(const Mat2 a, const Mat2 b) noexcept {
    return !(a == b);
}

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif