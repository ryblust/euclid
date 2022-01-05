#pragma once

namespace euclid {

template<arithmetic Type, std::size_t Size>
struct alignas(32) Point {
public:
    using value_type = Type;

    constexpr std::size_t size() const noexcept {
        return Size;
    }

    constexpr Point<float, Size> castTofloat() const noexcept requires (same_type<Type, int>) {
        return { point.castTofloat() };
    }

    constexpr void negative() noexcept {
        point.negative();
    }

    constexpr Point operator-() const noexcept {
        return { -point };
    }

    constexpr Vector<Type, Size> operator-(const Point otherPoint) const noexcept {
        return { point - otherPoint.point };
    }

    constexpr float distance(const Point otherPoint) const noexcept {
        return ((*this) - otherPoint).norm();
    }

    Array<Type, Size> point;
};

template<arithmetic First, arithmetic... Rest> requires same_type<First, Rest...>
Point(First, Rest...)->Point<First, sizeof...(Rest) + 1>;

using point2i = Point<int, 2>;
using point3i = Point<int, 3>;
using point4i = Point<int, 4>;

using point2f = Point<float, 2>;
using point3f = Point<float, 3>;
using point4f = Point<float, 4>;

}