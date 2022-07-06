#pragma once

#include "Vec2.hpp"
#include "Vec3.hpp"
#include "Vec4.hpp"
#include "Trigonometric.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4514 5246)
// enable /Wall
// C4514: remove unused inline functions
// C5246: the initialization of a subobject should be wrapped in braces
#endif

namespace euclid {

constexpr float EUCLID_CALL dot(Vec2 a, Vec2 b) noexcept {
    return a.x * b.x + a.y * b.y;
}

constexpr float EUCLID_CALL dot(Vec3 a, Vec3 b) noexcept {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

constexpr float EUCLID_CALL length(Vec2 a) noexcept {
    return math::sqrt(dot(a, a));
}

constexpr float EUCLID_CALL length(Vec3 a) noexcept {
    return math::sqrt(dot(a, a));
}

constexpr Vec2 EUCLID_CALL normalize(Vec2 a) noexcept {
    return a / length(a);
}

constexpr Vec3 EUCLID_CALL normalize(Vec3 a) noexcept {
    return a / length(a);
}

constexpr Vec3 EUCLID_CALL cross(Vec3 a, Vec3 b) noexcept {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

EUCLID_QUALIFIER float EUCLID_CALL dot(Vec4 a, Vec4 b) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return getVec4Data(a, 0) * getVec4Data(b, 0) +
               getVec4Data(a, 1) * getVec4Data(b, 1) +
               getVec4Data(a, 2) * getVec4Data(b, 2) +
               getVec4Data(a, 3) * getVec4Data(b, 3);
    }
    return _mm_cvtss_f32(_mm_dp_ps(a, b, 0xff));
}

EUCLID_QUALIFIER float EUCLID_CALL length(Vec4 a) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return math::sqrt(dot(a, a));
    }
    return _mm_cvtss_f32(_mm_sqrt_ps(_mm_dp_ps(a, a, 0xff)));
}

EUCLID_QUALIFIER float EUCLID_CALL lengthFast(Vec4 a) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return math::sqrt(dot(a, a));
    }
    const __m128 dpvec = _mm_dp_ps(a, a, 0xff);
    const __m128 rsqrt = _mm_rsqrt_ps(dpvec);
    return _mm_cvtss_f32(_mm_mul_ps(rsqrt, dpvec));
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL normalize(Vec4 a) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return a / length(a);
    }
    return { _mm_div_ps(a, _mm_sqrt_ps(_mm_dp_ps(a, a, 0xff))) };
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL normalizeFast(Vec4 a) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return a / math::sqrt(dot(a, a));
    }
    return { _mm_mul_ps(a, _mm_rsqrt_ps(_mm_dp_ps(a, a, 0xff))) };
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL cross(Vec4 a, Vec4 b) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return {
            getVec4Data(a, 1) * getVec4Data(b, 2) - getVec4Data(a, 2) * getVec4Data(b, 1),
            getVec4Data(a, 2) * getVec4Data(b, 0) - getVec4Data(a, 0) * getVec4Data(b, 2),
            getVec4Data(a, 0) * getVec4Data(b, 1) - getVec4Data(a, 1) * getVec4Data(b, 0),
            0
        };
    }
    const __m128 v1 = _mm_permute_ps(a, _MM_SHUFFLE(3, 0, 2, 1));
    const __m128 v2 = _mm_permute_ps(b, _MM_SHUFFLE(3, 0, 2, 1));
    const __m128 v3 = _mm_mul_ps(v1, b);
    const __m128 v4 = _mm_fmsub_ps(v2, a, v3);
    return { _mm_permute_ps(v4, _MM_SHUFFLE(3, 0, 2, 1)) };
}

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
