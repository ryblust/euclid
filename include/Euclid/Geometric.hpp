#pragma once

#include "Vec3.hpp"
#include "Vec4.hpp"
#include "Math.hpp"

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(push)
#pragma warning(disable: 4514 5246)
#endif  // _MSC_VER && !__clang__

namespace euclid {

// constexpr float EUCLID_CALL dot(Vec3 a, Vec3 b) noexcept {

// }

// constexpr float EUCLID_CALL length(Vec3 a) noexcept {

// }

// constexpr Vec3 EUCLID_CALL normalize(Vec3 a) noexcept {

// }

// constexpr Vec3 EUCLID_CALL cross(Vec3 a, Vec3 b) noexcept {

// }

EUCLID_QUALIFIER Vec4 EUCLID_CALL clamp(Vec4 v, Vec4 min, Vec4 max) noexcept {
#ifndef __clang__
  if (std::is_constant_evaluated()) {
    return Vec4 {
      math::clamp(getVec4Data(v, 0), getVec4Data(min, 0), getVec4Data(max, 0)),
      math::clamp(getVec4Data(v, 1), getVec4Data(min, 1), getVec4Data(max, 1)),
      math::clamp(getVec4Data(v, 2), getVec4Data(min, 2), getVec4Data(max, 2)),
      math::clamp(getVec4Data(v, 3), getVec4Data(min, 3), getVec4Data(max, 3))
    };
  }
#endif // __clang__
  return { _mm_max_ps(_mm_min_ps(v, max), min) };
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL clamp(Vec4 v, float min, float max) noexcept {
  return clamp(v, set1Vec4(min), set1Vec4(max));
}

constexpr Vec4 EUCLID_CALL lerp(Vec4 a, Vec4 b, Vec4 t) noexcept {
#ifndef __clang__
  if (std::is_constant_evaluated()) {
    return Vec4 {
      math::lerp(getVec4Data(a, 0), getVec4Data(b, 0), getVec4Data(t, 0)),
      math::lerp(getVec4Data(a, 1), getVec4Data(b, 1), getVec4Data(t, 1)),
      math::lerp(getVec4Data(a, 2), getVec4Data(b, 2), getVec4Data(t, 2)),
      math::lerp(getVec4Data(a, 3), getVec4Data(b, 3), getVec4Data(t, 3))
    };
  }
#endif // __clang__
  return { _mm_fmadd_ps(t, _mm_sub_ps(b, a), a) };
}

constexpr Vec4 EUCLID_CALL lerp(Vec4 a, Vec4 b, float t) noexcept {
  return lerp(a, b, set1Vec4(t));
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL saturate(Vec4 a) noexcept {
#ifndef __clang__
  if (std::is_constant_evaluated()) {
    return Vec4 {
      math::saturate(getVec4Data(a, 0)),
      math::saturate(getVec4Data(a, 1)),
      math::saturate(getVec4Data(a, 2)),
      math::saturate(getVec4Data(a, 3))
    };
  }
#endif // __clang__
  return { _mm_max_ps(_mm_min_ps(a, _mm_set1_ps(1)), _mm_setzero_ps()) };
}

EUCLID_QUALIFIER float EUCLID_CALL dot(Vec4 a, Vec4 b) noexcept {
#ifndef __clang__
  if (std::is_constant_evaluated()) {
    return
      getVec4Data(a, 0) * getVec4Data(b, 0) +
      getVec4Data(a, 1) * getVec4Data(b, 1) +
      getVec4Data(a, 2) * getVec4Data(b, 2) +
      getVec4Data(a, 3) * getVec4Data(b, 3);
  }
#endif // __clang__
  return _mm_cvtss_f32(_mm_dp_ps(a, b, 0xff));
}

EUCLID_QUALIFIER float EUCLID_CALL length(Vec4 a) noexcept {
#ifndef __clang__
  if (std::is_constant_evaluated()) {
    return math::sqrt(dot(a, a));
  }
#endif // __clang__
  return _mm_cvtss_f32(_mm_sqrt_ps(_mm_dp_ps(a, a, 0xff)));
}

EUCLID_QUALIFIER float EUCLID_CALL lengthEst(Vec4 a) noexcept {
#ifndef __clang__
  if (std::is_constant_evaluated()) {
    return math::sqrt(dot(a, a));
  }
#endif // __clang__
  const __m128 dpvec = _mm_dp_ps(a, a, 0xff);
  const __m128 rsqrt = _mm_rsqrt_ps(dpvec);
  return _mm_cvtss_f32(_mm_mul_ps(rsqrt, dpvec));
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL normalize(Vec4 a) noexcept {
#ifndef __clang__
  if (std::is_constant_evaluated()) {
    return a / math::sqrt(dot(a, a));
  }
#endif // __clang__
  return { _mm_div_ps(a, _mm_sqrt_ps(_mm_dp_ps(a, a, 0xff))) };
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL normalizeEst(Vec4 a) noexcept {
#ifndef __clang__
  if (std::is_constant_evaluated()) {
    return a * math::rsqrt(dot(a, a));
  }
#endif // __clang__
  return { _mm_mul_ps(a, _mm_rsqrt_ps(_mm_dp_ps(a, a, 0xff))) };
}

EUCLID_QUALIFIER Vec4 EUCLID_CALL cross(Vec4 a, Vec4 b) noexcept {
#ifndef __clang__
  if (std::is_constant_evaluated()) {
    return Vec4 {
      getVec4Data(a, 1) * getVec4Data(b, 2) - getVec4Data(a, 2) * getVec4Data(b, 1),
      getVec4Data(a, 2) * getVec4Data(b, 0) - getVec4Data(a, 0) * getVec4Data(b, 2),
      getVec4Data(a, 0) * getVec4Data(b, 1) - getVec4Data(a, 1) * getVec4Data(b, 0),
      0
    };
  }
#endif // __clang__
  const __m128 v1 = _mm_permute_ps(a, _MM_SHUFFLE(3, 0, 2, 1));
  const __m128 v2 = _mm_permute_ps(b, _MM_SHUFFLE(3, 0, 2, 1));
  const __m128 v3 = _mm_mul_ps(v1, b);
  const __m128 v4 = _mm_fmsub_ps(v2, a, v3);
  return { _mm_permute_ps(v4, _MM_SHUFFLE(3, 0, 2, 1)) };
}

} // namespace euclid

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(pop)
#endif // _MSC_VER && !__clang__