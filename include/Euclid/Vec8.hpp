#pragma once

#include "Core.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4514)
// enable /Wall
// C4514 : Ignore the compiler's warning about removing unused inline functions
#endif

namespace euclid {

/* 
   Always use setVec functions to construct a vector 
   Only use initializer list to construct float and double
   type vector if you really want to
*/

EUCLID_FUNC_QUALIFIER vec8i EUCLID_CALL setVec8i(const int a0, const int a1, const int a2, const int a3,
                                                     const int a4, const int a5, const int a6, const int a7) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        return { .m256i_i32 = { a0, a1, a2, a3, a4, a5, a6, a7 } };
    }
#endif
    return _mm256_set_epi32(a7, a6, a5, a4, a3, a2, a1, a0);
}

EUCLID_FUNC_QUALIFIER vec8f EUCLID_CALL setVec8f(const float a0, const float a1, const float a2, const float a3,
                                                     const float a4, const float a5, const float a6, const float a7) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        return { a0, a1, a2, a3, a4, a5, a6, a7 };
    }
#endif
    return _mm256_set_ps(a7, a6, a5, a4, a3, a2, a1, a0);
}

EUCLID_FUNC_QUALIFIER vec8i EUCLID_CALL set1Vec8i(const int val) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        return { .m256i_i32 = { val, val, val, val, val, val, val, val } };
    }
#endif
    return _mm256_set1_epi32(val);
}

EUCLID_FUNC_QUALIFIER vec8f EUCLID_CALL set1Vec8f(const float val) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        return { val, val, val, val, val, val, val, val };
    }
#endif
    return _mm256_set1_ps(val);
}

EUCLID_FUNC_QUALIFIER vec8i EUCLID_CALL setZeroVec8i() noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        return { .m256i_i32 = { 0, 0, 0, 0, 0, 0, 0, 0 } };
    }
#endif
    return _mm256_setzero_si256();
}

EUCLID_FUNC_QUALIFIER vec8f EUCLID_CALL setZeroVec8f() noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        return { 0, 0, 0, 0, 0, 0, 0, 0 };
    }
#endif
    return _mm256_setzero_ps();
}

/* ------------------------------------------------------------------------------- */

/* Type convertion for Vector */

EUCLID_FUNC_QUALIFIER vec8i EUCLID_CALL castVec8fTo8i(const vec8f vec) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec8i result{};
        for (std::size_t i = 0; i < 8; ++i) {
            result.m256i_i32[i] = static_cast<int>(vec.m256_f32[i]);
        }
        return result;
    }
#endif
    return _mm256_cvtps_epi32(vec);
}

EUCLID_FUNC_QUALIFIER vec8f EUCLID_CALL castVec8iTo8f(const vec8i vec) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec8f result{};
        for (std::size_t i = 0; i < 8; ++i) {
            result.m256_f32[i] = static_cast<float>(vec.m256i_i32[i]);
        }
        return result;
    }
#endif
    return _mm256_cvtepi32_ps(vec);
}

/* ------------------------------------------------------------------------------- */

/* Common Arithmetic Operations For Vector */

/* Negate Functions for Vector */

EUCLID_FUNC_QUALIFIER vec8i EUCLID_CALL negateVec8i(const vec8i a) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec8i result{};
        for (std::size_t i = 0; i < 8; ++i) {
            result.m256i_i32[i] = -result.m256i_i32[i];
        }
        return result;
    }
#endif
    return _mm256_sub_epi32(_mm256_setzero_si256(), a);
}

EUCLID_FUNC_QUALIFIER vec8f EUCLID_CALL negateVec8f(const vec8f a) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec8f result{};
        for (std::size_t i = 0; i < 8; ++i) {
            result.m256_f32[i] = -result.m256_f32[i];
        }
        return result;
    }
#endif
    return _mm256_sub_ps(_mm256_setzero_ps(), a);
}

/* Add Functions for Vector */

EUCLID_FUNC_QUALIFIER vec8i EUCLID_CALL addVec8i(const vec8i a, const vec8i b) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec8i result{};
        for (std::size_t i = 0; i < 8; ++i) {
            result.m256i_i32[i] = a.m256i_i32[i] + b.m256i_i32[i];
        }
        return result;
    }
#endif
    return _mm256_add_epi32(a, b);
}

EUCLID_FUNC_QUALIFIER vec8f EUCLID_CALL addVec8f(const vec8f a, const vec8f b) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec8f result{};
        for (std::size_t i = 0; i < 8; ++i) {
            result.m256_f32[i] = a.m256_f32[i] + b.m256_f32[i];
        }
        return result;
    }
#endif
    return _mm256_add_ps(a, b);
}

/* Substract Functions for Vector */

EUCLID_FUNC_QUALIFIER vec8i EUCLID_CALL subVec8i(const vec8i a, const vec8i b) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec8i result{};
        for (std::size_t i = 0; i < 8; ++i) {
            result.m256i_i32[i] = a.m256i_i32[i] - b.m256i_i32[i];
        }
        return result;
    }
#endif
    return _mm256_sub_epi32(a, b);
}

EUCLID_FUNC_QUALIFIER vec8f EUCLID_CALL subVec8f(const vec8f a, const vec8f b) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec8f result{};
        for (std::size_t i = 0; i < 8; ++i) {
            result.m256_f32[i] = a.m256_f32[i] - b.m256_f32[i];
        }
        return result;
    }
#endif
    return _mm256_sub_ps(a, b);
}

/* Multiplication Functions for Vector */

EUCLID_FUNC_QUALIFIER vec8i EUCLID_CALL mulVec8i(const vec8i a, const vec8i b) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec8i result{};
        for (std::size_t i = 0; i < 8; ++i) {
            result.m256i_i32[i] = a.m256i_i32[i] * b.m256i_i32[i];
        }
        return result;
    }
#endif
    return _mm256_mullo_epi32(a, b);
}

EUCLID_FUNC_QUALIFIER vec8f EUCLID_CALL mulVec8f(const vec8f a, const vec8f b) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec8f result{};
        for (std::size_t i = 0; i < 8; ++i) {
            result.m256_f32[i] = a.m256_f32[i] * b.m256_f32[i];
        }
        return result;
    }
#endif
    return _mm256_mul_ps(a, b);
}

/* Division Functions for Vector */

EUCLID_FUNC_QUALIFIER vec8i EUCLID_CALL divVec8i(const vec8i a, const vec8i b) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec8i result{};
        for (std::size_t i = 0; i < 8; ++i) {
            result.m256i_i32[i] = a.m256i_i32[i] / b.m256i_i32[i];
        }
        return result;
    }
    return _mm256_div_epi32(a, b);
#else
    const vec8f float_a = _mm256_cvtepi32_ps(a);
    const vec8f float_b = _mm256_cvtepi32_ps(b);
    const vec8f result  = _mm256_div_ps(float_a, float_b);
    return _mm256_cvtps_epi32(result);
#endif
}

EUCLID_FUNC_QUALIFIER vec8f EUCLID_CALL divVec8f(const vec8f a, const vec8f b) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec8f result{};
        for (std::size_t i = 0; i < 8; ++i) {
            result.m256_f32[i] = a.m256_f32[i] / b.m256_f32[i];
        }
        return result;
    }
#endif
    return _mm256_div_ps(a, b);
}

/* ------------------------------------------------------------------------------- */

#ifdef EUCLID_HAS_OPERATOR_OVERLOAD

/* Add Operator Overload for Vector */

EUCLID_FUNC_QUALIFIER vec8i& EUCLID_CALL operator+=(vec8i& a, const vec8i b) noexcept {
    a = addVec8i(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec8f& EUCLID_CALL operator+=(vec8f& a, const vec8f b) noexcept {
    a = addVec8f(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec8i  EUCLID_CALL operator+(const vec8i a, const vec8i b) noexcept {
    return addVec8i(a, b);
}

EUCLID_FUNC_QUALIFIER vec8f  EUCLID_CALL operator+(const vec8f a, const vec8f b) noexcept {
    return addVec8f(a, b);
}

/* Substract Operator Overload for Vector */

EUCLID_FUNC_QUALIFIER vec8i& EUCLID_CALL operator-=(vec8i& a, const vec8i b) noexcept {
    a = subVec8i(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec8f& EUCLID_CALL operator-=(vec8f& a, const vec8f b) noexcept {
    a = subVec8f(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec8i  EUCLID_CALL operator-(const vec8i a, const vec8i b) noexcept {
    return subVec8i(a, b);
}

EUCLID_FUNC_QUALIFIER vec8f  EUCLID_CALL operator-(const vec8f a, const vec8f b) noexcept {
    return subVec8f(a, b);
}

/* Multiplication Operator Overload for Vector */

EUCLID_FUNC_QUALIFIER vec8i& EUCLID_CALL operator*=(vec8i& a, const vec8i b) noexcept {
    a = mulVec8i(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec8f& EUCLID_CALL operator*=(vec8f& a, const vec8f b) noexcept {
    a = mulVec8f(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec8i  EUCLID_CALL operator*(const vec8i a, const vec8i b) noexcept {
    return mulVec8i(a, b);
}

EUCLID_FUNC_QUALIFIER vec8f  EUCLID_CALL operator*(const vec8f a, const vec8f b) noexcept {
    return mulVec8f(a, b);
}

/* Divsion Operator Overload for Vector */

EUCLID_FUNC_QUALIFIER vec8i& EUCLID_CALL operator/=(vec8i& a, const vec8i b) noexcept {
    a = divVec8i(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec8f& EUCLID_CALL operator/=(vec8f& a, const vec8f b) noexcept {
    a = divVec8f(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec8i  EUCLID_CALL operator/(const vec8i a, const vec8i b) noexcept {
    return divVec8i(a, b);
}

EUCLID_FUNC_QUALIFIER vec8f  EUCLID_CALL operator/(const vec8f a, const vec8f b) noexcept {
    return divVec8f(a, b);
}

#endif // Euclid Operator Overload

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif