#pragma once

#include "Vec8.hpp"

namespace euclid {

struct alignas(32) mat4i {
    using value_type = int;
    vec8i simd[2];
};

struct alignas(32) mat4f {
    using value_type = float;
    vec8f simd[2];
};

mat4i addMat4i(const mat4i a, const mat4i b) noexcept {
    const vec8i temp1 = addVec8i(a.simd[0], b.simd[0]);
    const vec8i temp2 = addVec8i(a.simd[1], b.simd[1]);
    return { temp1, temp2 };
}


}