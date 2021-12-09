#pragma once
#ifndef _WEEKNDPOINT_HPP
#define _WEEKNDPOINT_HPP

#include "Vector.hpp"

namespace euclid {

template<arithmetic Type, size_t Size>
class Point {
public:
    template<arithmetic T>
    constexpr Point<T, Size> cast() const noexcept {
        return { 
            static_cast<T>(point[0]),
            static_cast<T>(point[1]),
            static_cast<T>(point[2]),
            static_cast<T>(point[3])
        };
    }

    constexpr Point& negative() noexcept {
        point[0] = -point[0];
        point[1] = -point[1];
        point[2] = -point[2];
        point[3] = -point[3];
        return *this;
    }

    constexpr void print() const noexcept {
        #ifdef _IOSTREAM_
            std::cout << '(';
            for (size_t i = 0; i < Size - 1; ++i) {
                std::cout << point[i] << ", ";
            }
            std::cout << point[Size - 1] << ")\n";
        #endif // _IOSTREAM_
    }

    constexpr Point operator-() const noexcept {
        return { -point[0], -point[1], -point[2], -point[3] };
    }

    template<arithmetic T>
    constexpr Vector<arithmetic_promotion_t<Type, T>, Size> operator-(const Point<T, Size>& otherPoint) const noexcept {
        return { 
            point[0] - otherPoint[0],
            point[1] - otherPoint[1],
            point[2] - otherPoint[2],
            point[3] - otherPoint[3]
        };
    }

    template<arithmetic T>
    constexpr auto distance(const Point<T, Size>& otherPoint) const noexcept {
        return ((*this) - otherPoint).norm();
    }

    constexpr Type& operator[](const size_t pos) noexcept {
        return point[pos];
    }
    
    constexpr Type operator[](const size_t pos) const noexcept {
        return point[pos];
    }

    Type point[4]{};
};

template<arithmetic First, arithmetic... Rest> requires same_type<First, Rest...>
Point(First, Rest...)->Point<First, sizeof...(Rest) + 1>;

template<arithmetic T>
using point2 = Point<T, 2>;

template<arithmetic T>
using point3 = Point<T, 3>;

template<arithmetic T>
using point4 = Point<T, 4>;

using point2i = Point<int, 2>;
using point3i = Point<int, 3>;
using point4i = Point<int, 4>;

using point2u = Point<unsigned, 2>;
using point3u = Point<unsigned, 3>;
using point4u = Point<unsigned, 4>;

using point2f = Point<float, 2>;
using point3f = Point<float, 3>;
using point4f = Point<float, 4>;

using point2d = Point<double, 2>;
using point3d = Point<double, 3>;
using point4d = Point<double, 4>;

}

#endif // _WEEKNDPOINT_HPP