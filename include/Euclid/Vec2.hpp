#pragma once

#include "Core.h"

namespace euclid {

struct vec2 {
    using value_type = float;

    constexpr vec2& operator+=(const vec2 v) noexcept {
        x += v.x;
        y += v.y;
        return *this;
    }

    constexpr vec2& operator-=(const vec2 v) noexcept {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    constexpr vec2& operator*=(const vec2 v) noexcept {
        x *= v.x;
        y *= v.y;
        return *this;
    }

    constexpr vec2& operator*=(const float scale) noexcept {
        x *= scale;
        y *= scale;
        return *this;
    }

    constexpr vec2& operator/=(const vec2 v) noexcept {
        x /= v.x;
        y /= v.y;
        return *this;
    }

    constexpr vec2& operator/=(const float scale) noexcept {
        x /= scale;
        y /= scale;
        return *this;
    }

    constexpr vec2 operator+(const vec2 v) const noexcept {
        return { x + v.x, y + v.y };
    }

    constexpr vec2 operator-(const vec2 v) const noexcept {
        return { x - v.x, y - v.y };
    }

    constexpr vec2 operator*(const vec2 v) const noexcept {
        return { x * v.x, y * v.y };
    }

    constexpr vec2 operator*(const float scale) const noexcept {
        return { x * scale, y * scale };
    }

    constexpr vec2 operator/(const vec2 v) const noexcept {
        return { x / v.x, y / v.y };
    }

    constexpr vec2 operator/(const float scale) const noexcept {
        return { x / scale, y / scale };
    }

    float x, y;
};

}