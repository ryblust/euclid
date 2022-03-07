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
    float x, y;
};

constexpr Vec2 EUCLID_CALL operator+(const Vec2 a, const Vec2 b) noexcept {
    return { a.x + b.x, a.y + b.y };
}

constexpr Vec2 EUCLID_CALL operator-(const Vec2 a, const Vec2 b) noexcept {
    return { a.x - b.x, a.y - b.y };
}

constexpr Vec2 EUCLID_CALL operator*(const Vec2 a, const Vec2 b) noexcept {
    return { a.x * b.x, a.y * b.y };
}

constexpr Vec2 EUCLID_CALL operator*(const Vec2 a, const float scale) noexcept {
    return { a.x * scale, a.y * scale };
}

constexpr Vec2 EUCLID_CALL operator*(const float scale, const Vec2 a) noexcept {
    return a * scale;
}

constexpr Vec2 EUCLID_CALL operator/(const Vec2 a, const Vec2 b) noexcept {
    return { a.x / b.x, a.y / b.y };
}

constexpr Vec2 EUCLID_CALL operator/(const Vec2 a, const float scale) noexcept {
    return { a.x / scale, a.y / scale };
}

constexpr Vec2 EUCLID_CALL operator-(const Vec2 a) noexcept {
    return { -a.x, -a.y };
}

constexpr Vec2& EUCLID_CALL operator+=(Vec2& a, const Vec2 b) noexcept {
    a = a + b;
    return a;
}

constexpr Vec2& EUCLID_CALL operator-=(Vec2& a, const Vec2 b) noexcept {
    a = a - b;
    return a;
}

constexpr Vec2& EUCLID_CALL operator*=(Vec2& a, const Vec2 b) noexcept {
    a = a * b;
    return a;
}

constexpr Vec2& EUCLID_CALL operator*=(Vec2& a, const float scale) noexcept {
    a = a * scale;
    return a;
}

constexpr Vec2& EUCLID_CALL operator/=(Vec2& a, const Vec2 b) noexcept {
    a = a / b;
    return a;
}

constexpr Vec2& EUCLID_CALL operator/=(Vec2& a, const float scale) noexcept {
    a = a / scale;
    return a;
}

constexpr bool EUCLID_CALL operator==(const Vec2 a, const Vec2 b) noexcept {
    return a.x == b.x && a.y == b.y;
}

constexpr bool EUCLID_CALL operator!=(const Vec2 a, const Vec2 b) noexcept {
    return !(a == b);
}

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif