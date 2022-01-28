#pragma once

#include "Vec4.hpp"

namespace euclid {

struct alignas(16) mat2i {
    using value_type = int;

    EUCLID_MSVC_CONSTEXPR void operator+=(const mat2i m) noexcept {
#ifdef _MSC_VER
        if (__builtin_is_constant_evaluated()) {
            for (std::size_t i = 0; i < 4; ++i) {
                simd.m128i_i32[i] += m.simd.m128i_i32[i];
            }
            return;
        }
#endif
        simd = addVec4i(simd, m.simd);
    }

    EUCLID_MSVC_CONSTEXPR void operator-=(const mat2i m) noexcept {
#ifdef _MSC_VER
        if (__builtin_is_constant_evaluated()) {
            for (std::size_t i = 0; i < 4; ++i) {
                simd.m128i_i32[i] -= m.simd.m128i_i32[i];
            }
            return;
        }
#endif
        simd = addVec4i(simd, m.simd);
    }

    EUCLID_MSVC_CONSTEXPR mat2i operator+(const mat2i m) const noexcept {
        mat2i temp = *this;
        temp += m;
        return temp;
    }

    EUCLID_MSVC_CONSTEXPR mat2i operator-(const mat2i m) const noexcept {
        mat2i temp = *this;
        temp -= m;
        return temp;
    }

    vec4i simd;
};

EUCLID_FUNC_QUALIFIER mat2i EUCLID_CALL setMat2i(const int a0, const int a1, const int a2, const int a3) noexcept {
    return { setVec4i(a0, a1, a2, a3) };
}


}