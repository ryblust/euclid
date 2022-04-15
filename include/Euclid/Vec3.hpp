#pragma once

#include "Core.h"

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

constexpr Vec3 EUCLID_CALL set1Vec3(const float val) noexcept {
    return { val,val,val };
}

constexpr Vec3 EUCLID_CALL setZeroVec3() noexcept {
    return { 0,0,0 };
}

constexpr Vec3 EUCLID_CALL operator+(const Vec3 a, const Vec3 b) noexcept {
    return { a.x + b.x, a.y + b.y, a.z + b.z };
}

constexpr Vec3 EUCLID_CALL operator-(const Vec3 a, const Vec3 b) noexcept {
    return { a.x - b.x, a.y - b.y, a.z - b.z };
}

constexpr Vec3 EUCLID_CALL operator*(const Vec3 a, const Vec3 b) noexcept {
    return { a.x * b.x, a.y * b.y, a.z * b.z };
}

constexpr Vec3 EUCLID_CALL operator*(const Vec3 a, const float scale) noexcept {
    return { a.x * scale, a.y * scale, a.z * scale };
}

constexpr Vec3 EUCLID_CALL operator*(const float scale, const Vec3 a) noexcept {
    return a * scale;
}

constexpr Vec3 EUCLID_CALL operator/(const Vec3 a, const Vec3 b) noexcept {
    return { a.x / b.x, a.y / b.y, a.z / b.z };
}

constexpr Vec3 EUCLID_CALL operator/(const Vec3 a, const float scale) noexcept {
    return { a.x / scale, a.y / scale, a.z / scale };
}

constexpr Vec3 EUCLID_CALL operator-(const Vec3 a) noexcept {
    return { -a.x, -a.y, -a.z };
}

constexpr Vec3& EUCLID_CALL operator+=(Vec3& a, const Vec3 b) noexcept {
    a = a + b;
    return a;
}

constexpr Vec3& EUCLID_CALL operator-=(Vec3& a, const Vec3 b) noexcept {
    a = a - b;
    return a;
}

constexpr Vec3& EUCLID_CALL operator*=(Vec3& a, const Vec3 b) noexcept {
    a = a * b;
    return a;
}

constexpr Vec3& EUCLID_CALL operator*=(Vec3& a, const float scale) noexcept {
    a = a * scale;
    return a;
}

constexpr Vec3& EUCLID_CALL operator/=(Vec3& a, const Vec3 b) noexcept {
    a = a / b;
    return a;
}

constexpr Vec3& EUCLID_CALL operator/=(Vec3& a, const float scale) noexcept {
    a = a / scale;
    return a;
}

constexpr bool EUCLID_CALL operator==(const Vec3 a, const Vec3 b) noexcept {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

constexpr bool EUCLID_CALL operator!=(const Vec3 a, const Vec3 b) noexcept {
    return !(a == b);
}

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif