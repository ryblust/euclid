#pragma once

#include "Vec4.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4514 5246)
// enable /Wall
// C4514: removing unused inline functions
// C5246: the initialization of a subobject should be wrapped in braces
#endif

namespace euclid {

EUCLID_QUALIFIER float getVec8Data(const Vec8 a, const std::size_t pos) noexcept {
#if defined (_MSC_VER) && !defined(__clang__)
    return a.m256_f32[pos];
#else
    return a.v[pos];
#endif
}

EUCLID_QUALIFIER float& getVec8RefData(Vec8& a, const std::size_t pos) noexcept {
#if defined(_MSC_VER) && !defined(__clang__)
    return a.m256_f32[pos];
#elif __clang__
    return *(reinterpret_cast<float*>(&a) + pos);
#else
    return a.v[pos];
#endif
}

EUCLID_QUALIFIER Vec8 EUCLID_CALL set1Vec8(const float v) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return { v,v,v,v,v,v,v,v };
    }
    return _mm256_set1_ps(v);
}

EUCLID_QUALIFIER Vec8 EUCLID_CALL setZeroVec8() noexcept {
    if (__builtin_is_constant_evaluated()) {
        return { 0,0,0,0,0,0,0,0 };
    }
    return _mm256_setzero_ps();
}

EUCLID_QUALIFIER Vec8 EUCLID_CALL combine2Vec4(const Vec4 a, const Vec4 b) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return {
            getVec4Data(a, 0), getVec4Data(a, 1),
            getVec4Data(a, 2), getVec4Data(a, 3),
            getVec4Data(b, 0), getVec4Data(b, 1),
            getVec4Data(b, 2), getVec4Data(b, 3)
        };
    }
    return _mm256_set_m128(a, b);
}

EUCLID_QUALIFIER Vec8 EUCLID_CALL operator+(const Vec8 a, const Vec8 b) noexcept {
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
    return _mm256_add_ps(a, b);
}

EUCLID_QUALIFIER Vec8 EUCLID_CALL operator-(const Vec8 a, const Vec8 b) noexcept {
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
    return _mm256_sub_ps(a, b);
}

EUCLID_QUALIFIER Vec8 EUCLID_CALL operator*(const Vec8 a, const Vec8 b) noexcept {
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
    return _mm256_mul_ps(a, b);
}

EUCLID_QUALIFIER Vec8 EUCLID_CALL operator*(const Vec8 a, const float v) noexcept {
    return a * set1Vec8(v);
}

EUCLID_QUALIFIER Vec8 EUCLID_CALL operator*(const float v, const Vec8 a) noexcept {
    return a * v;
}

EUCLID_QUALIFIER Vec8 EUCLID_CALL operator/(const Vec8 a, const Vec8 b) noexcept {
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
    return _mm256_div_ps(a, b);
}

EUCLID_QUALIFIER Vec8 EUCLID_CALL operator/(const Vec8 a, const float v) noexcept {
    return a / set1Vec8(v);
}

EUCLID_QUALIFIER Vec8 EUCLID_CALL operator-(const Vec8 a) noexcept {
    return setZeroVec8() - a;
}

EUCLID_QUALIFIER Vec8& EUCLID_CALL operator+=(Vec8& a, const Vec8 b) noexcept {
    a = a + b;
    return a;
}

EUCLID_QUALIFIER Vec8& EUCLID_CALL operator-=(Vec8& a, const Vec8 b) noexcept {
    a = a - b;
    return a;
}

EUCLID_QUALIFIER Vec8& EUCLID_CALL operator*=(Vec8& a, const Vec8 b) noexcept {
    a = a * b;
    return a;
}

EUCLID_QUALIFIER Vec8& EUCLID_CALL operator*=(Vec8& a, const float v) noexcept {
    a = a * v;
    return a;
}

EUCLID_QUALIFIER Vec8& EUCLID_CALL operator/=(Vec8& a, const Vec8 b) noexcept {
    a = a / b;
    return a;
}

EUCLID_QUALIFIER Vec8& EUCLID_CALL operator/=(Vec8& a, const float v) noexcept {
    a = a / v;
    return a;
}

EUCLID_QUALIFIER bool EUCLID_CALL operator==(const Vec8 a, const Vec8 b) noexcept {
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

EUCLID_QUALIFIER bool EUCLID_CALL operator!=(const Vec8 a, const Vec8 b) noexcept {
    return !(a == b);
}

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif