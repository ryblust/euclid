#pragma once

#include "Core.h"

namespace euclid::util {

template<arithmetic T>
EUCLID_FORCEINLINE constexpr void swap(T& a, T& b) noexcept {
    T temp = a;
    a = b;
    b = temp;
}

}