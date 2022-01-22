#pragma once

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4514) // remove unused inline functions
#endif // _MSC_VER

namespace euclid::math {

template<float_point_type Ty>
inline constexpr Ty pi = static_cast<Ty>(3.1415926535897932384626433832795028841972);

inline constexpr float radian = pi<float> / 180;

EuclidForceinline constexpr float sqrt(const float number) noexcept {
    if (__builtin_is_constant_evaluated()) {
        auto value = __builtin_bit_cast(unsigned, number) >> 1;
        auto magic = __builtin_bit_cast(float, 0x5f1ffff9 - value);
        return magic *= (0.703952253f * (2.38924456f - number * magic * magic)) * number;
    }
#ifdef _MSC_VER
    return _mm256_sqrt_ps(_mm256_set1_ps(number)).m256_f32[0];
#else // __GNUC__
    return _mm256_sqrt_ps(_mm256_set1_ps(number))[0];
#endif
}

EuclidForceinline constexpr float cos(const float angle) noexcept {
    const float first  = angle * radian * angle * radian;
    const float second = first * first;
    const float third  = first * second;
    return 1.f - 0.5f * first + second * 0.04166666791f - third * 0.001361971023f;
}

EuclidForceinline constexpr float sin(const float angle) noexcept {
    const float first  = angle  * radian;
    const float square = first  * first;
    const float second = first  * square;
    const float third  = second * square;
    const float forth  = third  * square;
    return first - second * 0.1666666716f + third * 0.008333333768f - forth * 0.0001984127011f;
}

EuclidForceinline constexpr float tan(const float angle) noexcept {
    const float first  = (90 - angle) * radian;
    const float square = first  * first;
    const float second = first  * square;
    const float third  = second * square;
    const float forth  = third  * square;
    return -(-1.f / first + first / 3.f + second / 45.f + third * 2.f / 945.f + forth / 4725.f);
}

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER