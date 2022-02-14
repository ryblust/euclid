#pragma once

#include "Core.h"

namespace euclid {

struct vec3 {
    using value_type = float;

    constexpr vec3& operator+=(const vec3 v) noexcept {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    constexpr vec3& operator-=(const vec3 v) noexcept {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    constexpr vec3& operator*=(const vec3 v) noexcept {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }

    constexpr vec3& operator*=(const float scale) noexcept {
        x *= scale;
        y *= scale;
        z *= scale;
        return *this;
    }

    constexpr vec3& operator/=(const vec3 v) noexcept {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }

    constexpr vec3& operator/=(const float scale) noexcept {
        x /= scale;
        y /= scale;
        z /= scale;
        return *this;
    }

    constexpr vec3 operator+(const vec3 v) const noexcept {
        return { x + v.x, y + v.y, z + v.z };
    }

    constexpr vec3 operator-(const vec3 v) const noexcept {
        return { x - v.x, y - v.y, z - v.z };
    }

    constexpr vec3 operator*(const vec3 v) const noexcept {
        return { x * v.x, y * v.y, z * v.z };
    }

    constexpr vec3 operator*(const float scale) const noexcept {
        return { x * scale, y * scale, z * scale };
    }

    constexpr vec3 operator/(const vec3 v) const noexcept {
        return { x / v.x, y / v.y, z / v.z };
    }

    constexpr vec3 operator/(const float scale) const noexcept {
        return { x / scale, y / scale, z / scale };
    }

    float x, y, z;
};
  
}
