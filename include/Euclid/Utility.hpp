#pragma once

#include "Vec4.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4514)
// enable /Wall
// C4514 : Ignore the compiler's warning about removing unused inline functions
#endif

namespace euclid::util {

template<arithmetic T>
EUCLID_QUALIFIER T clamp(const T val, const T min, const T max) noexcept {
    return val < min ? min : max < val ? max : val;
}

template<floating_point T> 
EUCLID_QUALIFIER T lerp(const T a, const T b, const T t) noexcept {
    return a + t * (b - a);
}

template<arithmetic T>
EUCLID_QUALIFIER T saturate(const T val) noexcept {
    return val > 1 ? 1 : val < 0 ? 0 : val;
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL clamp(const Vec4 v, const Vec4 min, const Vec4 max) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return {
            clamp(getVec4Data(v, 0), getVec4Data(min, 0), getVec4Data(max, 0)),
            clamp(getVec4Data(v, 1), getVec4Data(min, 1), getVec4Data(max, 1)),
            clamp(getVec4Data(v, 2), getVec4Data(min, 2), getVec4Data(max, 2)),
            clamp(getVec4Data(v, 3), getVec4Data(min, 3), getVec4Data(max, 3))
        };
    }
    return _mm_max_ps(_mm_min_ps(v, max), min);
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL clamp(const Vec4 v, const float min, const float max) noexcept {
    return clamp(v, set1Vec4(min), set1Vec4(max));
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL lerp(const Vec4 a, const Vec4 b, const Vec4 t) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return {
            lerp(getVec4Data(a, 0), getVec4Data(b, 0), getVec4Data(t, 0)),
            lerp(getVec4Data(a, 1), getVec4Data(b, 1), getVec4Data(t, 1)),
            lerp(getVec4Data(a, 2), getVec4Data(b, 2), getVec4Data(t, 2)),
            lerp(getVec4Data(a, 3), getVec4Data(b, 3), getVec4Data(t, 3))
        };
    }
    return _mm_fmadd_ps(t, _mm_sub_ps(b, a), a);
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL lerp(const Vec4 a, const Vec4 b, const float t) noexcept {
    return lerp(a, b, set1Vec4(t));
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL saturate(const Vec4 a) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return {
            saturate(getVec4Data(a, 0)),
            saturate(getVec4Data(a, 1)),
            saturate(getVec4Data(a, 2)),
            saturate(getVec4Data(a, 3))
        };
    }
    return _mm_max_ps(_mm_min_ps(a, _mm_set1_ps(1)), _mm_setzero_ps());
}

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif