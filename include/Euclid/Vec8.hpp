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

/* Get Data from Vector */

EUCLID_FUNC_QUALIFIER int&   getRefDataVec8(vec8i& a, const std::size_t pos) noexcept {
#ifdef _MSC_VER
    return a.m256i_i32[pos];
#else
    int* p = reinterpret_cast<int*>(&a);
    return *(p + pos);
#endif
}

EUCLID_FUNC_QUALIFIER float& getRefDataVec8(vec8f& a, const std::size_t pos) noexcept {
#ifdef _MSC_VER
    return a.m256_f32[pos];
#else
    float* p = reinterpret_cast<float*>(&a);
    return *(p + pos);
#endif
}

EUCLID_FUNC_QUALIFIER int    getDataVec8(vec8i a, const std::size_t pos) noexcept {
    return getRefDataVec8(a, pos);
}

EUCLID_FUNC_QUALIFIER float  getDataVec8(vec8f a, const std::size_t pos) noexcept {
    return getRefDataVec8(a, pos);
}

/* Type Convertion for Vector */

EUCLID_FUNC_QUALIFIER vec8i EUCLID_CALL castToVec8i(const vec8f vec) noexcept {
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

EUCLID_FUNC_QUALIFIER vec8f EUCLID_CALL castToVec8f(const vec8i vec) noexcept {
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

EUCLID_FUNC_QUALIFIER vec8i EUCLID_CALL negateVec8(const vec8i a) noexcept {
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

EUCLID_FUNC_QUALIFIER vec8f EUCLID_CALL negateVec8(const vec8f a) noexcept {
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

EUCLID_FUNC_QUALIFIER vec8i EUCLID_CALL addVec8(const vec8i a, const vec8i b) noexcept {
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

EUCLID_FUNC_QUALIFIER vec8f EUCLID_CALL addVec8(const vec8f a, const vec8f b) noexcept {
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

EUCLID_FUNC_QUALIFIER vec8i EUCLID_CALL subVec8(const vec8i a, const vec8i b) noexcept {
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

EUCLID_FUNC_QUALIFIER vec8f EUCLID_CALL subVec8(const vec8f a, const vec8f b) noexcept {
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

EUCLID_FUNC_QUALIFIER vec8i EUCLID_CALL mulVec8(const vec8i a, const vec8i b) noexcept {
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

EUCLID_FUNC_QUALIFIER vec8f EUCLID_CALL mulVec8(const vec8f a, const vec8f b) noexcept {
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

EUCLID_FUNC_QUALIFIER vec8i EUCLID_CALL divVec8(const vec8i a, const vec8i b) noexcept {
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

EUCLID_FUNC_QUALIFIER vec8f EUCLID_CALL divVec8(const vec8f a, const vec8f b) noexcept {
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

/* Scale Functions for Vector */

EUCLID_FUNC_QUALIFIER vec8i EUCLID_CALL scaleVec8(const vec8i vec, const int val) noexcept {
    return mulVec8(vec, set1Vec8i(val));
}

EUCLID_FUNC_QUALIFIER vec8f EUCLID_CALL scaleVec8(const vec8f vec, const float val) noexcept {
    return mulVec8(vec, set1Vec8f(val));
}

/* ------------------------------------------------------------------------------- */

#ifdef EUCLID_HAS_GLOBAL_OPERATOR_OVERLOAD

/* Negate Operator Overload for Vector */

EUCLID_FUNC_QUALIFIER vec8i EUCLID_CALL operator-(const vec8i a) noexcept {
    return negateVec8(a);
}

EUCLID_FUNC_QUALIFIER vec8f EUCLID_CALL operator-(const vec8f a) noexcept {
    return negateVec8(a);
}

/* Add Operator Overload for Vector */

EUCLID_FUNC_QUALIFIER vec8i& EUCLID_CALL operator+=(vec8i& a, const vec8i b) noexcept {
    a = addVec8(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec8f& EUCLID_CALL operator+=(vec8f& a, const vec8f b) noexcept {
    a = addVec8(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec8i  EUCLID_CALL operator+(const vec8i a, const vec8i b) noexcept {
    return addVec8(a, b);
}

EUCLID_FUNC_QUALIFIER vec8f  EUCLID_CALL operator+(const vec8f a, const vec8f b) noexcept {
    return addVec8(a, b);
}

/* Substract Operator Overload for Vector */

EUCLID_FUNC_QUALIFIER vec8i& EUCLID_CALL operator-=(vec8i& a, const vec8i b) noexcept {
    a = subVec8(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec8f& EUCLID_CALL operator-=(vec8f& a, const vec8f b) noexcept {
    a = subVec8(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec8i  EUCLID_CALL operator-(const vec8i a, const vec8i b) noexcept {
    return subVec8(a, b);
}

EUCLID_FUNC_QUALIFIER vec8f  EUCLID_CALL operator-(const vec8f a, const vec8f b) noexcept {
    return subVec8(a, b);
}

/* Multiplication Operator Overload for Vector */

EUCLID_FUNC_QUALIFIER vec8i& EUCLID_CALL operator*=(vec8i& a, const vec8i b) noexcept {
    a = mulVec8(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec8f& EUCLID_CALL operator*=(vec8f& a, const vec8f b) noexcept {
    a = mulVec8(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec8i  EUCLID_CALL operator*(const vec8i a, const vec8i b) noexcept {
    return mulVec8(a, b);
}

EUCLID_FUNC_QUALIFIER vec8f  EUCLID_CALL operator*(const vec8f a, const vec8f b) noexcept {
    return mulVec8(a, b);
}

/* Divsion Operator Overload for Vector */

EUCLID_FUNC_QUALIFIER vec8i& EUCLID_CALL operator/=(vec8i& a, const vec8i b) noexcept {
    a = divVec8(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec8f& EUCLID_CALL operator/=(vec8f& a, const vec8f b) noexcept {
    a = divVec8(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec8i  EUCLID_CALL operator/(const vec8i a, const vec8i b) noexcept {
    return divVec8(a, b);
}

EUCLID_FUNC_QUALIFIER vec8f  EUCLID_CALL operator/(const vec8f a, const vec8f b) noexcept {
    return divVec8(a, b);
}

/* Scale Operator Overload for Vector */

EUCLID_FUNC_QUALIFIER vec8i& EUCLID_CALL operator*=(vec8i& vec, const int val) noexcept {
    vec = scaleVec8(vec, val);
    return vec;
}

EUCLID_FUNC_QUALIFIER vec8f& EUCLID_CALL operator*=(vec8f& vec, const float val) noexcept {
    vec = scaleVec8(vec, val);
    return vec;
}

EUCLID_FUNC_QUALIFIER vec8i  EUCLID_CALL operator*(const vec8i vec, const int val) noexcept {
    return scaleVec8(vec, val);
}

EUCLID_FUNC_QUALIFIER vec8f  EUCLID_CALL operator*(const vec8f vec, const float val) noexcept {
    return scaleVec8(vec, val);
}

EUCLID_FUNC_QUALIFIER vec8i  EUCLID_CALL operator*(const int val, const vec8i vec) noexcept {
    return scaleVec8(vec, val);
}

EUCLID_FUNC_QUALIFIER vec8f  EUCLID_CALL operator*(const float val, const vec8f vec) noexcept {
    return scaleVec8(vec, val);
}

/* ------------------------------------------------------------------------------- */

EUCLID_FUNC_QUALIFIER vec8i& EUCLID_CALL operator/=(vec8i& vec, const int val) noexcept {
    vec = divVec8(vec, set1Vec8i(val));
    return vec;
}

EUCLID_FUNC_QUALIFIER vec8f& EUCLID_CALL operator/=(vec8f& vec, const float val) noexcept {
    vec = divVec8(vec, set1Vec8f(val));
    return vec;
}

EUCLID_FUNC_QUALIFIER vec8i  EUCLID_CALL operator/(const vec8i vec, const int val) noexcept {
    return divVec8(vec, set1Vec8i(val));
}

EUCLID_FUNC_QUALIFIER vec8f  EUCLID_CALL operator/(const vec8f vec, const float val) noexcept {
    return divVec8(vec, set1Vec8f(val));
}

#endif // Euclid Operator Overload

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif