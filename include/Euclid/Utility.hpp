#pragma once

#include "Vec4.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4514)
// enable /Wall
// C4514 : Ignore the compiler's warning about removing unused inline functions
#endif

namespace euclid::util {

/* Clamp Functions */

template<arithmetic T>
EUCLID_QUALIFIER T clamp(const T val, const T min, const T max) noexcept {
    return val < min ? min : max < val ? max : val;
}

EUCLID_QUALIFIER vec4 EUCLID_CALL clamp(const vec4 v, const vec4 min, const vec4 max) noexcept {
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

EUCLID_QUALIFIER vec4 EUCLID_CALL clamp(const vec4 v, const float min, const float max) noexcept {
    return clamp(v, set1Vec4(min), set1Vec4(max));
}

/* Lerp Functions */

template<floating_point T> 
EUCLID_QUALIFIER T lerp(const T a, const T b, const T t) noexcept {
    return a + t * (b - a);
}

void lerp() {}

/* Saturate Functions */

template<arithmetic T>
EUCLID_FORCEINLINE constexpr T saturate(const T val) noexcept {
    return val > 1 ? 1 : val < 0 ? 0 : val;
}

void saturate() {}

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif