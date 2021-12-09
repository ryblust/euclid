#pragma once
#ifndef _WEEKNDVECTOR_HPP
#define _WEEKNDVECTOR_HPP

#include "Scalar.hpp"
#include "Common/Math.hpp"

namespace euclid {

template<arithmetic Type, size_t Size>
class Vector {
public:
    template<arithmetic T>
    constexpr Vector<T, Size> cast() const noexcept {
        return { 
            static_cast<T>(vec[0]),
            static_cast<T>(vec[1]),
            static_cast<T>(vec[2]),
            static_cast<T>(vec[3])
        };
    }

    constexpr arithmetic_promotion_t<Type, float> norm() const noexcept {
        const Type squareSum = vec[0] * vec[0] + vec[1] * vec[1] + 
                               vec[2] * vec[2] + vec[3] * vec[3];
        if (squareSum) [[likely]] {
            return math::sqrt(static_cast<float>(squareSum));
        } else [[unlikely]] {
            return 0;
        }
    }

    constexpr Vector<arithmetic_promotion_t<Type, float>, Size> normalize() const noexcept {
        if (const auto normValue = this->norm(); normValue) [[likely]] {
            return {
                vec[0] / normValue,
                vec[1] / normValue,
                vec[2] / normValue,
                vec[3] / normValue
            };
        } else [[unlikely]] {
            return {};
        }
    }

    constexpr Vector& negative() noexcept {
        vec[0] = -vec[0];
        vec[1] = -vec[1];
        vec[2] = -vec[2];
        vec[3] = -vec[3];
        return *this;
    }

    template<arithmetic T>
    constexpr Vector<arithmetic_promotion_t<Type, T>, Size == 2 ? 3 : Size> cross(const Vector<T, Size>& otherVec) const noexcept {
        return { 
            vec[1] * otherVec[2] - vec[2] * otherVec[1],
            vec[2] * otherVec[0] - vec[0] * otherVec[2],
            vec[0] * otherVec[1] - vec[1] * otherVec[0]
        };
    }

    template<arithmetic T>
    constexpr auto dot(const Vector<T, Size>& otherVec) const noexcept {
        return vec[0] * otherVec[0] + vec[1] * otherVec[1] + vec[2] * otherVec[2];
    }

    template<arithmetic T>
    constexpr auto included_angle(const Vector<T, Size>& otherVec) const noexcept {
        return this->dot(otherVec) / (this->norm() * otherVec.norm());
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
    constexpr auto distance(const Vector<T, Size>& otherVec) const noexcept {
        return ((*this) - otherVec).norm();
    }

    constexpr Vector operator-() const noexcept {
        return { -vec[0], -vec[1], -vec[2], -vec[3] };
    }

    template<arithmetic T> requires acceptable_loss<Type, T>
    constexpr Vector& operator+=(const Vector<T, Size>& otherVec) noexcept {
        vec[0] += otherVec[0];
        vec[1] += otherVec[1];
        vec[2] += otherVec[2];
        vec[3] += otherVec[3];
        return *this;
    }

    template<arithmetic T> requires acceptable_loss<Type, T>
    constexpr Vector& operator-=(const Vector<T, Size>& otherVec) noexcept {
        vec[0] -= otherVec[0];
        vec[1] -= otherVec[1];
        vec[2] -= otherVec[2];
        vec[3] -= otherVec[3];
        return *this;
    }

    template<arithmetic T>
    constexpr Vector<arithmetic_promotion_t<Type, T>, Size> operator+(const Vector<T, Size>& otherVec) const noexcept {
        return {
            vec[0] + otherVec[0],
            vec[1] + otherVec[1],
            vec[2] + otherVec[2],
            vec[3] + otherVec[3]
        };
    }

    template<arithmetic T>
    constexpr Vector<arithmetic_promotion_t<Type, T>, Size> operator-(const Vector<T, Size>& otherVec) const noexcept {
        return { 
            vec[0] - otherVec[0],
            vec[1] - otherVec[1],
            vec[2] - otherVec[2],
            vec[3] - otherVec[3]
        };
    }

    template<arithmetic T> requires acceptable_loss<Type, T>
    constexpr Vector& operator*=(const Scalar<T> scalar) noexcept {
        vec[0] *= scalar;
        vec[1] *= scalar;
        vec[2] *= scalar;
        vec[3] *= scalar;
        return *this;
    }

    template<arithmetic T> requires acceptable_loss<Type, T>
    constexpr Vector& operator*=(const T scalar) noexcept {
        vec[0] *= scalar;
        vec[1] *= scalar;
        vec[2] *= scalar;
        vec[3] *= scalar;
        return *this;
    }

    template<arithmetic T>
    constexpr Vector<arithmetic_promotion_t<Type, T>, Size> operator*(const Scalar<T> scalar) const noexcept {
        return { 
            vec[0] * scalar,
            vec[1] * scalar,
            vec[2] * scalar,
            vec[3] * scalar
        };
    }

    template<arithmetic T>
    constexpr Vector<arithmetic_promotion_t<Type, T>, Size> operator*(const T scalar) const noexcept {
        return {
            vec[0] * scalar,
            vec[1] * scalar,
            vec[2] * scalar,
            vec[3] * scalar
        };
    }

    constexpr Type& operator[](const size_t pos) noexcept {
        return vec[pos];
    }

    constexpr Type operator[](const size_t pos) const noexcept {
        return vec[pos];
    }

    Type vec[4]{};
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

template<arithmetic T>
using vec2 = Vector<T, 2>;

template<arithmetic T>
using vec3 = Vector<T, 3>;

template<arithmetic T>
using vec4 = Vector<T, 4>;

using vec2i = Vector<int, 2>;
using vec3i = Vector<int, 3>;
using vec4i = Vector<int, 4>;

using vec2u = Vector<unsigned, 2>;
using vec3u = Vector<unsigned, 3>;
using vec4u = Vector<unsigned, 4>;

using vec2f = Vector<float, 2>;
using vec3f = Vector<float, 3>;
using vec4f = Vector<float, 4>;

using vec2d = Vector<double, 2>;
using vec3d = Vector<double, 3>;
using vec4d = Vector<double, 4>;

}

#endif // _WEEKNDVECTOR_HPP