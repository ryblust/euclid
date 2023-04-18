module;
#include "Core.hpp"

export module Euclid.Vec4;
export import Euclid.Math;

export namespace Euclid {

struct alignas(16) Vec4 {

    constexpr float& operator[](std::size_t i) noexcept
    {
#ifdef __clang__
        return xyzw[i];
#elif _MSC_VER
        return data.m128_f32[i];
#else
        return data[i];
#endif
    }

    constexpr float operator[](std::size_t i) const noexcept
    {
        return const_cast<Vec4*>(this)->operator[](i);
    }

#ifdef __clang__
    // clang does not support to get m128 field at compile time
    // it's a workaround for clang to make Vec4 fully constexpr
    union {
        float xyzw[4];
        __m128 data;
    };
#else
    __m128 data;
#endif
};

constexpr bool EUCLID_CALL NearlyEqual(Vec4 a, Vec4 b) noexcept
{
    // consider a better floating-point comparison function

    if (std::is_constant_evaluated())
        return
            Math::NearlyEqual(a[0], b[0]) &&
            Math::NearlyEqual(a[1], b[1]) &&
            Math::NearlyEqual(a[2], b[2]) &&
            Math::NearlyEqual(a[3], b[3]);

    const __m128 absmask = _mm_castsi128_ps(_mm_set1_epi32(0x7fffffff));
    const __m128 epsilon = _mm_set1_ps(std::numeric_limits<float>::epsilon());
    const __m128 subdata = _mm_sub_ps(a.data, b.data);
    const __m128 andabsm = _mm_and_ps(subdata, absmask);

    return _mm_movemask_ps(_mm_cmplt_ps(andabsm, epsilon)) == 0xf;
}

} // namespace Euclid

using namespace Euclid;

export {

constexpr Vec4 EUCLID_CALL operator+(Vec4 a, Vec4 b) noexcept
{
    if (std::is_constant_evaluated())
        return Vec4{a[0] + b[0], a[1] + b[1], a[2] + b[2], a[3] + b[3]};

    return {.data = _mm_add_ps(a.data, b.data)};
}

constexpr Vec4 EUCLID_CALL operator-(Vec4 a, Vec4 b) noexcept
{
    if (std::is_constant_evaluated())
        return Vec4{a[0] - b[0], a[1] - b[1], a[2] - b[2], a[3] - b[3]};

    return {.data = _mm_sub_ps(a.data, b.data)};
}

constexpr Vec4 EUCLID_CALL operator*(Vec4 a, Vec4 b) noexcept
{
    if (std::is_constant_evaluated())
        return Vec4{a[0] * b[0], a[1] * b[1], a[2] * b[2], a[3] * b[3]};

    return {.data = _mm_mul_ps(a.data, b.data)};
}

constexpr Vec4 EUCLID_CALL operator/(Vec4 a, Vec4 b) noexcept
{
    if (std::is_constant_evaluated())
        return Vec4{a[0] / b[0], a[1] / b[1], a[2] / b[2], a[3] / b[3]};

    return {.data = _mm_div_ps(a.data, b.data)};
}

constexpr bool EUCLID_CALL operator==(Vec4 a, Vec4 b) noexcept
{
    if (std::is_constant_evaluated())
        return NearlyEqual(a, b);

    return _mm_movemask_ps(_mm_cmpeq_ps(a.data, b.data)) == 0xf;
}

constexpr Vec4 EUCLID_CALL operator-(Vec4 a) noexcept { return Vec4{} - a; }

constexpr Vec4 EUCLID_CALL operator*(Vec4 a, float v) noexcept { return a * Vec4{v,v,v,v}; }
constexpr Vec4 EUCLID_CALL operator/(Vec4 a, float v) noexcept { return a / Vec4{v,v,v,v}; }
constexpr Vec4 EUCLID_CALL operator*(float v, Vec4 a) noexcept { return a * Vec4{v,v,v,v}; }

constexpr Vec4& EUCLID_CALL operator+=(Vec4& a, Vec4 b) noexcept { a = a + b; return a; }
constexpr Vec4& EUCLID_CALL operator-=(Vec4& a, Vec4 b) noexcept { a = a - b; return a; }
constexpr Vec4& EUCLID_CALL operator*=(Vec4& a, Vec4 b) noexcept { a = a * b; return a; }
constexpr Vec4& EUCLID_CALL operator/=(Vec4& a, Vec4 b) noexcept { a = a / b; return a; }

constexpr Vec4& EUCLID_CALL operator*=(Vec4& a, float v) noexcept { a = a * v; return a; }
constexpr Vec4& EUCLID_CALL operator/=(Vec4& a, float v) noexcept { a = a / v; return a; }

constexpr bool EUCLID_CALL operator!=(Vec4 a, Vec4 b) noexcept { return !(a == b); }

}