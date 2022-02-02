#pragma once

#include "Vec4.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4514)
// enable /Wall
// C4514 : Ignore the compiler's warning about removing unused inline functions
#endif

namespace euclid {

struct alignas(16) mat2i {
    using value_type = int;

    EUCLID_MSVC_CONSTEXPR mat2i() noexcept : mat(setZeroVec4i()) {}

    EUCLID_MSVC_CONSTEXPR mat2i(const int a0, const int a1,
                                const int a2, const int a3)
        : mat(setVec4i(a0, a1, a2, a3)) {}

    EUCLID_MSVC_CONSTEXPR explicit mat2i (const vec4i vec) noexcept
        : mat(vec) {}

    EUCLID_MSVC_CONSTEXPR static mat2i EUCLID_CALL identity() noexcept {
        return mat2i{ setVec4i(1, 0, 0, 1) };
    }

    EUCLID_MSVC_CONSTEXPR int& operator()(const std::size_t row, const std::size_t col) noexcept {
        return getRefDataVec4(mat, 2 * row + col);
    }

    EUCLID_MSVC_CONSTEXPR int  operator()(const std::size_t row, const std::size_t col) const noexcept {
        return getDataVec4(mat, 2 * row + col);
    }

    vec4i mat;
};

struct alignas(16) mat2f {
    using value_type = float;

    EUCLID_MSVC_CONSTEXPR static mat2f EUCLID_CALL identity() noexcept {
        return { setVec4f(1, 0, 0, 1) };
    }

    EUCLID_MSVC_CONSTEXPR float& operator()(const std::size_t row, const std::size_t col) noexcept {
        return getRefDataVec4(mat, 2 * row + col);
    }

    EUCLID_MSVC_CONSTEXPR float  operator()(const std::size_t row, const std::size_t col) const noexcept {
        return getDataVec4(mat, 2 * row + col);
    }

    vec4f mat;
};

EUCLID_FUNC_QUALIFIER mat2i EUCLID_CALL negateMat2(const mat2i a) noexcept {
    return mat2i{ negateVec4(a.mat) };
}

EUCLID_FUNC_QUALIFIER mat2i EUCLID_CALL addMat2(const mat2i a, const mat2i b) noexcept {
    return mat2i{ addVec4(a.mat, b.mat) };
}

EUCLID_FUNC_QUALIFIER mat2i EUCLID_CALL subMat2(const mat2i a, const mat2i b) noexcept {
    return mat2i{ subVec4(a.mat, b.mat) };
}

EUCLID_FUNC_QUALIFIER mat2i EUCLID_CALL mulMat2(const mat2i a, const mat2i b) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        const int v0 = a(0, 0) * b(0, 0) + a(0, 1) * b(1, 0);
        const int v1 = a(0, 0) * b(0, 1) + a(0, 1) * b(1, 1);
        const int v2 = a(1, 0) * b(0, 0) + a(1, 1) * b(1, 0);
        const int v3 = a(1, 0) * b(1, 0) + a(1, 1) * b(1, 1);
        return { v0, v1, v2, v3 };
    }
#endif
    const vec4i mul1 = _mm_shuffle_epi32(b.mat, EUCLID_SHUFFLE_MASK(0, 2, 0, 2));
    const vec4i mul2 = _mm_shuffle_epi32(b.mat, EUCLID_SHUFFLE_MASK(1, 3, 1, 3));
    const vec4i res1 = _mm_mullo_epi32(a.mat, mul1);
    const vec4i res2 = _mm_mullo_epi32(a.mat, mul2);
    const vec4i res3 = _mm_hadd_epi32(res1, res2);
    const vec4i res  = _mm_shuffle_epi32(res3, EUCLID_SHUFFLE_MASK(0, 2, 1, 3));
    return mat2i{ res };
}

EUCLID_FUNC_QUALIFIER mat2i EUCLID_CALL divMat2(const mat2i a, const int val) noexcept {
    return mat2i{ divVec4(a.mat, set1Vec4i(val)) };
}

EUCLID_FUNC_QUALIFIER mat2i EUCLID_CALL scaleMat2(const mat2i m, const int v) noexcept {
    return mat2i{ scaleVec4(m.mat, v) };
}

EUCLID_FUNC_QUALIFIER mat2i EUCLID_CALL transposeMat2(const mat2i m) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        return { 
            m.mat.m128i_i32[0], m.mat.m128i_i32[2],
            m.mat.m128i_i32[1], m.mat.m128i_i32[3]
        };
    }
#endif
    return mat2i{ _mm_shuffle_epi32(m.mat, 0b11011000) };
}

/* ------------------------------------------------------------------------------------ */

EUCLID_FUNC_QUALIFIER mat2f EUCLID_CALL negateMat2(const mat2f a) noexcept {
    return { negateVec4(a.mat) };
}

EUCLID_FUNC_QUALIFIER mat2f EUCLID_CALL addMat2(const mat2f a, const mat2f b) noexcept {
    return { addVec4(a.mat, b.mat) };
}

EUCLID_FUNC_QUALIFIER mat2f EUCLID_CALL subMat2(const mat2f a, const mat2f b) noexcept {
    return { subVec4(a.mat, b.mat) };
}

EUCLID_FUNC_QUALIFIER mat2f EUCLID_CALL mulMat2(const mat2f a, const mat2f b) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        const float v0 = a(0, 0) * b(0, 0) + a(0, 1) * b(1, 0);
        const float v1 = a(0, 0) * b(0, 1) + a(0, 1) * b(1, 1);
        const float v2 = a(1, 0) * b(0, 0) + a(1, 1) * b(1, 0);
        const float v3 = a(1, 0) * b(1, 0) + a(1, 1) * b(1, 1);
        return { { v0, v1, v2, v3 } };
    }
#endif
    const vec4f mul1 = _mm_shuffle_ps(b.mat, b.mat, EUCLID_SHUFFLE_MASK(0, 2, 0, 2));
    const vec4f mul2 = _mm_shuffle_ps(b.mat, b.mat, EUCLID_SHUFFLE_MASK(1, 3, 1, 3));
    const vec4f res1 = _mm_mul_ps(a.mat, mul1);
    const vec4f res2 = _mm_mul_ps(a.mat, mul2);
    const vec4f res3 = _mm_hadd_ps(res1, res2);
    const vec4f res  = _mm_shuffle_ps(res3, res3, EUCLID_SHUFFLE_MASK(0, 2, 1, 3));
    return { res };
}

EUCLID_FUNC_QUALIFIER mat2f EUCLID_CALL divMat2(const mat2f a, const float v) noexcept {
    return { divVec4(a.mat, set1Vec4f(v)) };
}

EUCLID_FUNC_QUALIFIER mat2f EUCLID_CALL scaleMat2(const mat2f m, const float v) noexcept {
    return { scaleVec4(m.mat, v) };
}

EUCLID_FUNC_QUALIFIER mat2f EUCLID_CALL transposeMat2(const mat2f m) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        return { 
            { m.mat.m128_f32[0], m.mat.m128_f32[2],
              m.mat.m128_f32[1], m.mat.m128_f32[3] }
        };
    }
#endif
    return { _mm_shuffle_ps(m.mat, m.mat, 0b11011000) };
}

/* ------------------------------------------------------------------------------------ */

#ifdef EUCLID_HAS_GLOBAL_OPERATOR_OVERLOAD

EUCLID_FUNC_QUALIFIER mat2i& EUCLID_CALL operator+=(mat2i& a, const mat2i b) noexcept {
    a = addMat2(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER mat2i& EUCLID_CALL operator-=(mat2i& a, const mat2i b) noexcept {
    a = subMat2(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER mat2i& EUCLID_CALL operator*=(mat2i& m, const int v) noexcept {
    m = scaleMat2(m, v);
    return m;
}

EUCLID_FUNC_QUALIFIER mat2i& EUCLID_CALL operator/=(mat2i& m, const int v) noexcept {
    m = divMat2(m, v);
    return m;
}

EUCLID_FUNC_QUALIFIER mat2i  EUCLID_CALL operator-(const mat2i a) noexcept {
    return negateMat2(a);
}

EUCLID_FUNC_QUALIFIER mat2i  EUCLID_CALL operator+(const mat2i a, const mat2i b) noexcept {
    return addMat2(a, b);
}

EUCLID_FUNC_QUALIFIER mat2i  EUCLID_CALL operator-(const mat2i a, const mat2i b) noexcept {
    return subMat2(a, b);
}

EUCLID_FUNC_QUALIFIER mat2i  EUCLID_CALL operator*(const mat2i a, const mat2i b) noexcept {
    return mulMat2(a, b);
}

EUCLID_FUNC_QUALIFIER mat2i  EUCLID_CALL operator*(const mat2i m, const int v) noexcept {
    return scaleMat2(m, v);
}

EUCLID_FUNC_QUALIFIER mat2i  EUCLID_CALL operator/(const mat2i m, const int v) noexcept {
    return divMat2(m, v);
}

EUCLID_FUNC_QUALIFIER mat2i  EUCLID_CALL operator*(const int v, const mat2i m) noexcept {
    return scaleMat2(m, v);
}

/* -------------------------------------------------------------------------------------- */

EUCLID_FUNC_QUALIFIER mat2f& EUCLID_CALL operator+=(mat2f& a, const mat2f b) noexcept {
    a = addMat2(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER mat2f& EUCLID_CALL operator-=(mat2f& a, const mat2f b) noexcept {
    a = subMat2(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER mat2f& EUCLID_CALL operator*=(mat2f& m, const float v) noexcept {
    m = scaleMat2(m, v);
    return m;
}

EUCLID_FUNC_QUALIFIER mat2f& EUCLID_CALL operator/=(mat2f& m, const float v) noexcept {
    m = divMat2(m, v);
    return m;
}

EUCLID_FUNC_QUALIFIER mat2f  EUCLID_CALL operator-(const mat2f a) noexcept {
    return negateMat2(a);
}

EUCLID_FUNC_QUALIFIER mat2f  EUCLID_CALL operator+(const mat2f a, const mat2f b) noexcept {
    return addMat2(a, b);
}

EUCLID_FUNC_QUALIFIER mat2f  EUCLID_CALL operator-(const mat2f a, const mat2f b) noexcept {
    return subMat2(a, b);
}

EUCLID_FUNC_QUALIFIER mat2f  EUCLID_CALL operator*(const mat2f a, const mat2f b) noexcept {
    return mulMat2(a, b);
}

EUCLID_FUNC_QUALIFIER mat2f  EUCLID_CALL operator*(const mat2f m, const float v) noexcept {
    return scaleMat2(m, v);
}

EUCLID_FUNC_QUALIFIER mat2f  EUCLID_CALL operator/(const mat2f m, const float v) noexcept {
    return divMat2(m, v);
}

EUCLID_FUNC_QUALIFIER mat2f  EUCLID_CALL operator*(const float v, const mat2f m) noexcept {
    return scaleMat2(m, v);
}

#endif

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif