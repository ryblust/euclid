#pragma once

#include "Core.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4514)
// enable /Wall
// C4514 : Ignore the compiler's warning about removing unused inline functions
#endif

namespace euclid {

EUCLID_QUALIFIER float getVec4Data(const Vec4 a, const std::size_t pos) noexcept {
#ifdef _MSC_VER
    return a.m128_f32[pos];
#else
    return a.v[pos];
#endif
}

EUCLID_QUALIFIER float& getVec4RefData(Vec4& a, const std::size_t pos) noexcept {
#ifdef _MSC_VER
    return a.m128_f32[pos];
#elif __clang__
    return *(reinterpret_cast<float*>(&a) + pos);
#else
    return a.v[pos];
#endif
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL set1Vec4(const float v) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return { v,v,v,v };
    }
    return _mm_set1_ps(v);
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL setZeroVec4() noexcept {
    if (__builtin_is_constant_evaluated()) {
        return { 0,0,0,0 };
    }
    return _mm_setzero_ps();
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL operator+(const Vec4 a, const Vec4 b) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return {
            getVec4Data(a, 0) + getVec4Data(b, 0),
            getVec4Data(a, 1) + getVec4Data(b, 1),
            getVec4Data(a, 2) + getVec4Data(b, 2),
            getVec4Data(a, 3) + getVec4Data(b, 3)
        };
    }
    return _mm_add_ps(a, b);
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL operator-(const Vec4 a, const Vec4 b) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return { 
            getVec4Data(a, 0) - getVec4Data(b, 0),
            getVec4Data(a, 1) - getVec4Data(b, 1),
            getVec4Data(a, 2) - getVec4Data(b, 2),
            getVec4Data(a, 3) - getVec4Data(b, 3)
        };
    }
    return _mm_sub_ps(a, b);
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL operator*(const Vec4 a, const Vec4 b) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return { 
            getVec4Data(a, 0) * getVec4Data(b, 0),
            getVec4Data(a, 1) * getVec4Data(b, 1),
            getVec4Data(a, 2) * getVec4Data(b, 2),
            getVec4Data(a, 3) * getVec4Data(b, 3)
        };
    }
    return _mm_mul_ps(a, b);
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL operator*(const Vec4 a, const float v) noexcept {
    return a * set1Vec4(v);
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL operator*(const float v, const Vec4 a) noexcept {
    return a * v;
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL operator/(const Vec4 a, const Vec4 b) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return { 
            getVec4Data(a, 0) / getVec4Data(b, 0),
            getVec4Data(a, 1) / getVec4Data(b, 1),
            getVec4Data(a, 2) / getVec4Data(b, 2),
            getVec4Data(a, 3) / getVec4Data(b, 3)
        };
    }
    return _mm_div_ps(a, b);
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL operator/(const Vec4 a, const float v) noexcept {
    return a / set1Vec4(v);
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL operator-(const Vec4 a) noexcept {
    return setZeroVec4() - a;
}

EUCLID_QUALIFIER Vec4& EUCLID_CALL operator+=(Vec4& a, const Vec4 b) noexcept {
    a = a + b;
    return a;
}

EUCLID_QUALIFIER Vec4& EUCLID_CALL operator-=(Vec4& a, const Vec4 b) noexcept {
    a = a - b;
    return a;
}

EUCLID_QUALIFIER Vec4& EUCLID_CALL operator*=(Vec4& a, const Vec4 b) noexcept {
    a = a * b;
    return a;
}

EUCLID_QUALIFIER Vec4& EUCLID_CALL operator*=(Vec4& a, const float v) noexcept {
    a = a * v;
    return a;
}

EUCLID_QUALIFIER Vec4& EUCLID_CALL operator/=(Vec4& a, const Vec4 b) noexcept {
    a = a / b;
    return a;
}

EUCLID_QUALIFIER Vec4& EUCLID_CALL operator/=(Vec4& a, const float v) noexcept {
    a = a / v;
    return a;
}

EUCLID_QUALIFIER bool EUCLID_CALL operator==(const Vec4 a, const Vec4 b) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return getVec4Data(a, 0) == getVec4Data(b, 0) &&
               getVec4Data(a, 1) == getVec4Data(b, 1) &&
               getVec4Data(a, 2) == getVec4Data(b, 2) &&
               getVec4Data(a, 3) == getVec4Data(b, 3);
    }
    return _mm_movemask_ps(_mm_cmpeq_ps(a, b)) == 0xf;
}

EUCLID_QUALIFIER bool EUCLID_CALL operator!=(const Vec4 a, const Vec4 b) noexcept {
    return !(a == b);
}

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif