#pragma once

namespace euclid::util {

template<arithmetic T>
EUCLID_FORCEINLINE constexpr void swap(T& left, T& right) noexcept {
    T temp = left;
    left   = right;
    right  = temp;
}

// Todo : Make a 'static for' thing to unroll loop

}