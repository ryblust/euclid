#pragma once

#include "Core.h"
#include "Utility.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4514)
// enable /Wall
// C4514: remove unused inline functions
#endif

namespace euclid {

struct Vec3 {
  float x, y, z;
};

namespace detail {
    
inline __m128 EUCLID_CALL loadVec3(const Vec3& a) noexcept {
  const __m128 xy = _mm_castpd_ps(_mm_load_sd(reinterpret_cast<const double*>(&a)));
  const __m128 z  = _mm_load_ss(&a.z);
  return _mm_insert_ps(xy, z, 0x20);
}

inline void EUCLID_CALL storeVec3(Vec3& dst, const __m128 src) noexcept {
  *reinterpret_cast<int*>(&dst.x) = _mm_extract_ps(src, 0);
  *reinterpret_cast<int*>(&dst.y) = _mm_extract_ps(src, 1);
  *reinterpret_cast<int*>(&dst.z) = _mm_extract_ps(src, 2);
}

}

constexpr Vec3 EUCLID_CALL set1Vec3(float val) noexcept {
  return { val,val,val };
}

constexpr Vec3 EUCLID_CALL setZeroVec3() noexcept {
  return { 0,0,0 };
}

constexpr Vec3 EUCLID_CALL operator+(Vec3 a, Vec3 b) noexcept {
  return { a.x + b.x, a.y + b.y, a.z + b.z };
}

constexpr Vec3 EUCLID_CALL operator-(Vec3 a, Vec3 b) noexcept {
  return { a.x - b.x, a.y - b.y, a.z - b.z };
}

constexpr Vec3 EUCLID_CALL operator*(Vec3 a, Vec3 b) noexcept {
  return { a.x * b.x, a.y * b.y, a.z * b.z };
}

constexpr Vec3 EUCLID_CALL operator*(Vec3 a, float scale) noexcept {
  return { a.x * scale, a.y * scale, a.z * scale };
}

constexpr Vec3 EUCLID_CALL operator*(float scale, Vec3 a) noexcept {
  return a * scale;
}

constexpr Vec3 EUCLID_CALL operator/(Vec3 a, Vec3 b) noexcept {
  return { a.x / b.x, a.y / b.y, a.z / b.z };
}

constexpr Vec3 EUCLID_CALL operator/(Vec3 a, float scale) noexcept {
  return { a.x / scale, a.y / scale, a.z / scale };
}

constexpr Vec3 EUCLID_CALL operator-(Vec3 a) noexcept {
  return { -a.x, -a.y, -a.z };
}

constexpr Vec3& EUCLID_CALL operator+=(Vec3& a, Vec3 b) noexcept {
  a = a + b;
  return a;
}

constexpr Vec3& EUCLID_CALL operator-=(Vec3& a, Vec3 b) noexcept {
  a = a - b;
  return a;
}

constexpr Vec3& EUCLID_CALL operator*=(Vec3& a, Vec3 b) noexcept {
  a = a * b;
  return a;
}

constexpr Vec3& EUCLID_CALL operator*=(Vec3& a, float scale) noexcept {
  a = a * scale;
  return a;
}

constexpr Vec3& EUCLID_CALL operator/=(Vec3& a, Vec3 b) noexcept {
  a = a / b;
  return a;
}

constexpr Vec3& EUCLID_CALL operator/=(Vec3& a, float scale) noexcept {
  a = a / scale;
  return a;
}

constexpr bool EUCLID_CALL operator==(Vec3 a, Vec3 b) noexcept {
  return a.x == b.x && a.y == b.y && a.z == b.z;
}

constexpr bool EUCLID_CALL operator!=(Vec3 a, Vec3 b) noexcept {
  return !(a == b);
}

constexpr bool EUCLID_CALL equals(Vec3 a, Vec3 b) noexcept {
  return
    detail::floating_point_equals(a.x, b.x) &&
    detail::floating_point_equals(a.y, b.y) &&
    detail::floating_point_equals(a.z, b.z);
}

constexpr Vec3 EUCLID_CALL clamp(Vec3 v, Vec3 min, Vec3 max) noexcept {
  return {
    detail::clamp(v.x, min.x, max.x),
    detail::clamp(v.y, min.y, max.y),
    detail::clamp(v.z, min.z, max.z)
  };
}

constexpr Vec3 EUCLID_CALL clamp(Vec3 v, float min, float max) noexcept {
  return clamp(v, set1Vec3(min), set1Vec3(max));
}

constexpr Vec3 EUCLID_CALL lerp(Vec3 a, Vec3 b, Vec3 t) noexcept {
  return { 
    detail::lerp(a.x, b.x, t.x),
    detail::lerp(a.y, b.y, t.y),
    detail::lerp(a.z, b.z, t.z)
  };
}

constexpr Vec3 EUCLID_CALL lerp(Vec3 a, Vec3 b, float t) noexcept {
  return lerp(a, b, set1Vec3(t));
}

constexpr Vec3 EUCLID_CALL saturate(Vec3 a) noexcept {
  return {
    detail::saturate(a.x),
    detail::saturate(a.y),
    detail::saturate(a.z)
  };
}

} // namespace euclid

#ifdef _MSC_VER
#pragma warning(pop)
#endif