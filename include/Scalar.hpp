#pragma once
#ifndef _WEEKNDSCALAR_HPP
#define _WEEKNDSCALAR_HPP

#include "Common/Core.hpp"

namespace euclid {

template<arithmetic Type>
class Scalar {
public:
    constexpr Scalar() noexcept {}

    constexpr Scalar(const Type val) noexcept : scalar(val) {}

    constexpr operator Type() const noexcept {
        return scalar;
    }

    constexpr Scalar& negative() noexcept {
        scalar = -scalar;
        return *this;
    }

    constexpr Scalar operator-() const noexcept {
        return -scalar;
    }

    constexpr void print() const noexcept {
        #ifdef _IOSTREAM_
            std::cout << scalar << '\n';
        #endif // _IOSTREAM_
    }

    template<arithmetic T>
    constexpr Scalar<T> cast() const noexcept {
        return static_cast<T>(scalar);
    }

    Type scalar{};
};

using scalari = Scalar<int>;
using scalaru = Scalar<unsigned>;
using scalarf = Scalar<float>;
using scalard = Scalar<double>;

}

#endif // _WEEKNDSCALAR_HPP