#pragma once

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4514)
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
    } else {
#ifdef _MSC_VER
        return _mm256_sqrt_ps(_mm256_set1_ps(number)).m256_f32[0];
#else // __GNUC__
        return _mm256_sqrt_ps(_mm256_set1_ps(number))[0];
#endif
    }
}

EuclidForceinline constexpr float cos(const float angle) noexcept {
    if (__builtin_is_constant_evaluated()) {
        const float first  = angle * radian * angle * radian;
        const float second = first * first;
        const float third  = first * second;
        return 1.f - 0.5f * first + second * 0.04166666791f - third * 0.001361971023f;
    } else {
        const __m256 rad    = _mm256_mul_ps(_mm256_set1_ps(angle), _mm256_set1_ps(radian));
        const __m256 first  = _mm256_mul_ps(rad,   rad);
        const __m256 second = _mm256_mul_ps(first, first);
        const __m256 third  = _mm256_mul_ps(first, second);
        const __m256 result = _mm256_sub_ps(_mm256_set1_ps(1.f),
                              _mm256_add_ps(_mm256_mul_ps(_mm256_set1_ps(0.5f),            first),
                              _mm256_sub_ps(_mm256_mul_ps(_mm256_set1_ps(0.04166666791f),  second),
                                            _mm256_mul_ps(_mm256_set1_ps(0.001361971023f), third))));
#ifdef _MSC_VER
        return result.m256_f32[0];
#else // __GNUC__
        return result[0];
#endif
    }
}

EuclidForceinline constexpr float sin(const float angle) noexcept {
    if (__builtin_is_constant_evaluated()) {
        const float first  = angle  * radian;
        const float square = first  * first;
        const float second = first  * square;
        const float third  = second * square;
        const float forth  = third  * square;
        return first - second * 0.1666666716f + third * 0.008333333768f - forth * 0.0001984127011f;
    } else {
        const __m256 first  = _mm256_mul_ps(_mm256_set1_ps(angle), _mm256_set1_ps(radian));
        const __m256 square = _mm256_mul_ps(first,  first);
        const __m256 second = _mm256_mul_ps(first,  square);
        const __m256 third  = _mm256_mul_ps(second, square);
        const __m256 forth  = _mm256_mul_ps(third,  square);
        const __m256 result = _mm256_sub_ps(first,
                              _mm256_add_ps(_mm256_mul_ps(_mm256_set1_ps(0.1666666716f),    second),
                              _mm256_sub_ps(_mm256_mul_ps(_mm256_set1_ps(0.008333333768f),  third),
                                            _mm256_mul_ps(_mm256_set1_ps(0.0001984127011f), forth))));
#ifdef _MSC_VER
        return result.m256_f32[0];
#else // __GUNC__
        return result[0];
#endif
    }
}

EuclidForceinline constexpr float tan(const float angle) noexcept {
    if (__builtin_is_constant_evaluated()) {
        const float first  = (90 - angle) * radian;
        const float square = first  * first;
        const float second = first  * square;
        const float third  = second * square;
        const float forth  = third  * square;
        return -(-1.f / first + first / 3.f + second / 45.f + third * 2.f / 945.f + forth / 4725.f);
    } else {
        const __m256 first  = _mm256_mul_ps(_mm256_sub_ps(_mm256_set1_ps(90.f), _mm256_set1_ps(angle)), _mm256_set1_ps(radian));
        const __m256 square = _mm256_mul_ps(first,  first);
        const __m256 second = _mm256_mul_ps(first,  square);
        const __m256 third  = _mm256_mul_ps(second, square);
        const __m256 forth  = _mm256_mul_ps(third,  square);
        const __m256 result = _mm256_sub_ps(_mm256_setzero_ps(), 
                              _mm256_add_ps(_mm256_div_ps(_mm256_set1_ps(-1.f),             first),
                              _mm256_add_ps(_mm256_mul_ps(_mm256_set1_ps(0.3333333433f),    first),
                              _mm256_add_ps(_mm256_mul_ps(_mm256_set1_ps(0.02222222276f),   second),
                              _mm256_add_ps(_mm256_mul_ps(_mm256_set1_ps(0.002116402145f),  third),
                                            _mm256_mul_ps(_mm256_set1_ps(0.0002116402175f), forth))))));
#ifdef _MSC_VER
        return result.m256_f32[0];
#else // __GUNC__
        return result[0];
#endif
    }
}

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER