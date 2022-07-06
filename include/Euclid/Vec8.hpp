#pragma once

#include "Vec4.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4514 5246)
// enable /Wall
// C4514: remove unused inline functions
// C5246: the initialization of a subobject should be wrapped in braces
#endif

namespace euclid {

EUCLID_QUALIFIER float getVec8Data(const Vec8& a, std::size_t pos) noexcept {
#if defined(_MSC_VER) && !defined(__clang__)
    return a.m256_f32[pos];
#else
    return a.v[pos];
#endif
}

EUCLID_QUALIFIER float& getVec8Data(Vec8& a, std::size_t pos) noexcept {
#if defined(_MSC_VER) && !defined(__clang__)
    return a.m256_f32[pos];
#elif __clang__
    return *(reinterpret_cast<float*>(&a) + pos);
#else
    return a.v[pos];
#endif
}

// it's recommended to use this function to create Vec8 rather than using the list-initialization
constexpr Vec8 EUCLID_CALL setVec8(float x, float y, float z,float w, float a, float b, float c, float d) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return { x,y,z,w,a,b,c,d };
    }
    return { _mm256_set_ps(d,c,b,a,w,z,y,x) };
}

constexpr Vec8 EUCLID_CALL set1Vec8(float v) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return { v,v,v,v,v,v,v,v };
    }
    return { _mm256_set1_ps(v) };
}

constexpr Vec8 EUCLID_CALL setZeroVec8() noexcept {
    if (__builtin_is_constant_evaluated()) {
        return { 0,0,0,0,0,0,0,0 };
    }
    return { _mm256_setzero_ps() };
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL getVec8LowerVec4(Vec8 a) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return { 
            getVec8Data(a, 0),
            getVec8Data(a, 1),
            getVec8Data(a, 2),
            getVec8Data(a, 3)
        };
    }
    return { _mm256_extractf128_ps(a, 0) };
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL getVec8UpperVec4(Vec8 a) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return {
            getVec8Data(a, 4),
            getVec8Data(a, 5),
            getVec8Data(a, 6),
            getVec8Data(a, 7)
        };
    }
    return { _mm256_extractf128_ps(a, 1) };
}

EUCLID_QUALIFIER Vec8 EUCLID_CALL toVec8(Vec4 a, Vec4 b) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return {
            getVec4Data(a, 0), getVec4Data(a, 1),
            getVec4Data(a, 2), getVec4Data(a, 3),
            getVec4Data(b, 0), getVec4Data(b, 1),
            getVec4Data(b, 2), getVec4Data(b, 3)
        };
    }
    return { _mm256_set_m128(a, b) };
}

EUCLID_QUALIFIER Vec8 EUCLID_CALL operator+(Vec8 a, Vec8 b) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return {
            getVec8Data(a, 0) + getVec8Data(b, 0),
            getVec8Data(a, 1) + getVec8Data(b, 1),
            getVec8Data(a, 2) + getVec8Data(b, 2),
            getVec8Data(a, 3) + getVec8Data(b, 3),
            getVec8Data(a, 4) + getVec8Data(b, 4),
            getVec8Data(a, 5) + getVec8Data(b, 5),
            getVec8Data(a, 6) + getVec8Data(b, 6),
            getVec8Data(a, 7) + getVec8Data(b, 7)
        };
    }
    return { _mm256_add_ps(a, b) };
}

EUCLID_QUALIFIER Vec8 EUCLID_CALL operator-(Vec8 a, Vec8 b) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return {
            getVec8Data(a, 0) - getVec8Data(b, 0),
            getVec8Data(a, 1) - getVec8Data(b, 1),
            getVec8Data(a, 2) - getVec8Data(b, 2),
            getVec8Data(a, 3) - getVec8Data(b, 3),
            getVec8Data(a, 4) - getVec8Data(b, 4),
            getVec8Data(a, 5) - getVec8Data(b, 5),
            getVec8Data(a, 6) - getVec8Data(b, 6),
            getVec8Data(a, 7) - getVec8Data(b, 7)
        };
    }
    return { _mm256_sub_ps(a, b) };
}

EUCLID_QUALIFIER Vec8 EUCLID_CALL operator*(Vec8 a, Vec8 b) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return {
            getVec8Data(a, 0) * getVec8Data(b, 0),
            getVec8Data(a, 1) * getVec8Data(b, 1),
            getVec8Data(a, 2) * getVec8Data(b, 2),
            getVec8Data(a, 3) * getVec8Data(b, 3),
            getVec8Data(a, 4) * getVec8Data(b, 4),
            getVec8Data(a, 5) * getVec8Data(b, 5),
            getVec8Data(a, 6) * getVec8Data(b, 6),
            getVec8Data(a, 7) * getVec8Data(b, 7)
        };
    }
    return { _mm256_mul_ps(a, b) };
}

EUCLID_QUALIFIER Vec8 EUCLID_CALL operator*(Vec8 a, float v) noexcept {
    return a * set1Vec8(v);
}

EUCLID_QUALIFIER Vec8 EUCLID_CALL operator*(float v, Vec8 a) noexcept {
    return a * v;
}

EUCLID_QUALIFIER Vec8 EUCLID_CALL operator/(Vec8 a, Vec8 b) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return {
            getVec8Data(a, 0) / getVec8Data(b, 0),
            getVec8Data(a, 1) / getVec8Data(b, 1),
            getVec8Data(a, 2) / getVec8Data(b, 2),
            getVec8Data(a, 3) / getVec8Data(b, 3),
            getVec8Data(a, 4) / getVec8Data(b, 4),
            getVec8Data(a, 5) / getVec8Data(b, 5),
            getVec8Data(a, 6) / getVec8Data(b, 6),
            getVec8Data(a, 7) / getVec8Data(b, 7)
        };
    }
    return { _mm256_div_ps(a, b) };
}

EUCLID_QUALIFIER Vec8 EUCLID_CALL operator/(Vec8 a, float v) noexcept {
    return a / set1Vec8(v);
}

EUCLID_QUALIFIER Vec8 EUCLID_CALL operator-(Vec8 a) noexcept {
    return setZeroVec8() - a;
}

EUCLID_QUALIFIER Vec8& EUCLID_CALL operator+=(Vec8& a, Vec8 b) noexcept {
    a = a + b;
    return a;
}

EUCLID_QUALIFIER Vec8& EUCLID_CALL operator-=(Vec8& a, Vec8 b) noexcept {
    a = a - b;
    return a;
}

EUCLID_QUALIFIER Vec8& EUCLID_CALL operator*=(Vec8& a, Vec8 b) noexcept {
    a = a * b;
    return a;
}

EUCLID_QUALIFIER Vec8& EUCLID_CALL operator*=(Vec8& a, float v) noexcept {
    a = a * v;
    return a;
}

EUCLID_QUALIFIER Vec8& EUCLID_CALL operator/=(Vec8& a, Vec8 b) noexcept {
    a = a / b;
    return a;
}

EUCLID_QUALIFIER Vec8& EUCLID_CALL operator/=(Vec8& a, float v) noexcept {
    a = a / v;
    return a;
}

EUCLID_QUALIFIER bool EUCLID_CALL operator==(Vec8 a, Vec8 b) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return getVec8Data(a, 0) == getVec8Data(b, 0) && 
               getVec8Data(a, 1) == getVec8Data(b, 1) &&
               getVec8Data(a, 2) == getVec8Data(b, 2) &&
               getVec8Data(a, 3) == getVec8Data(b, 3) &&
               getVec8Data(a, 4) == getVec8Data(b, 4) &&
               getVec8Data(a, 5) == getVec8Data(b, 5) &&
               getVec8Data(a, 6) == getVec8Data(b, 6) &&
               getVec8Data(a, 7) == getVec8Data(b, 7);
    }
    return _mm256_movemask_ps(_mm256_cmp_ps(a, b, 0)) == 0xff;
}

EUCLID_QUALIFIER bool EUCLID_CALL operator!=(Vec8 a, Vec8 b) noexcept {
    return !(a == b);
}

EUCLID_QUALIFIER bool EUCLID_CALL equals(Vec8 a, Vec8 b) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return detail::floating_point_equals(getVec8Data(a, 0), getVec8Data(b, 0)) &&
               detail::floating_point_equals(getVec8Data(a, 1), getVec8Data(b, 1)) &&
               detail::floating_point_equals(getVec8Data(a, 2), getVec8Data(b, 2)) &&
               detail::floating_point_equals(getVec8Data(a, 3), getVec8Data(b, 3)) &&
               detail::floating_point_equals(getVec8Data(a, 4), getVec8Data(b, 4)) &&
               detail::floating_point_equals(getVec8Data(a, 5), getVec8Data(b, 5)) &&
               detail::floating_point_equals(getVec8Data(a, 6), getVec8Data(b, 6)) &&
               detail::floating_point_equals(getVec8Data(a, 7), getVec8Data(b, 7));
    }
    return a == b;
}

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
