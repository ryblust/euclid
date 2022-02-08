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

struct alignas(32) mat4 {
    using value_type = float;

    static constexpr mat4 EUCLID_CALL identity() noexcept {
        return { { { 1,0,0,0,0,1,0,0 }, { 0,0,1,0,0,0,0,1 } } };
    }

    constexpr float& operator()(const std::size_t row, const std::size_t col) noexcept {
        const std::size_t pos = 4 * row + col;
        if (pos > 7) {
            return getVec8RefData(mat[1], pos - 8);
        }
        return getVec8RefData(mat[0], pos);
    }

    constexpr float operator()(const std::size_t row, const std::size_t col) const noexcept {
        const std::size_t pos = 4 * row + col;
        if (pos > 7) {
            return getVec8Data(mat[1], pos - 8);
        }
        return getVec8Data(mat[0], pos);
    }

    constexpr vec4 operator[](const std::size_t pos) const noexcept {
        if (__builtin_is_constant_evaluated()) {
            const float a0 = getVec8Data(mat[0], pos);
            const float a1 = getVec8Data(mat[0], pos + 4);
            const float a2 = getVec8Data(mat[1], pos);
            const float a3 = getVec8Data(mat[1], pos + 4);
            return { a0,a1,a2,a3 };
        }
        switch (pos) {
            case 0: {
                const vec8 data = _mm256_unpacklo_ps(mat[0], mat[1]);
                const vec4 extract = _mm256_extractf128_ps(data, 1);
                return _mm_unpacklo_ps(_mm256_castps256_ps128(data), extract);
            }
            case 1: {
                const vec8 data = _mm256_unpacklo_ps(mat[0], mat[1]);
                const vec4 extract = _mm256_extractf128_ps(data, 1);
                return _mm_unpackhi_ps(_mm256_castps256_ps128(data), extract);
            }
            case 2: {
                const vec8 data = _mm256_unpackhi_ps(mat[0], mat[1]);
                const vec4 extract = _mm256_extractf128_ps(data, 1);
                return _mm_unpacklo_ps(_mm256_castps256_ps128(data), extract);
            }
            case 3: {
                const vec8 data = _mm256_unpackhi_ps(mat[0], mat[1]);
                const vec4 extract = _mm256_extractf128_ps(data, 1);
                return _mm_unpackhi_ps(_mm256_castps256_ps128(data), extract);
            }
            default: return setZeroVec4();
        }
    }

    vec8 mat[2];
};

EUCLID_QUALIFIER mat4 EUCLID_CALL transpose(mat4 a) noexcept {
    if (__builtin_is_constant_evaluated()) {
        util::swap(a(0, 1), a(1, 0));
        util::swap(a(0, 2), a(2, 0));
        util::swap(a(0, 3), a(3, 0));
        util::swap(a(1, 2), a(2, 1));
        util::swap(a(1, 3), a(3, 1));
        util::swap(a(2, 3), a(3, 2));
        return a;
    }
    const __m256i mask  = _mm256_set_epi32(7,3,6,2,5,1,4,0);
    const vec8 unpack1 = _mm256_unpacklo_ps(a.mat[0], a.mat[1]);
    const vec8 unpack2 = _mm256_unpackhi_ps(a.mat[0], a.mat[1]);
    const vec8 trans1  = _mm256_permutevar8x32_ps(unpack1, mask);
    const vec8 trans2  = _mm256_permutevar8x32_ps(unpack2, mask);
    return { { trans1, trans2 } };
}

EUCLID_QUALIFIER mat4 EUCLID_CALL operator+(const mat4 a, const mat4 b) noexcept {
    return { { a.mat[0] + b.mat[0], a.mat[1] + b.mat[1] } };
}

EUCLID_QUALIFIER mat4 EUCLID_CALL operator-(const mat4 a, const mat4 b) noexcept {
    return { { a.mat[0] - b.mat[0], a.mat[1] - b.mat[1] } };
}

EUCLID_QUALIFIER vec4 EUCLID_CALL operator*(const mat4 m, const vec4 v) noexcept {
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
    const vec8 data = _mm256_set_m128(v, v);
    const vec8 mul1 = _mm256_mul_ps(m.mat[0], data);
    const vec8 mul2 = _mm256_mul_ps(m.mat[1], data);
    const vec8 res1 = _mm256_hadd_ps(mul1, mul2);
    const vec8 res2 = _mm256_hadd_ps(res1, res1);
    const vec8 res3 = _mm256_permutevar8x32_ps(res2, mask);
    return _mm256_castps256_ps128(res3);
}

EUCLID_QUALIFIER mat4 EUCLID_CALL operator*(mat4 a, const mat4 b) noexcept {
    if (__builtin_is_constant_evaluated()) {
        const vec4 data1 = a * b[0];
        const vec4 data2 = a * b[1];
        const vec4 data3 = a * b[2];
        const vec4 data4 = a * b[3];
        const mat4 temp = { {combine2Vec4(data1, data2), combine2Vec4(data3, data4) } };
        return transpose(temp);
    }
    vec8 a0 = _mm256_shuffle_ps(a.mat[0], a.mat[0], EUCLID_SHUFFLE_MASK(0, 0, 0, 0));
    vec8 a1 = _mm256_shuffle_ps(a.mat[1], a.mat[1], EUCLID_SHUFFLE_MASK(0, 0, 0, 0));
    vec8 b0 = _mm256_permute2f128_ps(b.mat[0], b.mat[0], 0x00);
    vec8 c0 = _mm256_mul_ps(a0, b0);
    vec8 c1 = _mm256_mul_ps(a1, b0);
    a0 = _mm256_shuffle_ps(a.mat[0], a.mat[0], EUCLID_SHUFFLE_MASK(1, 1, 1, 1));
    a1 = _mm256_shuffle_ps(a.mat[1], a.mat[1], EUCLID_SHUFFLE_MASK(1, 1, 1, 1));
    b0 = _mm256_permute2f128_ps(b.mat[0], b.mat[0], 0x11);
    vec8 c2 = _mm256_fmadd_ps(a0, b0, c0);
    vec8 c3 = _mm256_fmadd_ps(a1, b0, c1);
    a0 = _mm256_shuffle_ps(a.mat[0], a.mat[0], EUCLID_SHUFFLE_MASK(2, 2, 2, 2));
    a1 = _mm256_shuffle_ps(a.mat[1], a.mat[1], EUCLID_SHUFFLE_MASK(2, 2, 2, 2));
    vec8 b1 = _mm256_permute2f128_ps(b.mat[1], b.mat[1], 0x00);
    vec8 c4 = _mm256_mul_ps(a0, b1);
    vec8 c5 = _mm256_mul_ps(a1, b1);
    a0 = _mm256_shuffle_ps(a.mat[0], a.mat[0], EUCLID_SHUFFLE_MASK(3, 3, 3, 3));
    a1 = _mm256_shuffle_ps(a.mat[1], a.mat[1], EUCLID_SHUFFLE_MASK(3, 3, 3, 3));
    b1 = _mm256_permute2f128_ps(b.mat[1], b.mat[1], 0x11);
    vec8 c6 = _mm256_fmadd_ps(a0, b1, c4);
    vec8 c7 = _mm256_fmadd_ps(a1, b1, c5);
    a.mat[0] = _mm256_add_ps(c2, c6);
    a.mat[1] = _mm256_add_ps(c3, c7);
    return { { a.mat[0],a.mat[1] } };
}

EUCLID_QUALIFIER mat4 EUCLID_CALL operator*(const mat4 a, const float v) noexcept {
    return { { a.mat[0] * v, a.mat[1] * v} };
}

EUCLID_QUALIFIER mat4 EUCLID_CALL operator*(const float v, const mat4 a) noexcept {
    return { { a.mat[0] * v, a.mat[1] * v} };
}

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif