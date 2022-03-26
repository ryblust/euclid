#pragma once

#include "Math.hpp"
#include "Vec2.hpp"
#include "Vec3.hpp"
#include "Vec4.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4514)
// enable /Wall
// C4514 : Ignore the compiler's warning about removing unused inline functions
#endif

namespace euclid {

constexpr float EUCLID_CALL dot(const Vec2 a, const Vec2 b) noexcept {
    return a.x * b.x + a.y * b.y;
}

constexpr float EUCLID_CALL dot(const Vec3 a, const Vec3 b) noexcept {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

constexpr float EUCLID_CALL length(const Vec2 a) noexcept {
    return math::sqrt(dot(a, a));
}

constexpr float EUCLID_CALL length(const Vec3 a) noexcept {
    return math::sqrt(dot(a, a));
}

constexpr Vec2 EUCLID_CALL normalize(const Vec2 a) noexcept {
    return a / length(a);
}

constexpr Vec3 EUCLID_CALL normalize(const Vec3 a) noexcept {
    return a / length(a);
}

constexpr Vec3 EUCLID_CALL cross(const Vec3 a, const Vec3 b) noexcept {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

EUCLID_QUALIFIER float EUCLID_CALL dot(const Vec4 a, const Vec4 b) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return getVec4Data(a, 0) * getVec4Data(b, 0) +
               getVec4Data(a, 1) * getVec4Data(b, 1) +
               getVec4Data(a, 2) * getVec4Data(b, 2) +
               getVec4Data(a, 3) * getVec4Data(b, 3);
    }
    return _mm_cvtss_f32(_mm_dp_ps(a, b, 0xff));
}

EUCLID_QUALIFIER float EUCLID_CALL length(const Vec4 a) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return math::sqrt(dot(a, a));
    }
    return _mm_cvtss_f32(_mm_sqrt_ps(_mm_dp_ps(a, a, 0xff)));
}

EUCLID_QUALIFIER float EUCLID_CALL lengthEst(const Vec4 a) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return math::sqrt(dot(a, a));
    }
    const Vec4 dpvec = _mm_dp_ps(a, a, 0xff);
    const Vec4 rsqrt = _mm_rsqrt_ps(dpvec);
    return _mm_cvtss_f32(_mm_mul_ps(rsqrt, dpvec));
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL normalize(const Vec4 a) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return a / length(a);
    }
    return _mm_div_ps(a, _mm_sqrt_ps(_mm_dp_ps(a, a, 0xff)));
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL normalizeEst(const Vec4 a) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return a / math::sqrt(dot(a, a));
    }
    return _mm_mul_ps(a, _mm_rsqrt_ps(_mm_dp_ps(a, a, 0xff)));
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL cross(const Vec4 a, const Vec4 b) noexcept {
    if (__builtin_is_constant_evaluated()) {
        return {
            getVec4Data(a, 1) * getVec4Data(b, 2) - getVec4Data(a, 2) * getVec4Data(b, 1),
            getVec4Data(a, 2) * getVec4Data(b, 0) - getVec4Data(a, 0) * getVec4Data(b, 2),
            getVec4Data(a, 0) * getVec4Data(b, 1) - getVec4Data(a, 1) * getVec4Data(b, 0),
            0
        };
    }
    const Vec4 v1 = _mm_permute_ps(a, _MM_SHUFFLE(3, 0, 2, 1));
    const Vec4 v2 = _mm_permute_ps(b, _MM_SHUFFLE(3, 0, 2, 1));
    const Vec4 v3 = _mm_mul_ps(v1, b);
    const Vec4 v4 = _mm_fmsub_ps(v2, a, v3);
    return _mm_permute_ps(v4, _MM_SHUFFLE(3, 0, 2, 1));
}

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
