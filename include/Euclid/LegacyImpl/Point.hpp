#pragma once

namespace euclid {

template<euclid_type Type, std::size_t Size> requires (Size <= 8)
struct alignas(Size <= 4 ? 16 : 32) Point {
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
        return ((*this) - otherPoint).length();
    }

    Array<Type, Size> point;
};

template<euclid_type First, euclid_type... Rest> requires same_type<First, Rest...>
Point(First, Rest...)->Point<First, sizeof...(Rest) + 1>;

template<typename T>
struct is_point {
    static constexpr bool value = false;
};

template<euclid_type T, std::size_t S>
struct is_point<Point<T, S>> {
    static constexpr bool value = true;
};

}