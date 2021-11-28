#pragma once
#ifndef _SORRYSCALAR_HPP
#define _SORRYSCALAR_HPP

#include "Common/Concepts.hpp"

namespace euclid {

template<arithmetic Type>
class Scalar {
public:
    using value_type = Type;
    using this_type  = Scalar;
    using this_ref   = Scalar&;

    constexpr Scalar(const value_type val) noexcept : scalar(val) {}

    constexpr operator value_type() const noexcept {
        return scalar;
    }

    constexpr this_ref negative() noexcept {
        scalar = -scalar;
        return *this;
    }

    constexpr this_type operator-() const noexcept {
        return -scalar;
    }

    constexpr void print() const noexcept {
        #ifdef _IOSTREAM_
            std::cout << scalar << '\n';
        #endif // IOSTREAM_
    }

    template<arithmetic T>
    constexpr Scalar<T> cast() const noexcept {
        return static_cast<T>(scalar);
    }
public:
    value_type scalar{};
};

using scalari = Scalar<int>;
using scalaru = Scalar<unsigned>;
using scalarf = Scalar<float>;
using scalard = Scalar<double>;

}

#endif // _SORRYSCALAR_HPP