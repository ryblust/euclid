#pragma once
#ifndef _SORRYVECTOR_HPP
#define _SORRYVECTOR_HPP

#include "Scalar.hpp"
#include "Common/Math.hpp"

namespace euclid {

template<arithmetic Type, size_t Size>
class Vector {
public:
    using value_type = Type;
    using reference  = Type&;
    using this_type  = Vector;
    using this_ref   = Vector&;

    template<arithmetic T>
    constexpr Vector<T, Size> cast() const noexcept {
        return { static_cast<T>(vec[0]),
                 static_cast<T>(vec[1]),
                 static_cast<T>(vec[2]),
                 static_cast<T>(vec[3])
        };
    }

    constexpr scalarf norm() const noexcept {
        return math::sqrt(static_cast<float>(vec[0] * vec[0] +
                                             vec[1] * vec[1] +
                                             vec[2] * vec[2] +
                                             vec[3] * vec[3]));
    }

    constexpr auto normalize() const noexcept {
        using type = arithmetic_promotion_t<Type, float>;
        const scalarf normValue = norm();
        return Vector<type, Size> { vec[0] / normValue,
                                    vec[1] / normValue,
                                    vec[2] / normValue,
                                    vec[3] / normValue };
    }

    constexpr this_ref negative() noexcept {
        vec[0] = -vec[0];
        vec[1] = -vec[1];
        vec[2] = -vec[2];
        vec[3] = -vec[3];
        return *this;
    }

    template<arithmetic T>
    constexpr auto cross(const Vector<T, Size>& otherVec) const noexcept {
        using type = arithmetic_promotion_t<Type, T>;
        return Vector<type, Size> { vec[1] * otherVec[2] - vec[2] * otherVec[1],
                                    vec[2] * otherVec[0] - vec[0] * otherVec[2],
                                    vec[0] * otherVec[1] - vec[1] * otherVec[0] };
    }

    template<arithmetic T>
    constexpr auto dot(const Vector<T, Size>& otherVec) const noexcept {
        return Scalar{ vec[0] * otherVec[0] +
                       vec[1] * otherVec[1] +
                       vec[2] * otherVec[2]
        };
    }

    template<arithmetic T>
    constexpr auto included_angle(const Vector<T, Size>& otherVec) const noexcept {
        return Scalar{ this->dot(otherVec) / (this->norm() * otherVec.norm()) };
    }

    constexpr void print() const noexcept {
        #ifdef _IOSTREAM_
            std::cout << '[';
            for (size_t i = 0; i < Size - 1; ++i) {
                std::cout << vec[i] << ", ";
            }
            std::cout << vec[Size - 1] << "]\n";
        #endif // _IOSTREAM_
    }

    template<arithmetic T>
    constexpr scalarf distance(const Vector<T, Size>& otherVec) const noexcept {
        return ((*this) - otherVec).norm();
    }

    constexpr this_type operator-() const noexcept {
        return { -vec[0], -vec[1], -vec[2], -vec[3] };
    }

    template<arithmetic T> requires acceptable_loss<Type, T>
    constexpr this_ref operator+=(const Vector<T, Size>& otherVec) noexcept {
        vec[0] += otherVec[0];
        vec[1] += otherVec[1];
        vec[2] += otherVec[2];
        vec[3] += otherVec[3];
        return *this;
    }

    template<arithmetic T> requires acceptable_loss<Type, T>
    constexpr this_ref operator-=(const Vector<T, Size>& otherVec) noexcept {
        vec[0] -= otherVec[0];
        vec[1] -= otherVec[1];
        vec[2] -= otherVec[2];
        vec[3] -= otherVec[3];
        return *this;
    }

    template<arithmetic T>
    constexpr auto operator+(const Vector<T, Size>& otherVec) const noexcept {
        using type = arithmetic_promotion_t<Type, T>;
        return Vector<type, Size> { vec[0] + otherVec[0],
                                    vec[1] + otherVec[1],
                                    vec[2] + otherVec[2], 
                                    vec[3] + otherVec[3] };
    }

    template<arithmetic T>
    constexpr auto operator-(const Vector<T, Size>& otherVec) const noexcept {
        using type = arithmetic_promotion_t<Type, T>;
        return Vector<type, Size> { vec[0] - otherVec[0],
                                    vec[1] - otherVec[1],
                                    vec[2] - otherVec[2],
                                    vec[3] - otherVec[3] };
    }

    template<arithmetic T> requires acceptable_loss<Type, T>
    constexpr this_ref operator*=(const Scalar<T> scalar) noexcept {
        vec[0] *= scalar;
        vec[1] *= scalar;
        vec[2] *= scalar;
        vec[3] *= scalar;
        return *this;
    }

    template<arithmetic T> requires acceptable_loss<Type, T>
    constexpr this_ref operator*=(const T scalar) noexcept {
        return (*this) *= Scalar(scalar);
    }

    template<arithmetic T>
    constexpr auto operator*(const Scalar<T> scalar) const noexcept {
        using type = arithmetic_promotion_t<Type, T>;
        return Vector<type, Size> { vec[0] * scalar,
                                    vec[1] * scalar,
                                    vec[2] * scalar,
                                    vec[3] * scalar };
    }

    constexpr auto operator*(arithmetic auto const scalar) const noexcept {
        return (*this) * Scalar(scalar);
    }

    constexpr reference operator[](size_t pos) noexcept {
        return vec[pos];
    }

    constexpr value_type operator[](size_t pos) const noexcept {
        return vec[pos];
    }
public:
    value_type vec[4]{};
};

template<arithmetic Ty1, arithmetic Ty2, size_t S> inline
constexpr auto operator*(const Scalar<Ty1> scalar, const Vector<Ty2, S>& vec) noexcept {
    return vec * scalar;
}

template<arithmetic Ty1, arithmetic Ty2, size_t S> inline
constexpr auto operator*(const Ty1 scalar, const Vector<Ty2, S>& vec) noexcept {
    return vec * scalar;
}

template<arithmetic First, arithmetic... Rest> requires same_type<First, Rest...>
Vector(First, Rest...)->Vector<First, sizeof...(Rest) + 1>;

#pragma region TypeDefineForVector
// for 2d Vector
template<arithmetic T>
using vec2 = Vector<T, 2>;
// for 3d Vector
template<arithmetic T>
using vec3 = Vector<T, 3>;
// for 4d Vector
template<arithmetic T>
using vec4 = Vector<T, 4>;

// for integer Vector
using vec2i = Vector<int, 2>;
using vec3i = Vector<int, 3>;
using vec4i = Vector<int, 4>;
// for unsigned Vector
using vec2u = Vector<unsigned, 2>;
using vec3u = Vector<unsigned, 3>;
using vec4u = Vector<unsigned, 4>;
// for float Vector
using vec2f = Vector<float, 2>;
using vec3f = Vector<float, 3>;
using vec4f = Vector<float, 4>;
// for double Vector
using vec2d = Vector<double, 2>;
using vec3d = Vector<double, 3>;
using vec4d = Vector<double, 4>;
#pragma endregion

}

#endif // _SORRYVECTOR_HPP