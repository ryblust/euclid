#pragma once

#include "Core.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4514)
// enable /Wall
// C4514 : Ignore the compiler's warning about removing unused inline functions
#endif

namespace euclid {

struct Vec3 {
    using value_type = float;

    constexpr Vec3& EUCLID_CALL operator+=(const Vec3 v) noexcept {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    constexpr Vec3& EUCLID_CALL operator-=(const Vec3 v) noexcept {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    constexpr Vec3& EUCLID_CALL operator*=(const Vec3 v) noexcept {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }

    constexpr Vec3& EUCLID_CALL operator*=(const float scale) noexcept {
        x *= scale;
        y *= scale;
        z *= scale;
        return *this;
    }

    constexpr Vec3& EUCLID_CALL operator/=(const Vec3 v) noexcept {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }

    constexpr Vec3& EUCLID_CALL operator/=(const float scale) noexcept {
        x /= scale;
        y /= scale;
        z /= scale;
        return *this;
    }

    constexpr Vec3 EUCLID_CALL operator+(const Vec3 v) const noexcept {
        return { x + v.x, y + v.y, z + v.z };
    }

    constexpr Vec3 EUCLID_CALL operator-(const Vec3 v) const noexcept {
        return { x - v.x, y - v.y, z - v.z };
    }

    constexpr Vec3 EUCLID_CALL operator*(const Vec3 v) const noexcept {
        return { x * v.x, y * v.y, z * v.z };
    }

    constexpr Vec3 EUCLID_CALL operator*(const float scale) const noexcept {
        return { x * scale, y * scale, z * scale };
    }

    constexpr Vec3 EUCLID_CALL operator/(const Vec3 v) const noexcept {
        return { x / v.x, y / v.y, z / v.z };
    }

    constexpr Vec3 EUCLID_CALL operator/(const float scale) const noexcept {
        return { x / scale, y / scale, z / scale };
    }

    constexpr Vec3 EUCLID_CALL operator-() const noexcept {
        return { -x, -y, -z };
    }

    constexpr bool EUCLID_CALL operator==(const Vec3 v) const noexcept {
        return x == v.x && y == v.y && z == v.z;
    }

    constexpr bool EUCLID_CALL operator!=(const Vec3 v) const noexcept {
        return !(this->operator==(v));
    }

    float x, y, z;
};
  
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif