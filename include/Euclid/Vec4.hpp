#pragma once

#include "Core.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4514)
// enable /Wall
// C4514 : Ignore the compiler's warning about removing unused inline functions
#endif

#define EUCLID_SHUFFLE_MASK(x, y, z, w) (((w) << 6) | ((z) << 4) | ((y) << 2) | ((x)))

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

/* Get Data from Vector */

EUCLID_FUNC_QUALIFIER int&    EUCLID_CALL getRefDataVec4(vec4i& a, const std::size_t pos) noexcept {
#ifdef _MSC_VER
    return a.m128i_i32[pos];
#else
    int* p = reinterpret_cast<int*>(&a);
    return *(p + pos);
#endif
}

EUCLID_FUNC_QUALIFIER float&  EUCLID_CALL getRefDataVec4(vec4f& a, const std::size_t pos) noexcept {
#ifdef _MSC_VER
    return a.m128_f32[pos];
#else
    float* p = reinterpret_cast<float*>(&a);
    return *(p + pos);
#endif
}

EUCLID_FUNC_QUALIFIER double& EUCLID_CALL getRefDataVec4(vec4d& a, const std::size_t pos) noexcept {
#ifdef _MSC_VER
    return a.m256d_f64[pos];
#else
    double* p = reinterpret_cast<double*>(&a);
    return *(p + pos);
#endif
}

EUCLID_FUNC_QUALIFIER int    EUCLID_CALL getDataVec4(vec4i a, const std::size_t pos) noexcept {
    return getRefDataVec4(a, pos);
}

EUCLID_FUNC_QUALIFIER float  EUCLID_CALL getDataVec4(vec4f a, const std::size_t pos) noexcept {
    return getRefDataVec4(a, pos);
}

EUCLID_FUNC_QUALIFIER double EUCLID_CALL getDataVec4(vec4d a, const std::size_t pos) noexcept {
    return getRefDataVec4(a, pos);
}

/* ------------------------------------------------------------------------------- */

/* Type Convertion for Vector */

EUCLID_FUNC_QUALIFIER vec4i EUCLID_CALL castToVec4i(const vec4f vec) noexcept {
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

EUCLID_FUNC_QUALIFIER vec4i EUCLID_CALL castToVec4i(const vec4d vec) noexcept {
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

EUCLID_FUNC_QUALIFIER vec4f EUCLID_CALL castToVec4f(const vec4i vec) noexcept {
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

EUCLID_FUNC_QUALIFIER vec4f EUCLID_CALL castToVec4f(const vec4d vec) noexcept {
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

EUCLID_FUNC_QUALIFIER vec4d EUCLID_CALL castToVec4d(const vec4i vec) noexcept {
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

EUCLID_FUNC_QUALIFIER vec4d EUCLID_CALL castToVec4d(const vec4f vec) noexcept {
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

EUCLID_FUNC_QUALIFIER vec4i EUCLID_CALL negateVec4(const vec4i a) noexcept {
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

EUCLID_FUNC_QUALIFIER vec4f EUCLID_CALL negateVec4(const vec4f a) noexcept {
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

EUCLID_FUNC_QUALIFIER vec4d EUCLID_CALL negateVec4(const vec4d a) noexcept {
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

EUCLID_FUNC_QUALIFIER vec4i EUCLID_CALL addVec4(const vec4i a, const vec4i b) noexcept {
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

EUCLID_FUNC_QUALIFIER vec4f EUCLID_CALL addVec4(const vec4f a, const vec4f b) noexcept {
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

EUCLID_FUNC_QUALIFIER vec4d EUCLID_CALL addVec4(const vec4d a, const vec4d b) noexcept {
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

EUCLID_FUNC_QUALIFIER vec4i EUCLID_CALL subVec4(const vec4i a, const vec4i b) noexcept {
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

EUCLID_FUNC_QUALIFIER vec4f EUCLID_CALL subVec4(const vec4f a, const vec4f b) noexcept {
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

EUCLID_FUNC_QUALIFIER vec4d EUCLID_CALL subVec4(const vec4d a, const vec4d b) noexcept {
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

EUCLID_FUNC_QUALIFIER vec4i EUCLID_CALL mulVec4(const vec4i a, const vec4i b) noexcept {
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

EUCLID_FUNC_QUALIFIER vec4f EUCLID_CALL mulVec4(const vec4f a, const vec4f b) noexcept {
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

EUCLID_FUNC_QUALIFIER vec4d EUCLID_CALL mulVec4(const vec4d a, const vec4d b) noexcept {
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

EUCLID_FUNC_QUALIFIER vec4i EUCLID_CALL divVec4(const vec4i a, const vec4i b) noexcept {
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

EUCLID_FUNC_QUALIFIER vec4f EUCLID_CALL divVec4(const vec4f a, const vec4f b) noexcept {
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

EUCLID_FUNC_QUALIFIER vec4d EUCLID_CALL divVec4(const vec4d a, const vec4d b) noexcept {
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

/* Scale Functions for Vector */

EUCLID_FUNC_QUALIFIER vec4i EUCLID_CALL scaleVec4(const vec4i vec, const int val) noexcept {
    return mulVec4(vec, set1Vec4i(val));
}

EUCLID_FUNC_QUALIFIER vec4f EUCLID_CALL scaleVec4(const vec4f vec, const float val) noexcept {
    return mulVec4(vec, set1Vec4f(val));
}

EUCLID_FUNC_QUALIFIER vec4d EUCLID_CALL scaleVec4(const vec4d vec, const double val) noexcept {
    return mulVec4(vec, set1Vec4d(val));
}

/* ------------------------------------------------------------------------------- */

#ifdef EUCLID_HAS_GLOBAL_OPERATOR_OVERLOAD

/* Negate Operator Overload for Vector */

EUCLID_FUNC_QUALIFIER vec4i  EUCLID_CALL operator-(const vec4i a) noexcept {
    return negateVec4(a);
}

EUCLID_FUNC_QUALIFIER vec4f  EUCLID_CALL operator-(const vec4f a) noexcept {
    return negateVec4(a);
}

EUCLID_FUNC_QUALIFIER vec4d  EUCLID_CALL operator-(const vec4d a) noexcept {
    return negateVec4(a);
}

/* Add Operator Overload for Vector */

EUCLID_FUNC_QUALIFIER vec4i& EUCLID_CALL operator+=(vec4i& a, const vec4i b) noexcept {
    a = addVec4(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec4f& EUCLID_CALL operator+=(vec4f& a, const vec4f b) noexcept {
    a = addVec4(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec4d& EUCLID_CALL operator+=(vec4d& a, const vec4d b) noexcept {
    a = addVec4(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec4i  EUCLID_CALL operator+(const vec4i a, const vec4i b) noexcept {
    return addVec4(a, b);
}

EUCLID_FUNC_QUALIFIER vec4f  EUCLID_CALL operator+(const vec4f a, const vec4f b) noexcept {
    return addVec4(a, b);
}

EUCLID_FUNC_QUALIFIER vec4d  EUCLID_CALL operator+(const vec4d a, const vec4d b) noexcept {
    return addVec4(a, b);
}

/* Substract Operator Overload for Vector */

EUCLID_FUNC_QUALIFIER vec4i& EUCLID_CALL operator-=(vec4i& a, const vec4i b) noexcept {
    a = subVec4(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec4f& EUCLID_CALL operator-=(vec4f& a, const vec4f b) noexcept {
    a = subVec4(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec4d& EUCLID_CALL operator-=(vec4d& a, const vec4d b) noexcept {
    a = subVec4(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec4i  EUCLID_CALL operator-(const vec4i a, const vec4i b) noexcept {
    return subVec4(a, b);
}

EUCLID_FUNC_QUALIFIER vec4f  EUCLID_CALL operator-(const vec4f a, const vec4f b) noexcept {
    return subVec4(a, b);
}

EUCLID_FUNC_QUALIFIER vec4d  EUCLID_CALL operator-(const vec4d a, const vec4d b) noexcept {
    return subVec4(a, b);
}

/* Multiplication Operator Overload for Vector */

EUCLID_FUNC_QUALIFIER vec4i& EUCLID_CALL operator*=(vec4i& a, const vec4i b) noexcept {
    a = mulVec4(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec4f& EUCLID_CALL operator*=(vec4f& a, const vec4f b) noexcept {
    a = mulVec4(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec4d& EUCLID_CALL operator*=(vec4d& a, const vec4d b) noexcept {
    a = mulVec4(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec4i  EUCLID_CALL operator*(const vec4i a, const vec4i b) noexcept {
    return mulVec4(a, b);
}

EUCLID_FUNC_QUALIFIER vec4f  EUCLID_CALL operator*(const vec4f a, const vec4f b) noexcept {
    return mulVec4(a, b);
}

EUCLID_FUNC_QUALIFIER vec4d  EUCLID_CALL operator*(const vec4d a, const vec4d b) noexcept {
    return mulVec4(a, b);
}

/* Divsion Operator Overload for Vector */

EUCLID_FUNC_QUALIFIER vec4i& EUCLID_CALL operator/=(vec4i& a, const vec4i b) noexcept {
    a = divVec4(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec4f& EUCLID_CALL operator/=(vec4f& a, const vec4f b) noexcept {
    a = divVec4(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec4d& EUCLID_CALL operator/=(vec4d& a, const vec4d b) noexcept {
    a = divVec4(a, b);
    return a;
}

EUCLID_FUNC_QUALIFIER vec4i  EUCLID_CALL operator/(const vec4i a, const vec4i b) noexcept {
    return divVec4(a, b);
}

EUCLID_FUNC_QUALIFIER vec4f  EUCLID_CALL operator/(const vec4f a, const vec4f b) noexcept {
    return divVec4(a, b);
}

EUCLID_FUNC_QUALIFIER vec4d  EUCLID_CALL operator/(const vec4d a, const vec4d b) noexcept {
    return divVec4(a, b);
}

/* Scale Operator Overload for Vector */

EUCLID_FUNC_QUALIFIER vec4i& EUCLID_CALL operator*=(vec4i& vec, const int val) noexcept {
    vec = scaleVec4(vec, val);
    return vec;
}

EUCLID_FUNC_QUALIFIER vec4f& EUCLID_CALL operator*=(vec4f& vec, const float val) noexcept {
    vec = scaleVec4(vec, val);
    return vec;
}

EUCLID_FUNC_QUALIFIER vec4d& EUCLID_CALL operator*=(vec4d& vec, const double val) noexcept {
    vec = scaleVec4(vec, val);
    return vec;
}

EUCLID_FUNC_QUALIFIER vec4i  EUCLID_CALL operator*(const vec4i vec, const int val) noexcept {
    return scaleVec4(vec, val);
}

EUCLID_FUNC_QUALIFIER vec4f  EUCLID_CALL operator*(const vec4f vec, const float val) noexcept {
    return scaleVec4(vec, val);
}

EUCLID_FUNC_QUALIFIER vec4d  EUCLID_CALL operator*(const vec4d vec, const double val) noexcept {
    return scaleVec4(vec, val);
}

EUCLID_FUNC_QUALIFIER vec4i  EUCLID_CALL operator*(const int val, const vec4i vec) noexcept {
    return scaleVec4(vec, val);
}

EUCLID_FUNC_QUALIFIER vec4f  EUCLID_CALL operator*(const float val, const vec4f vec) noexcept {
    return scaleVec4(vec, val);
}

EUCLID_FUNC_QUALIFIER vec4d  EUCLID_CALL operator*(const double val, const vec4d vec) noexcept {
    return scaleVec4(vec, val);
}

/* ------------------------------------------------------------------------------- */

EUCLID_FUNC_QUALIFIER vec4i& EUCLID_CALL operator/=(vec4i& vec, const int val) noexcept {
    vec = divVec4(vec, set1Vec4i(val));
    return vec;
}

EUCLID_FUNC_QUALIFIER vec4f& EUCLID_CALL operator/=(vec4f& vec, const float val) noexcept {
    vec = divVec4(vec, set1Vec4f(val));
    return vec;
}

EUCLID_FUNC_QUALIFIER vec4d& EUCLID_CALL operator/=(vec4d& vec, const double val) noexcept {
    vec = divVec4(vec, set1Vec4d(val));
    return vec;
}

EUCLID_FUNC_QUALIFIER vec4i  EUCLID_CALL operator/(const vec4i vec, const int val) noexcept {
    return divVec4(vec, set1Vec4i(val));
}

EUCLID_FUNC_QUALIFIER vec4f  EUCLID_CALL operator/(const vec4f vec, const float val) noexcept {
    return divVec4(vec, set1Vec4f(val));
}

EUCLID_FUNC_QUALIFIER vec4d  EUCLID_CALL operator/(const vec4d vec, const double val) noexcept {
    return divVec4(vec, set1Vec4d(val));
}

#endif // Euclid Operator Overload

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif