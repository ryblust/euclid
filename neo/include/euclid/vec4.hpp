#pragma once

#include "defines.hpp"
#include "math.hpp"

#include <immintrin.h>
#include <type_traits>

namespace euclid {

#ifdef __clang__

// Clang doest not have constexpr operator overloads for __m128.
// Here' s a workaround for clang to make Vec4 fully constexpr.
struct alignas(16) Vec4 final {
    constexpr Vec4() noexcept : forconstexpr{} {} ;
    constexpr Vec4(float x, float y, float z, float w) noexcept : forconstexpr{x,y,z,w} {}
    constexpr Vec4(__m128 vec) noexcept : xyzw(vec) {}
    constexpr operator __m128() const noexcept { return xyzw; }

    union { float forconstexpr[4]; __m128 xyzw; };
};

constexpr float& GetVec4Data(Vec4& a, std::size_t i) noexcept { return a.forconstexpr[i]; }
constexpr const float& GetVec4Data(const Vec4& a, std::size_t i) noexcept { return a.forconstexpr[i]; }

#elif __GNUC__

// GCC already has constexpr operator overloads for __m128
using Vec4 = __m128;

constexpr float& GetVec4Data(Vec4& a, std::size_t i) noexcept { return a[i]; }
constexpr const float& GetVec4Data(const Vec4& a, std::size_t i) noexcept { return a[i]; }

#elif _MSC_VER

// Directly use __m128 in MSVC can avoid Buffer Security Check
// in many scenarios. Use __declspec(safebuffers) is not perfect
// unless you enable the /GS- compiler option.
using Vec4 = __m128;

constexpr float& GetVec4Data(Vec4& a, std::size_t i) noexcept { return a.m128_f32[i]; }
constexpr const float& GetVec4Data(const Vec4& a, std::size_t i) noexcept { return a.m128_f32[i]; }

constexpr Vec4 __vectorcall operator+(Vec4 a, Vec4 b) noexcept
{
    if (std::is_constant_evaluated()) {
        return Vec4 {
            a.m128_f32[0] + b.m128_f32[0],
            a.m128_f32[1] + b.m128_f32[1],
            a.m128_f32[2] + b.m128_f32[2],
            a.m128_f32[3] + b.m128_f32[3]
        };
    }
    return _mm_add_ps(a, b);
}

constexpr Vec4 __vectorcall operator-(Vec4 a, Vec4 b) noexcept
{
    if (std::is_constant_evaluated()) {
        return Vec4 {
            a.m128_f32[0] - b.m128_f32[0],
            a.m128_f32[1] - b.m128_f32[1],
            a.m128_f32[2] - b.m128_f32[2],
            a.m128_f32[3] - b.m128_f32[3]
        };
    }
    return _mm_sub_ps(a, b);
}

constexpr Vec4 __vectorcall operator*(Vec4 a, Vec4 b) noexcept
{
    if (std::is_constant_evaluated()) {
        return Vec4 {
            a.m128_f32[0] * b.m128_f32[0],
            a.m128_f32[1] * b.m128_f32[1],
            a.m128_f32[2] * b.m128_f32[2],
            a.m128_f32[3] * b.m128_f32[3]
        };
    }
    return _mm_mul_ps(a, b);
}

constexpr Vec4 __vectorcall operator/(Vec4 a, Vec4 b) noexcept
{
    if (std::is_constant_evaluated()) {
        return Vec4 {
            a.m128_f32[0] / b.m128_f32[0],
            a.m128_f32[1] / b.m128_f32[1],
            a.m128_f32[2] / b.m128_f32[2],
            a.m128_f32[3] / b.m128_f32[3]
        };
    }
    return _mm_div_ps(a, b);
}

constexpr bool __vectorcall operator==(Vec4 a, Vec4 b) noexcept
{
    if (std::is_constant_evaluated()) {
        return
            a.m128_f32[0] == b.m128_f32[0] &&
            a.m128_f32[1] == b.m128_f32[1] &&
            a.m128_f32[2] == b.m128_f32[2] &&
            a.m128_f32[3] == b.m128_f32[3];
    }
    return _mm_movemask_ps(_mm_cmpeq_ps(a, b)) == 0xf;
}

constexpr Vec4 __vectorcall operator-(Vec4 a) noexcept { return Vec4{} - a; }

constexpr Vec4 __vectorcall operator*(Vec4 a, float v) noexcept { return a * Vec4{v,v,v,v}; }
constexpr Vec4 __vectorcall operator/(Vec4 a, float v) noexcept { return a / Vec4{v,v,v,v}; }
constexpr Vec4 __vectorcall operator*(float v, Vec4 a) noexcept { return a * Vec4{v,v,v,v}; }

constexpr Vec4& __vectorcall operator+=(Vec4& a, Vec4 b) noexcept { return a += b; }
constexpr Vec4& __vectorcall operator-=(Vec4& a, Vec4 b) noexcept { return a -= b; }
constexpr Vec4& __vectorcall operator*=(Vec4& a, Vec4 b) noexcept { return a *= b; }
constexpr Vec4& __vectorcall operator/=(Vec4& a, Vec4 b) noexcept { return a /= b; }

constexpr Vec4& __vectorcall operator*=(Vec4& a, float v) noexcept { return a *= v; }
constexpr Vec4& __vectorcall operator/=(Vec4& a, float v) noexcept { return a /= v; }

constexpr bool __vectorcall operator!=(Vec4 a, Vec4 b) noexcept { return !(a == b); }

#endif // Vec4 for the big three compiler

constexpr Vec4 EUCLID_CALL Clamp(Vec4 vec, Vec4 min, Vec4 max) noexcept
{
    if (std::is_constant_evaluated()) {
        return Vec4 {
            math::Clamp(GetVec4Data(vec, 0), GetVec4Data(min, 0), GetVec4Data(max, 0)),
            math::Clamp(GetVec4Data(vec, 1), GetVec4Data(min, 1), GetVec4Data(max, 1)),
            math::Clamp(GetVec4Data(vec, 2), GetVec4Data(min, 2), GetVec4Data(max, 2)),
            math::Clamp(GetVec4Data(vec, 3), GetVec4Data(min, 3), GetVec4Data(max, 3))
        };
    }
    return _mm_max_ps(_mm_min_ps(vec, max), min);
}

constexpr Vec4 EUCLID_CALL Clamp(Vec4 vec, float min, float max) noexcept
{
    return Clamp(vec, Vec4{min, min, min, min}, Vec4{max, max, max, max});
}

constexpr Vec4 EUCLID_CALL Lerp(Vec4 a, Vec4 b, Vec4 t) noexcept
{
    if (std::is_constant_evaluated()) {
        return Vec4 {
            math::Lerp(GetVec4Data(a, 0), GetVec4Data(b, 0), GetVec4Data(t, 0)),
            math::Lerp(GetVec4Data(a, 1), GetVec4Data(b, 1), GetVec4Data(t, 1)),
            math::Lerp(GetVec4Data(a, 2), GetVec4Data(b, 2), GetVec4Data(t, 2)),
            math::Lerp(GetVec4Data(a, 3), GetVec4Data(b, 3), GetVec4Data(t, 3))
        };
    }
    return _mm_fmadd_ps(t, _mm_sub_ps(b, a), a);
}

constexpr Vec4 EUCLID_CALL Lerp(Vec4 a, Vec4 b, float t) noexcept
{
    return Lerp(a, b, Vec4{t, t, t, t});
}

} // namespace euclid