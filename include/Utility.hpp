#pragma once

namespace euclid::util {

template<arithmetic T>
EuclidForceinline constexpr T clamp(const T val, const T min, const T max) noexcept {
    return val < min ? min : max < val ? max : val;
}

template<float_point_type T> 
EuclidForceinline constexpr T lerp(const T a, const T b, const T t) noexcept {
    return a + t * (b - a);
}

template<arithmetic T>
EuclidForceinline constexpr T saturate(const T val) noexcept {
    return val > 1 ? 1 : val < 0 ? 0 : val;
}

template<arithmetic T, std::size_t S>
EuclidForceinline constexpr Vector<T, S> clamp(const Vector<T, S> vec,
                                               const Vector<T, S> min,
                                               const Vector<T, S> max) {
    Vector<T, S> clampVec;
    if (__builtin_is_constant_evaluated()) {
        for (std::size_t i = 0; i < S; ++i) {
            clampVec.vector.data[i] = clamp(vec.vector.data[i], min.vector.data[i], max.vector.data[i]);
        }
    } else {
        if constexpr (same_type<T, float>) {
            _mm256_store_ps((float*)__builtin_addressof(clampVec),
            _mm256_max_ps(_mm256_min_ps(*(__m256*)__builtin_addressof(vec),
            *(__m256*)__builtin_addressof(max)), *(__m256*)__builtin_addressof(min)));
        } else {
            _mm256_store_si256((__m256i*)__builtin_addressof(clampVec),
            _mm256_max_epi32(_mm256_min_epi32(*(__m256i*)__builtin_addressof(vec),
            *(__m256i*)__builtin_addressof(max)), *(__m256i*)__builtin_addressof(min)));
        }
    }
    return clampVec;
}

template<std::size_t S>
EuclidForceinline constexpr Vector<float, S> lerp(const Vector<float, S> a, 
                                                  const Vector<float, S> b, 
                                                  const Vector<float, S> t) noexcept {
    Vector<float, S> lerpVec;
    if (__builtin_is_constant_evaluated()) {
        for (std::size_t i = 0; i < S; ++i) {
            lerpVec.vector.data[i] = lerp(a.vector.data[i], b.vector.data[i], t.vector.data[i]);
        }
    } else {
        _mm256_store_ps((float*)&lerpVec, _mm256_add_ps(*(__m256*)__builtin_addressof(a),
        _mm256_mul_ps(*(__m256*)__builtin_addressof(t),
        _mm256_sub_ps(*(__m256*)__builtin_addressof(b), *(__m256*)__builtin_addressof(a)))));
    }
    return lerpVec;
}

template<arithmetic T, std::size_t S>
EuclidForceinline constexpr Vector<T, S> saturate(const Vector<T, S> vec) noexcept {
    Vector<T, S> saturateVec;
    if (__builtin_is_constant_evaluated()) {
        for (std::size_t i = 0; i < S; ++i) {
            saturateVec.vector.data[i] = saturate(vec.vector.data[i]);
        }
    } else {
        if constexpr (same_type<T, float>) {
            _mm256_store_ps((float*)__builtin_addressof(saturateVec), 
            _mm256_max_ps(_mm256_min_ps(*(__m256*)__builtin_addressof(vec),
            _mm256_set1_ps(1.f)),_mm256_setzero_ps()));
        } else {
            _mm256_store_si256((__m256i*)__builtin_addressof(saturateVec),
            _mm256_max_epi32(_mm256_min_epi32(*(__m256i*)__builtin_addressof(vec),
            _mm256_set1_epi32(1.f)), _mm256_setzero_si256()));
        }
    }
    return saturateVec;
}

template<typename T>
concept euclid_component = is_array<T>::value  || is_point<T>::value ||
                           is_vector<T>::value || is_mat2<T>::value  ||
                           is_mat4<T>::value;

}