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

EUCLID_FUNC_QUALIFIER vec4i EUCLID_CALL setVec4i(const int x, const int y, const int z, const int w) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        return { .m128i_i32 = { x, y, z, w } };
    }
#endif
    return _mm_set_epi32(w, z, y, x);
}

EUCLID_FUNC_QUALIFIER vec4f EUCLID_CALL setVec4f(const float x, const float y, const float z, const float w) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        return { x, y, z, w };
    }
#endif
    return _mm_set_ps(w, z, y, x);
}

EUCLID_FUNC_QUALIFIER vec4d EUCLID_CALL setVec4d(const double x, const double y, const double z, const double w) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        return { x, y, z, w };
    }
#endif
    return _mm256_set_pd(w, z, y, x);
}

EUCLID_FUNC_QUALIFIER vec4i EUCLID_CALL set1Vec4i(const int val) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        return { .m128i_i32 = { val, val, val, val } };
    }
#endif
    return _mm_set1_epi32(val);
}

EUCLID_FUNC_QUALIFIER vec4f EUCLID_CALL set1Vec4f(const float val) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        return { val, val, val, val };
    }
#endif
    return _mm_set1_ps(val);
}

EUCLID_FUNC_QUALIFIER vec4d EUCLID_CALL set1Vec4d(const double val) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        return { val, val, val, val };
    }
#endif
    return _mm256_set1_pd(val);
}

EUCLID_FUNC_QUALIFIER vec4i EUCLID_CALL setZeroVec4i() noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        return { .m128i_i32 = { 0, 0, 0, 0 } };
    }
#endif
    return _mm_setzero_si128();
}

EUCLID_FUNC_QUALIFIER vec4f EUCLID_CALL setZeroVec4f() noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        return { 0, 0, 0, 0 };
    }
#endif
    return _mm_setzero_ps();
}

EUCLID_FUNC_QUALIFIER vec4d EUCLID_CALL setZeroVec4d() noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        return { 0, 0, 0, 0 };
    }
#endif
    return _mm256_setzero_pd();
}

/* ------------------------------------------------------------------------------- */

/* Type convertion for Vector */

EUCLID_FUNC_QUALIFIER vec4i EUCLID_CALL castVec4fTo4i(const vec4f vec) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec4i result{};
        for (std::size_t i = 0; i < 4; ++i) {
            result.m128i_i32[i] = static_cast<int>(vec.m128_f32[i]);
        }
        return result;
    }
#endif
    return _mm_cvtps_epi32(vec);
}

EUCLID_FUNC_QUALIFIER vec4i EUCLID_CALL castVec4dTo4i(const vec4d vec) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec4i result{};
        for (std::size_t i = 0; i < 4; ++i) {
            result.m128i_i32[i] = static_cast<int>(vec.m256d_f64[i]);
        }
        return result;
    }
#endif
    return _mm256_cvtpd_epi32(vec);
}

EUCLID_FUNC_QUALIFIER vec4f EUCLID_CALL castVec4iTo4f(const vec4i vec) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec4f result{};
        for (std::size_t i = 0; i < 4; ++i) {
            result.m128_f32[i] = static_cast<float>(vec.m128i_i32[i]);
        }
        return result;
    }
#endif
    return _mm_cvtepi32_ps(vec);
}

EUCLID_FUNC_QUALIFIER vec4f EUCLID_CALL castVec4dTo4f(const vec4d vec) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec4f result{};
        for (std::size_t i = 0; i < 4; ++i) {
            result.m128_f32[i] = static_cast<float>(vec.m256d_f64[i]);
        }
        return result;
    }
#endif
    return _mm256_cvtpd_ps(vec);
}

EUCLID_FUNC_QUALIFIER vec4d EUCLID_CALL castVec4dTo4f(const vec4i vec) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec4d result{};
        for (std::size_t i = 0; i < 4; ++i) {
            result.m256d_f64[i] = static_cast<double>(vec.m128i_i32[i]);
        }
        return result;
    }
#endif
    return _mm256_cvtepi32_pd(vec);
}

EUCLID_FUNC_QUALIFIER vec4d EUCLID_CALL castVec4dTo4f(const vec4f vec) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec4d result{};
        for (std::size_t i = 0; i < 4; ++i) {
            result.m256d_f64[i] = static_cast<double>(vec.m128_f32[i]);
        }
        return result;
    }
#endif
    return _mm256_cvtps_pd(vec);
}

/* ------------------------------------------------------------------------------- */

/* Common Arithmetic Operations For Vector */

/* Negate Functions for Vector */

EUCLID_FUNC_QUALIFIER vec4i EUCLID_CALL negateVec4i(const vec4i a) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec4i result{};
        for (std::size_t i = 0; i < 4; ++i) {
            result.m128i_i32[i] = -result.m128i_i32[i];
        }
        return result;
    }
#endif
    return _mm_sub_epi32(_mm_setzero_si128(), a);
}

EUCLID_FUNC_QUALIFIER vec4f EUCLID_CALL negateVec4f(const vec4f a) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec4f result{};
        for (std::size_t i = 0; i < 4; ++i) {
            result.m128_f32[i] = -result.m128_f32[i];
        }
        return result;
    }
#endif
    return _mm_sub_ps(_mm_setzero_ps(), a);
}

EUCLID_FUNC_QUALIFIER vec4d EUCLID_CALL negateVec4d(const vec4d a) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec4d result{};
        for (std::size_t i = 0; i < 4; ++i) {
            result.m256d_f64[i] = -result.m256d_f64[i];
        }
        return result;
    }
#endif
    return _mm256_sub_pd(_mm256_setzero_pd(), a);
}

/* Add Functions for Vector */

EUCLID_FUNC_QUALIFIER vec4i EUCLID_CALL addVec4i(const vec4i a, const vec4i b) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec4i result{};
        for (std::size_t i = 0; i < 4; ++i) {
            result.m128i_i32[i] = a.m128i_i32[i] + b.m128i_i32[i];
        }
        return result;
    }
#endif
    return _mm_add_epi32(a, b);
}

EUCLID_FUNC_QUALIFIER vec4f EUCLID_CALL addVec4f(const vec4f a, const vec4f b) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec4f result{};
        for (std::size_t i = 0; i < 4; ++i) {
            result.m128_f32[i] = a.m128_f32[i] + b.m128_f32[i];
        }
        return result;
    }
#endif
    return _mm_add_ps(a, b);
}

EUCLID_FUNC_QUALIFIER vec4d EUCLID_CALL addVec4d(const vec4d a, const vec4d b) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec4d result{};
        for (std::size_t i = 0; i < 4; ++i) {
            result.m256d_f64[i] = a.m256d_f64[i] + b.m256d_f64[i];
        }
        return result;
    }
#endif
    return _mm256_add_pd(a, b);
}

/* Substract Functions for Vector */

EUCLID_FUNC_QUALIFIER vec4i EUCLID_CALL subVec4i(const vec4i a, const vec4i b) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec4i result{};
        for (std::size_t i = 0; i < 4; ++i) {
            result.m128i_i32[i] = a.m128i_i32[i] - b.m128i_i32[i];
        }
        return result;
    }
#endif
    return _mm_sub_epi32(a, b);
}

EUCLID_FUNC_QUALIFIER vec4f EUCLID_CALL subVec4f(const vec4f a, const vec4f b) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec4f result{};
        for (std::size_t i = 0; i < 4; ++i) {
            result.m128_f32[i] = a.m128_f32[i] - b.m128_f32[i];
        }
        return result;
    }
#endif
    return _mm_sub_ps(a, b);
}

EUCLID_FUNC_QUALIFIER vec4d EUCLID_CALL subVec4d(const vec4d a, const vec4d b) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec4d result{};
        for (std::size_t i = 0; i < 4; ++i) {
            result.m256d_f64[i] = a.m256d_f64[i] - b.m256d_f64[i];
        }
        return result;
    }
#endif
    return _mm256_sub_pd(a, b);
}

/* Multiplication Functions for Vector */

EUCLID_FUNC_QUALIFIER vec4i EUCLID_CALL mulVec4i(const vec4i a, const vec4i b) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec4i result{};
        for (std::size_t i = 0; i < 4; ++i) {
            result.m128i_i32[i] = a.m128i_i32[i] * b.m128i_i32[i];
        }
        return result;
    }
#endif
    return _mm_mullo_epi32(a, b);
}

EUCLID_FUNC_QUALIFIER vec4f EUCLID_CALL mulVec4f(const vec4f a, const vec4f b) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec4f result{};
        for (std::size_t i = 0; i < 4; ++i) {
            result.m128_f32[i] = a.m128_f32[i] * b.m128_f32[i];
        }
        return result;
    }
#endif
    return _mm_mul_ps(a, b);
}

EUCLID_FUNC_QUALIFIER vec4d EUCLID_CALL mulVec4d(const vec4d a, const vec4d b) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec4d result{};
        for (std::size_t i = 0; i < 4; ++i) {
            result.m256d_f64[i] = a.m256d_f64[i] * b.m256d_f64[i];
        }
        return result;
    }
#endif
    return _mm256_mul_pd(a, b);
}

/* Division Functions for Vector */

EUCLID_FUNC_QUALIFIER vec4i EUCLID_CALL divVec4i(const vec4i a, const vec4i b) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec4i result{};
        for (std::size_t i = 0; i < 4; ++i) {
            result.m128i_i32[i] = a.m128i_i32[i] / b.m128i_i32[i];
        }
        return result;
    }
    return _mm_div_epi32(a, b);
#else
    const vec4f float_a = _mm_cvtepi32_ps(a);
    const vec4f float_b = _mm_cvtepi32_ps(b);
    const vec4f result  = _mm_div_ps(float_a, float_b);
    return _mm_cvtps_epi32(result);
#endif
}

EUCLID_FUNC_QUALIFIER vec4f EUCLID_CALL divVec4f(const vec4f a, const vec4f b) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec4f result{};
        for (std::size_t i = 0; i < 4; ++i) {
            result.m128_f32[i] = a.m128_f32[i] / b.m128_f32[i];
        }
        return result;
    }
#endif
    return _mm_div_ps(a, b);
}

EUCLID_FUNC_QUALIFIER vec4d EUCLID_CALL divVec4d(const vec4d a, const vec4d b) noexcept {
#ifdef _MSC_VER
    if (__builtin_is_constant_evaluated()) {
        vec4d result{};
        for (std::size_t i = 0; i < 4; ++i) {
            result.m256d_f64[i] = a.m256d_f64[i] / b.m256d_f64[i];
        }
        return result;
    }
#endif
    return _mm256_div_pd(a, b);
}

/* ------------------------------------------------------------------------------- */

#ifdef EUCLID_HAS_OPERATOR_OVERLOAD

/* Add Operator Overload for Vector */

EUCLID_FUNC_QUALIFIER vec4i& EUCLID_CALL operator+=(vec4i& a, const vec4i b) noexcept {
    a = addVec4i(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec4f& EUCLID_CALL operator+=(vec4f& a, const vec4f b) noexcept {
    a = addVec4f(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec4d& EUCLID_CALL operator+=(vec4d& a, const vec4d b) noexcept {
    a = addVec4d(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec4i  EUCLID_CALL operator+(const vec4i a, const vec4i b) noexcept {
    return addVec4i(a, b);
}

EUCLID_FUNC_QUALIFIER vec4f  EUCLID_CALL operator+(const vec4f a, const vec4f b) noexcept {
    return addVec4f(a, b);
}

EUCLID_FUNC_QUALIFIER vec4d  EUCLID_CALL operator+(const vec4d a, const vec4d b) noexcept {
    return addVec4d(a, b);
}

/* Substract Operator Overload for Vector */

EUCLID_FUNC_QUALIFIER vec4i& EUCLID_CALL operator-=(vec4i& a, const vec4i b) noexcept {
    a = subVec4i(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec4f& EUCLID_CALL operator-=(vec4f& a, const vec4f b) noexcept {
    a = subVec4f(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec4d& EUCLID_CALL operator-=(vec4d& a, const vec4d b) noexcept {
    a = subVec4d(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec4i  EUCLID_CALL operator-(const vec4i a, const vec4i b) noexcept {
    return subVec4i(a, b);
}

EUCLID_FUNC_QUALIFIER vec4f  EUCLID_CALL operator-(const vec4f a, const vec4f b) noexcept {
    return subVec4f(a, b);
}

EUCLID_FUNC_QUALIFIER vec4d  EUCLID_CALL operator-(const vec4d a, const vec4d b) noexcept {
    return subVec4d(a, b);
}

/* Multiplication Operator Overload for Vector */

EUCLID_FUNC_QUALIFIER vec4i& EUCLID_CALL operator*=(vec4i& a, const vec4i b) noexcept {
    a = mulVec4i(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec4f& EUCLID_CALL operator*=(vec4f& a, const vec4f b) noexcept {
    a = mulVec4f(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec4d& EUCLID_CALL operator*=(vec4d& a, const vec4d b) noexcept {
    a = mulVec4d(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec4i  EUCLID_CALL operator*(const vec4i a, const vec4i b) noexcept {
    return mulVec4i(a, b);
}

EUCLID_FUNC_QUALIFIER vec4f  EUCLID_CALL operator*(const vec4f a, const vec4f b) noexcept {
    return mulVec4f(a, b);
}

EUCLID_FUNC_QUALIFIER vec4d  EUCLID_CALL operator*(const vec4d a, const vec4d b) noexcept {
    return mulVec4d(a, b);
}

/* Divsion Operator Overload for Vector */

EUCLID_FUNC_QUALIFIER vec4i& EUCLID_CALL operator/=(vec4i& a, const vec4i b) noexcept {
    a = divVec4i(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec4f& EUCLID_CALL operator/=(vec4f& a, const vec4f b) noexcept {
    a = divVec4f(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec4d& EUCLID_CALL operator/=(vec4d& a, const vec4d b) noexcept {
    a = divVec4d(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec4i  EUCLID_CALL operator/(const vec4i a, const vec4i b) noexcept {
    return divVec4i(a, b);
}

EUCLID_FUNC_QUALIFIER vec4f  EUCLID_CALL operator/(const vec4f a, const vec4f b) noexcept {
    return divVec4f(a, b);
}

EUCLID_FUNC_QUALIFIER vec4d  EUCLID_CALL operator/(const vec4d a, const vec4d b) noexcept {
    return divVec4d(a, b);
}

#endif // Euclid Operator Overload

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif