#pragma once

#include "Core.h"
#include "Math.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4514)
// enable /Wall
// C4514 : Ignore the compiler's warning about removing unused inline functions
#endif

#define EUCLID_SHUFFLE_MASK(x, y, z, w) (((w) << 6) | ((z) << 4) | ((y) << 2) | ((x)))

namespace euclid {

constexpr float& getVec4RefData(vec4& vec, const std::size_t pos) noexcept {
#ifdef _MSC_VER
    return vec.m128_f32[pos];
#else
    return vec.v[pos];
#endif
}

constexpr float getVec4Data(const vec4 vec, const std::size_t pos) noexcept {
#ifdef _MSC_VER
    return vec.m128_f32[pos];
#else
    return vec.v[pos];
#endif
}

EUCLID_QUALIFIER vec4 EUCLID_CALL set1Vec4(const float v) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return { v,v,v,v };
    }
    return _mm_set1_ps(v);
}

EUCLID_QUALIFIER vec4 EUCLID_CALL setZeroVec4() noexcept {
    if (__builtin_is_constant_evaluated()) {
        return { 0,0,0,0 };
    }
    return _mm_setzero_ps();
}

EUCLID_QUALIFIER vec4 EUCLID_CALL operator+(const vec4 a, const vec4 b) noexcept {
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

EUCLID_QUALIFIER vec4 EUCLID_CALL operator-(const vec4 a, const vec4 b) noexcept {
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

EUCLID_QUALIFIER vec4 EUCLID_CALL operator*(const vec4 a, const vec4 b) noexcept {
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

EUCLID_QUALIFIER vec4 EUCLID_CALL operator*(const vec4 a, const float v) noexcept {
    return a * set1Vec4(v);
}

EUCLID_QUALIFIER vec4 EUCLID_CALL operator*(const float v, const vec4 a) noexcept {
    return a * v;
}

EUCLID_QUALIFIER vec4 EUCLID_CALL operator/(const vec4 a, const vec4 b) noexcept {
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

EUCLID_QUALIFIER vec4 EUCLID_CALL operator/(const vec4 a, const float v) noexcept {
    return a / set1Vec4(v);
}

EUCLID_QUALIFIER vec4& EUCLID_CALL operator+=(vec4& a, const vec4 b) noexcept {
    a = a + b;
    return a;
}

EUCLID_QUALIFIER vec4& EUCLID_CALL operator-=(vec4& a, const vec4 b) noexcept {
    a = a - b;
    return a;
}

EUCLID_QUALIFIER vec4& EUCLID_CALL operator*=(vec4& a, const vec4 b) noexcept {
    a = a * b;
    return a;
}

EUCLID_QUALIFIER vec4& EUCLID_CALL operator*=(vec4& a, const float v) noexcept {
    a = a * v;
    return a;
}

EUCLID_QUALIFIER vec4& EUCLID_CALL operator/=(vec4& a, const vec4 b) noexcept {
    a = a / b;
    return a;
}

EUCLID_QUALIFIER vec4& EUCLID_CALL operator/=(vec4& a, const float v) noexcept {
    a = a / v;
    return a;
}

EUCLID_QUALIFIER vec4 EUCLID_CALL operator-(const vec4 a) noexcept {
    return setZeroVec4() - a;
}

EUCLID_QUALIFIER bool EUCLID_CALL operator==(const vec4 a, const vec4 b) noexcept {
    return getVec4Data(a, 0) == getVec4Data(b, 0) &&
           getVec4Data(a, 1) == getVec4Data(b, 1) &&
           getVec4Data(a, 2) == getVec4Data(b, 2) &&
           getVec4Data(a, 3) == getVec4Data(b, 3);
}

EUCLID_QUALIFIER bool EUCLID_CALL operator!=(const vec4 a, const vec4 b) noexcept {
    return !(a == b);
}

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif