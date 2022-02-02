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

struct alignas(32) mat4i {
    using value_type = int;

    EUCLID_MSVC_CONSTEXPR mat4i() noexcept : mat{ setZeroVec8i(), setZeroVec8i() } {}

    EUCLID_MSVC_CONSTEXPR mat4i(
        const int a0,  const int a1,  const int a2,  const int a3,
        const int a4,  const int a5,  const int a6,  const int a7,
        const int a8,  const int a9,  const int a10, const int a11,
        const int a12, const int a13, const int a14, const int a15)
        noexcept : mat{ setVec8i(a0, a1, a2,  a3,  a4,  a5,  a6,  a7),
                        setVec8i(a8, a9, a10, a11, a12, a13, a14, a15) } {}

    EUCLID_MSVC_CONSTEXPR mat4i(const vec8i a, const vec8i b) noexcept : mat{ a, b } {}

    EUCLID_MSVC_CONSTEXPR static mat4i EUCLID_CALL identity() noexcept {
        return { setVec8i(1,0,0,0,0,1,0,0), setVec8i(0,0,1,0,0,0,0,1) };
    }

    EUCLID_MSVC_CONSTEXPR int& operator()(const std::size_t row, const std::size_t col) noexcept {
        const std::size_t pos = 4 * row + col;
        if (pos > 7) {
            return getRefDataVec8(mat[1], pos - 8);
        }
        return getRefDataVec8(mat[0], pos);
    }

    EUCLID_MSVC_CONSTEXPR int operator()(const std::size_t row, const std::size_t col) const noexcept {
        return const_cast<mat4i*>(this)->operator()(row, col);
    }

    EUCLID_MSVC_CONSTEXPR vec4i operator[](const std::size_t pos) const noexcept {
        const int a0 = getDataVec8(mat[0], pos);
        const int a1 = getDataVec8(mat[0], pos + 4);
        const int a2 = getDataVec8(mat[1], pos);
        const int a3 = getDataVec8(mat[1], pos + 4);
        return setVec4i(a0, a1, a2, a3);
    }

    vec8i mat[2];
};

struct alignas(32) mat4f {
    using value_type = float;

    EUCLID_MSVC_CONSTEXPR static mat4f EUCLID_CALL identity() noexcept {
        return { { setVec8f(1,0,0,0,0,1,0,0), setVec8f(0,0,1,0,0,0,0,1) } };
    }

    EUCLID_MSVC_CONSTEXPR float& operator()(const std::size_t row, const std::size_t col) noexcept {
        const std::size_t pos = 4 * row + col;
        if (pos > 7) {
            return getRefDataVec8(mat[1], pos - 8);
        }
        return getRefDataVec8(mat[0], pos);
    }

    EUCLID_MSVC_CONSTEXPR float operator()(const std::size_t row, const std::size_t col) const noexcept {
        return const_cast<mat4f*>(this)->operator()(row, col);
    }

    EUCLID_MSVC_CONSTEXPR vec4f operator[](const std::size_t pos) const noexcept {
        const float a0 = getDataVec8(mat[0], pos);
        const float a1 = getDataVec8(mat[0], pos + 4);
        const float a2 = getDataVec8(mat[1], pos);
        const float a3 = getDataVec8(mat[1], pos + 4);
        return setVec4f(a0, a1, a2, a3);
    }

    vec8f mat[2];
};

EUCLID_FUNC_QUALIFIER vec4i EUCLID_CALL mat4MulVec(const mat4i m, const vec4i v) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        int a[4]{};
        for (std::size_t i = 0; i < 4; ++i) {
            a[0] += getDataVec8(m.mat[0], i)     * getDataVec4(v, i);
            a[1] += getDataVec8(m.mat[0], i + 4) * getDataVec4(v, i);
            a[2] += getDataVec8(m.mat[1], i)     * getDataVec4(v, i);
            a[3] += getDataVec8(m.mat[1], i + 4) * getDataVec4(v, i);
        }
        return setVec4i(a[0], a[1], a[2], a[3]);
    }
#endif
    const vec8i data = _mm256_set_m128i(v, v);
    const vec8i mul1 = _mm256_mullo_epi32(m.mat[0], data);
    const vec8i mul2 = _mm256_mullo_epi32(m.mat[1], data);
    const vec8i res1 = _mm256_hadd_epi32(mul1, mul2);
    const vec8i res2 = _mm256_hadd_epi32(res1, res1);
    const vec8i mask = _mm256_set_epi32(0,0,0,0,5,1,4,0);
    const vec8i res3 = _mm256_permutevar8x32_epi32(res2, mask);
    return _mm256_castsi256_si128(res3);
}

EUCLID_FUNC_QUALIFIER mat4i EUCLID_CALL transposeMat4(const mat4i a) noexcept {
#ifdef _MSC_VER
    mat4i temp = a;
    if (__builtin_is_constant_evaluated()) {
        util::swap(temp(0, 1), temp(1, 0));
        util::swap(temp(0, 2), temp(2, 0));
        util::swap(temp(0, 3), temp(3, 0));
        util::swap(temp(1, 2), temp(2, 1));
        util::swap(temp(1, 3), temp(3, 1));
        util::swap(temp(2, 3), temp(3, 2));
        return temp;
    }
#endif
    const vec8i mask    = _mm256_set_epi32(7,3,6,2,5,1,4,0);
    const vec8i unpack1 = _mm256_unpacklo_epi32(a.mat[0], a.mat[1]);
    const vec8i unpack2 = _mm256_unpackhi_epi32(a.mat[0], a.mat[1]);
    const vec8i trans1  = _mm256_permutevar8x32_epi32(unpack1, mask);
    const vec8i trans2  = _mm256_permutevar8x32_epi32(unpack2, mask);
    return { trans1, trans2 };
}

EUCLID_FUNC_QUALIFIER mat4i EUCLID_CALL negateMat4(const mat4i a) noexcept {
    return { negateVec8(a.mat[0]), negateVec8(a.mat[1]) };
}

EUCLID_FUNC_QUALIFIER mat4i EUCLID_CALL addMat4(const mat4i a, const mat4i b) noexcept {
    return { addVec8(a.mat[0], b.mat[0]), addVec8(a.mat[1], b.mat[1]) };
}

EUCLID_FUNC_QUALIFIER mat4i EUCLID_CALL subMat4(const mat4i a, const mat4i b) noexcept {
    return { subVec8(a.mat[0], b.mat[0]), subVec8(a.mat[1], b.mat[1]) };
}

EUCLID_FUNC_QUALIFIER mat4i EUCLID_CALL mulMat4(const mat4i a, const mat4i b) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        const auto [a00, a10, a20, a30] = mat4MulVec(a, b[0]).m128i_i32;
        const auto [a01, a11, a21, a31] = mat4MulVec(a, b[1]).m128i_i32;
        const auto [a02, a12, a22, a32] = mat4MulVec(a, b[2]).m128i_i32;
        const auto [a03, a13, a23, a33] = mat4MulVec(a, b[3]).m128i_i32;
        return {
            a00, a01, a02, a03,
            a10, a11, a12, a13,
            a20, a21, a22, a23,
            a30, a31, a32, a33
        };
    }
#endif
    const mat4i transMat     = transposeMat4(b);
    const vec8i resultMask   = _mm256_set_epi32(6,3,4,1,7,2,5,0);
    const vec8i invertFirst  = _mm256_permute2x128_si256(a.mat[0], a.mat[0], 1);
    const vec8i invertSecond = _mm256_permute2x128_si256(a.mat[1], a.mat[1], 1);
    const vec8i firstMulTrFirst   = _mm256_mullo_epi32(a.mat[0], transMat.mat[0]);
    const vec8i firstMulTrSecond  = _mm256_mullo_epi32(a.mat[0], transMat.mat[1]);
    const vec8i secondMulTrFirst  = _mm256_mullo_epi32(a.mat[1], transMat.mat[0]);
    const vec8i secondMulTrSecond = _mm256_mullo_epi32(a.mat[1], transMat.mat[1]);
    const vec8i invertFirstMulTrFirst   = _mm256_mullo_epi32(invertFirst,  transMat.mat[0]);
    const vec8i invertFirstMulTrSecond  = _mm256_mullo_epi32(invertFirst,  transMat.mat[1]);
    const vec8i invertSecondMulTrFirst  = _mm256_mullo_epi32(invertSecond, transMat.mat[0]);
    const vec8i invertSecondMulTrSecond = _mm256_mullo_epi32(invertSecond, transMat.mat[1]);
    const vec8i halfFirst1   = _mm256_hadd_epi32(firstMulTrFirst,   invertFirstMulTrFirst);
    const vec8i halfFirst2   = _mm256_hadd_epi32(firstMulTrSecond,  invertFirstMulTrSecond);
    const vec8i halfSecond1  = _mm256_hadd_epi32(secondMulTrFirst,  invertSecondMulTrFirst);
    const vec8i halfSecond2  = _mm256_hadd_epi32(secondMulTrSecond, invertSecondMulTrSecond);
    const vec8i tempFirst    = _mm256_hadd_epi32(halfFirst1,  halfFirst2);
    const vec8i tempSecond   = _mm256_hadd_epi32(halfSecond1, halfSecond2);
    const vec8i resultFirst  = _mm256_permutevar8x32_epi32(tempFirst,  resultMask);
    const vec8i resultSecond = _mm256_permutevar8x32_epi32(tempSecond, resultMask);
    return { resultFirst, resultSecond };
}

EUCLID_FUNC_QUALIFIER mat4i EUCLID_CALL divMat4(const mat4i a, const int val) noexcept {
    return { divVec8(a.mat[0], set1Vec8i(val)), divVec8(a.mat[1], set1Vec8i(val)) };
}

EUCLID_FUNC_QUALIFIER mat4i EUCLID_CALL scaleMat4(const mat4i a, const int v) noexcept {
    return { scaleVec8(a.mat[0], v), scaleVec8(a.mat[1], v) };
}

/* -------------------------------------------------------------------------------- */

EUCLID_FUNC_QUALIFIER vec4f EUCLID_CALL mat4MulVec(const mat4f m, const vec4f v) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        float a[4]{};
        for (std::size_t i = 0; i < 4; ++i) {
            a[0] += getDataVec8(m.mat[0], i)     * getDataVec4(v, i);
            a[1] += getDataVec8(m.mat[0], i + 4) * getDataVec4(v, i);
            a[2] += getDataVec8(m.mat[1], i)     * getDataVec4(v, i);
            a[3] += getDataVec8(m.mat[1], i + 4) * getDataVec4(v, i);
        }
        return setVec4f(a[0], a[1], a[2], a[3]);
    }
#endif
    const vec8f data = _mm256_set_m128(v, v);
    const vec8f mul1 = _mm256_mul_ps(m.mat[0], data);
    const vec8f mul2 = _mm256_mul_ps(m.mat[1], data);
    const vec8f res1 = _mm256_hadd_ps(mul1, mul2);
    const vec8f res2 = _mm256_hadd_ps(res1, res1);
    const vec8i mask = _mm256_set_epi32(0,0,0,0,5,1,4,0);
    const vec8f res3 = _mm256_permutevar8x32_ps(res2, mask);
    return _mm256_castps256_ps128(res3);
}

EUCLID_FUNC_QUALIFIER mat4f EUCLID_CALL transposeMat4(const mat4f a) noexcept {
#ifdef _MSC_VER
    mat4f temp = a;
    if (__builtin_is_constant_evaluated()) {
        util::swap(temp(0, 1), temp(1, 0));
        util::swap(temp(0, 2), temp(2, 0));
        util::swap(temp(0, 3), temp(3, 0));
        util::swap(temp(1, 2), temp(2, 1));
        util::swap(temp(1, 3), temp(3, 1));
        util::swap(temp(2, 3), temp(3, 2));
        return temp;
    }
#endif
    const vec8i mask    = _mm256_set_epi32(7,3,6,2,5,1,4,0);
    const vec8f unpack1 = _mm256_unpacklo_ps(a.mat[0], a.mat[1]);
    const vec8f unpack2 = _mm256_unpackhi_ps(a.mat[0], a.mat[1]);
    const vec8f trans1  = _mm256_permutevar8x32_ps(unpack1, mask);
    const vec8f trans2  = _mm256_permutevar8x32_ps(unpack2, mask);
    return { { trans1, trans2 } };
}

EUCLID_FUNC_QUALIFIER mat4f EUCLID_CALL negateMat4(const mat4f a) noexcept {
    return { { negateVec8(a.mat[0]), negateVec8(a.mat[1]) } };
}

EUCLID_FUNC_QUALIFIER mat4f EUCLID_CALL addMat4(const mat4f a, const mat4f b) noexcept {
    return { { addVec8(a.mat[0], b.mat[0]), addVec8(a.mat[1], b.mat[1]) } };
}

EUCLID_FUNC_QUALIFIER mat4f EUCLID_CALL subMat4(const mat4f a, const mat4f b) noexcept {
    return { { subVec8(a.mat[0], b.mat[0]), subVec8(a.mat[1], b.mat[1]) } };
}

EUCLID_FUNC_QUALIFIER mat4f EUCLID_CALL mulMat4(const mat4f a, const mat4f b) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        const auto [a00, a10, a20, a30] = mat4MulVec(a, b[0]).m128_f32;
        const auto [a01, a11, a21, a31] = mat4MulVec(a, b[1]).m128_f32;
        const auto [a02, a12, a22, a32] = mat4MulVec(a, b[2]).m128_f32;
        const auto [a03, a13, a23, a33] = mat4MulVec(a, b[3]).m128_f32;
        return {
            { { a00, a01, a02, a03,
                a10, a11, a12, a13 },
              { a20, a21, a22, a23,
                a30, a31, a32, a33 } }
        };
    }
#endif
    const mat4f transMat     = transposeMat4(b);
    const vec8i resultMask   = _mm256_set_epi32(6,3,4,1,7,2,5,0);
    const vec8f invertFirst  = _mm256_permute2f128_ps(a.mat[0], a.mat[0], 1);
    const vec8f invertSecond = _mm256_permute2f128_ps(a.mat[1], a.mat[1], 1);
    const vec8f firstMulTrFirst   = _mm256_mul_ps(a.mat[0], transMat.mat[0]);
    const vec8f firstMulTrSecond  = _mm256_mul_ps(a.mat[0], transMat.mat[1]);
    const vec8f secondMulTrFirst  = _mm256_mul_ps(a.mat[1], transMat.mat[0]);
    const vec8f secondMulTrSecond = _mm256_mul_ps(a.mat[1], transMat.mat[1]);
    const vec8f invertFirstMulTrFirst   = _mm256_mul_ps(invertFirst,  transMat.mat[0]);
    const vec8f invertFirstMulTrSecond  = _mm256_mul_ps(invertFirst,  transMat.mat[1]);
    const vec8f invertSecondMulTrFirst  = _mm256_mul_ps(invertSecond, transMat.mat[0]);
    const vec8f invertSecondMulTrSecond = _mm256_mul_ps(invertSecond, transMat.mat[1]);
    const vec8f halfFirst1   = _mm256_hadd_ps(firstMulTrFirst,   invertFirstMulTrFirst);
    const vec8f halfFirst2   = _mm256_hadd_ps(firstMulTrSecond,  invertFirstMulTrSecond);
    const vec8f halfSecond1  = _mm256_hadd_ps(secondMulTrFirst,  invertSecondMulTrFirst);
    const vec8f halfSecond2  = _mm256_hadd_ps(secondMulTrSecond, invertSecondMulTrSecond);
    const vec8f tempFirst    = _mm256_hadd_ps(halfFirst1,  halfFirst2);
    const vec8f tempSecond   = _mm256_hadd_ps(halfSecond1, halfSecond2);
    const vec8f resultFirst  = _mm256_permutevar8x32_ps(tempFirst,  resultMask);
    const vec8f resultSecond = _mm256_permutevar8x32_ps(tempSecond, resultMask);
    return { { resultFirst, resultSecond } };
}

EUCLID_FUNC_QUALIFIER mat4f EUCLID_CALL divMat4(const mat4f a, const float v) noexcept {
    return { { divVec8(a.mat[0], set1Vec8f(v)), divVec8(a.mat[1], set1Vec8f(v)) } };
}

EUCLID_FUNC_QUALIFIER mat4f EUCLID_CALL scaleMat4(const mat4f a, const float v) noexcept {
    return { { scaleVec8(a.mat[0], v), scaleVec8(a.mat[1], v) } };
}

/* -------------------------------------------------------------------------------- */

#ifdef EUCLID_HAS_GLOBAL_OPERATOR_OVERLOAD

EUCLID_FUNC_QUALIFIER mat4i& EUCLID_CALL operator+=(mat4i& a, const mat4i b) noexcept {
    a = addMat4(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER mat4i& EUCLID_CALL operator-=(mat4i& a, const mat4i b) noexcept {
    a = subMat4(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER mat4i& EUCLID_CALL operator*=(mat4i& a, const int val) noexcept {
    a = scaleMat4(a, val);
    return a;
}

EUCLID_FUNC_QUALIFIER mat4i& EUCLID_CALL operator/=(mat4i& a, const int val) noexcept {
    a = divMat4(a, val);
    return a;
}

EUCLID_FUNC_QUALIFIER mat4i  EUCLID_CALL operator-(const mat4i m) noexcept {
    return negateMat4(m);
}

EUCLID_FUNC_QUALIFIER mat4i  EUCLID_CALL operator+(const mat4i a, const mat4i b) noexcept {
    return addMat4(a, b);
}

EUCLID_FUNC_QUALIFIER mat4i  EUCLID_CALL operator-(const mat4i a, const mat4i b) noexcept {
    return subMat4(a, b);
}

EUCLID_FUNC_QUALIFIER mat4i  EUCLID_CALL operator*(const mat4i a, const mat4i b) noexcept {
    return mulMat4(a, b);
}

EUCLID_FUNC_QUALIFIER mat4i  EUCLID_CALL operator*(const mat4i a, const int val) noexcept {
    return scaleMat4(a, val);
}

EUCLID_FUNC_QUALIFIER vec4i  EUCLID_CALL operator*(const mat4i m, const vec4i v) noexcept {
    return mat4MulVec(m, v);
}

EUCLID_FUNC_QUALIFIER mat4i  EUCLID_CALL operator/(const mat4i a, const int val) noexcept {
    return divMat4(a, val);
}

EUCLID_FUNC_QUALIFIER mat4i  EUCLID_CALL operator*(const int val, const mat4i a) noexcept {
    return scaleMat4(a, val);
}

/* ------------------------------------------------------------------------- */

EUCLID_FUNC_QUALIFIER mat4f& EUCLID_CALL operator+=(mat4f& a, const mat4f b) noexcept {
    a = addMat4(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER mat4f& EUCLID_CALL operator-=(mat4f& a, const mat4f b) noexcept {
    a = subMat4(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER mat4f& EUCLID_CALL operator*=(mat4f& a, const float v) noexcept {
    a = scaleMat4(a, v);
    return a;
}

EUCLID_FUNC_QUALIFIER mat4f& EUCLID_CALL operator/=(mat4f& a, const float v) noexcept {
    a = divMat4(a, v);
    return a;
}

EUCLID_FUNC_QUALIFIER mat4f  EUCLID_CALL operator-(const mat4f m) noexcept {
    return negateMat4(m);
}

EUCLID_FUNC_QUALIFIER mat4f  EUCLID_CALL operator+(const mat4f a, const mat4f b) noexcept {
    return addMat4(a, b);
}

EUCLID_FUNC_QUALIFIER mat4f  EUCLID_CALL operator-(const mat4f a, const mat4f b) noexcept {
    return subMat4(a, b);
}

EUCLID_FUNC_QUALIFIER mat4f  EUCLID_CALL operator*(const mat4f a, const mat4f b) noexcept {
    return mulMat4(a, b);
}

EUCLID_FUNC_QUALIFIER vec4f  EUCLID_CALL operator*(const mat4f m, const vec4f v) noexcept {
    return mat4MulVec(m, v);
}

EUCLID_FUNC_QUALIFIER mat4f  EUCLID_CALL operator*(const mat4f a, const float v) noexcept {
    return scaleMat4(a, v);
}

EUCLID_FUNC_QUALIFIER mat4f  EUCLID_CALL operator/(const mat4f a, const float v) noexcept {
    return divMat4(a, v);
}

EUCLID_FUNC_QUALIFIER mat4f  EUCLID_CALL operator*(const float v, const mat4f a) noexcept {
    return scaleMat4(a, v);
}

#endif // Overload Global Operator

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif