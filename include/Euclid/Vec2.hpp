#pragma once

#include "Core.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4514)
// enable /Wall
// C4514 : Ignore the compiler's warning about removing unused inline functions
#endif

namespace euclid {

struct Vec2 {
    using value_type = float;

    constexpr Vec2& EUCLID_CALL operator+=(const Vec2 v) noexcept {
        x += v.x;
        y += v.y;
        return *this;
    }

    constexpr Vec2& EUCLID_CALL operator-=(const Vec2 v) noexcept {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    constexpr Vec2& EUCLID_CALL operator*=(const Vec2 v) noexcept {
        x *= v.x;
        y *= v.y;
        return *this;
    }

    constexpr Vec2& EUCLID_CALL operator*=(const float scale) noexcept {
        x *= scale;
        y *= scale;
        return *this;
    }

    constexpr Vec2& EUCLID_CALL operator/=(const Vec2 v) noexcept {
        x /= v.x;
        y /= v.y;
        return *this;
    }

    constexpr Vec2& EUCLID_CALL operator/=(const float scale) noexcept {
        x /= scale;
        y /= scale;
        return *this;
    }

    constexpr Vec2 EUCLID_CALL operator+(const Vec2 v) const noexcept {
        return { x + v.x, y + v.y };
    }

    constexpr Vec2 EUCLID_CALL operator-(const Vec2 v) const noexcept {
        return { x - v.x, y - v.y };
    }

    constexpr Vec2 EUCLID_CALL operator*(const Vec2 v) const noexcept {
        return { x * v.x, y * v.y };
    }

    constexpr Vec2 EUCLID_CALL operator*(const float scale) const noexcept {
        return { x * scale, y * scale };
    }

    constexpr Vec2 EUCLID_CALL operator/(const Vec2 v) const noexcept {
        return { x / v.x, y / v.y };
    }

    constexpr Vec2 EUCLID_CALL operator/(const float scale) const noexcept {
        return { x / scale, y / scale };
    }

    constexpr Vec2 EUCLID_CALL operator-() const noexcept {
        return { -x, -y };
    }

    constexpr bool EUCLID_CALL operator==(const Vec2 v) const noexcept {
        return x == v.x && y == v.y;
    }

    constexpr bool EUCLID_CALL operator!=(const Vec2 v) const noexcept {
        return !(this->operator==(v));
    }

    float x, y;
};

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif