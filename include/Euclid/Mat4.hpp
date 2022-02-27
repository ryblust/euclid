#pragma once

#include "Vec8.hpp"
#include "Util.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4514 5045)
// enable /Wall
// C4514 : Ignore the compiler's warning about removing unused inline functions
// C5045 : Spectre mitigation
#endif

namespace euclid {

struct alignas(32) Mat4 {
    using value_type = float;

    static constexpr Mat4 EUCLID_CALL identity() noexcept {
        return { { { 1,0,0,0,0,1,0,0 }, { 0,0,1,0,0,0,0,1 } } };
    }

    EUCLID_CONSTEXPR float& operator()(const std::size_t row, const std::size_t col) noexcept {
        if (row > 2) {
            return getVec8RefData(mat[1], 4 * row + col - 8);
        }
        return getVec8RefData(mat[0], 4 * row + col);
    }

    EUCLID_CONSTEXPR float operator()(const std::size_t row, const std::size_t col) const noexcept {
        if (row > 2) {
            return getVec8Data(mat[1], 4 * row + col - 8);
        }
        return getVec8Data(mat[0], 4 * row + col);
    }

    EUCLID_CONSTEXPR Vec4 operator[](const std::size_t pos) const noexcept {
        if (__builtin_is_constant_evaluated()) {
            const float a0 = getVec8Data(mat[0], pos);
            const float a1 = getVec8Data(mat[0], pos + 4);
            const float a2 = getVec8Data(mat[1], pos);
            const float a3 = getVec8Data(mat[1], pos + 4);
            return { a0,a1,a2,a3 };
        }
        switch (pos) {
            case 0: {
                const Vec8 data = _mm256_unpacklo_ps(mat[0], mat[1]);
                const Vec4 extract = _mm256_extractf128_ps(data, 1);
                return _mm_unpacklo_ps(_mm256_castps256_ps128(data), extract);
            }
            case 1: {
                const Vec8 data = _mm256_unpacklo_ps(mat[0], mat[1]);
                const Vec4 extract = _mm256_extractf128_ps(data, 1);
                return _mm_unpackhi_ps(_mm256_castps256_ps128(data), extract);
            }
            case 2: {
                const Vec8 data = _mm256_unpackhi_ps(mat[0], mat[1]);
                const Vec4 extract = _mm256_extractf128_ps(data, 1);
                return _mm_unpacklo_ps(_mm256_castps256_ps128(data), extract);
            }
            case 3: {
                const Vec8 data = _mm256_unpackhi_ps(mat[0], mat[1]);
                const Vec4 extract = _mm256_extractf128_ps(data, 1);
                return _mm_unpackhi_ps(_mm256_castps256_ps128(data), extract);
            }
            default: return setZeroVec4();
        }
    }

    Vec8 mat[2];
};

EUCLID_QUALIFIER Mat4 EUCLID_CALL transpose(Mat4 a) noexcept {
    if (__builtin_is_constant_evaluated()) {
        util::swap(a(0, 1), a(1, 0));
        util::swap(a(0, 2), a(2, 0));
        util::swap(a(0, 3), a(3, 0));
        util::swap(a(1, 2), a(2, 1));
        util::swap(a(1, 3), a(3, 1));
        util::swap(a(2, 3), a(3, 2));
        return a;
    }
    const __m256i mask = _mm256_set_epi32(7,3,6,2,5,1,4,0);
    const Vec8 unpack1 = _mm256_unpacklo_ps(a.mat[0], a.mat[1]);
    const Vec8 unpack2 = _mm256_unpackhi_ps(a.mat[0], a.mat[1]);
    const Vec8 trans1  = _mm256_permutevar8x32_ps(unpack1, mask);
    const Vec8 trans2  = _mm256_permutevar8x32_ps(unpack2, mask);
    return { { trans1, trans2 } };
}

EUCLID_QUALIFIER Mat4 EUCLID_CALL operator+(const Mat4 a, const Mat4 b) noexcept {
    return { { a.mat[0] + b.mat[0], a.mat[1] + b.mat[1] } };
}

EUCLID_QUALIFIER Mat4 EUCLID_CALL operator-(const Mat4 a, const Mat4 b) noexcept {
    return { { a.mat[0] - b.mat[0], a.mat[1] - b.mat[1] } };
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL operator*(const Mat4 m, const Vec4 v) noexcept {
    if (__builtin_is_constant_evaluated()) {
        float a[4]{};
        for (std::size_t i = 0; i < 4; ++i) {
            a[0] += getVec8Data(m.mat[0], i)     * getVec4Data(v, i);
            a[1] += getVec8Data(m.mat[0], i + 4) * getVec4Data(v, i);
            a[2] += getVec8Data(m.mat[1], i)     * getVec4Data(v, i);
            a[3] += getVec8Data(m.mat[1], i + 4) * getVec4Data(v, i);
        }
        return { a[0], a[1], a[2], a[3] };
    }
    const __m256i mask = _mm256_set_epi32(0,0,0,0,5,1,4,0);
    const Vec8 data = _mm256_set_m128(v, v);
    const Vec8 mul1 = _mm256_mul_ps(m.mat[0], data);
    const Vec8 mul2 = _mm256_mul_ps(m.mat[1], data);
    const Vec8 res1 = _mm256_hadd_ps(mul1, mul2);
    const Vec8 res2 = _mm256_hadd_ps(res1, res1);
    const Vec8 res3 = _mm256_permutevar8x32_ps(res2, mask);
    return _mm256_castps256_ps128(res3);
}

EUCLID_QUALIFIER Mat4 EUCLID_CALL operator*(const Mat4 a, const Mat4 b) noexcept {
    if (__builtin_is_constant_evaluated()) {
        const Vec4 data1 = a * b[0];
        const Vec4 data2 = a * b[1];
        const Vec4 data3 = a * b[2];
        const Vec4 data4 = a * b[3];
        const Mat4 temp = { { combine2Vec4(data1, data2), combine2Vec4(data3, data4) } };
        return transpose(temp);
    }
    Vec8 a0 = _mm256_shuffle_ps(a.mat[0], a.mat[0], EUCLID_SHUFFLE_MASK(0, 0, 0, 0));
    Vec8 a1 = _mm256_shuffle_ps(a.mat[1], a.mat[1], EUCLID_SHUFFLE_MASK(0, 0, 0, 0));
    Vec8 b0 = _mm256_permute2f128_ps(b.mat[0], b.mat[0], 0x00);
    Vec8 c0 = _mm256_mul_ps(a0, b0);
    Vec8 c1 = _mm256_mul_ps(a1, b0);
    a0 = _mm256_shuffle_ps(a.mat[0], a.mat[0], EUCLID_SHUFFLE_MASK(1, 1, 1, 1));
    a1 = _mm256_shuffle_ps(a.mat[1], a.mat[1], EUCLID_SHUFFLE_MASK(1, 1, 1, 1));
    b0 = _mm256_permute2f128_ps(b.mat[0], b.mat[0], 0x11);
    Vec8 c2 = _mm256_fmadd_ps(a0, b0, c0);
    Vec8 c3 = _mm256_fmadd_ps(a1, b0, c1);
    a0 = _mm256_shuffle_ps(a.mat[0], a.mat[0], EUCLID_SHUFFLE_MASK(2, 2, 2, 2));
    a1 = _mm256_shuffle_ps(a.mat[1], a.mat[1], EUCLID_SHUFFLE_MASK(2, 2, 2, 2));
    Vec8 b1 = _mm256_permute2f128_ps(b.mat[1], b.mat[1], 0x00);
    Vec8 c4 = _mm256_mul_ps(a0, b1);
    Vec8 c5 = _mm256_mul_ps(a1, b1);
    a0 = _mm256_shuffle_ps(a.mat[0], a.mat[0], EUCLID_SHUFFLE_MASK(3, 3, 3, 3));
    a1 = _mm256_shuffle_ps(a.mat[1], a.mat[1], EUCLID_SHUFFLE_MASK(3, 3, 3, 3));
    b1 = _mm256_permute2f128_ps(b.mat[1], b.mat[1], 0x11);
    Vec8 c6 = _mm256_fmadd_ps(a0, b1, c4);
    Vec8 c7 = _mm256_fmadd_ps(a1, b1, c5);
    return { { _mm256_add_ps(c2, c6), _mm256_add_ps(c3, c7) } };
}

EUCLID_QUALIFIER Mat4 EUCLID_CALL operator*(const Mat4 a, const float v) noexcept {
    return { { a.mat[0] * v, a.mat[1] * v} };
}

EUCLID_QUALIFIER Mat4 EUCLID_CALL operator*(const float v, const Mat4 a) noexcept {
    return { { a.mat[0] * v, a.mat[1] * v} };
}

EUCLID_QUALIFIER Mat4& EUCLID_CALL operator+=(Mat4& a, const Mat4 b) noexcept {
    a = a + b;
    return a;
}

EUCLID_QUALIFIER Mat4& EUCLID_CALL operator-=(Mat4& a, const Mat4 b) noexcept {
    a = a - b;
    return a;
}

EUCLID_QUALIFIER Mat4& EUCLID_CALL operator*=(Mat4& a, const float v) noexcept {
    a = a * v;
    return a;
}

EUCLID_QUALIFIER bool EUCLID_CALL operator==(const Mat4 a, const Mat4 b) noexcept {
    return a[0] == b[0] && a[1] == b[1];
}

EUCLID_QUALIFIER bool EUCLID_CALL operator!=(const Mat4 a, const Mat4 b) noexcept {
    return !(a == b);
}

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif