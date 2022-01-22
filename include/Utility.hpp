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

template<euclid_type T, std::size_t S>
EuclidForceinline constexpr Vector<T, S> clamp(const Vector<T, S> vec, const Vector<T, S> min, const Vector<T, S> max) noexcept {
    if (__builtin_is_constant_evaluated()) {
        Vector<T, S> clampVec{};
        for (std::size_t i = 0; i < S; ++i) {
            clampVec.vector.data[i] = clamp(vec.vector.data[i], min.vector.data[i], max.vector.data[i]);
        }
        return clampVec;
    }
    Vector<T, S> clampVec;
    if constexpr (same_type<T, float>) {
        _mm256_store_ps((float*)&clampVec,
        _mm256_max_ps(_mm256_min_ps(*(__m256*)&vec, *(__m256*)&max), *(__m256*)&min));
    } else {
        _mm256_store_si256((__m256i*)&clampVec,
        _mm256_max_epi32(_mm256_min_epi32(*(__m256i*)&vec, *(__m256i*)&max), *(__m256i*)&min));
    }
    return clampVec;
}

template<euclid_type T, std::size_t S>
EuclidForceinline constexpr Vector<T, S> clamp(const Vector<T, S> vec, const T min, const T max) noexcept {
    if (__builtin_is_constant_evaluated()) {
        Vector<T, S> clampVec{};
        for (std::size_t i = 0; i < S; ++i) {
            clampVec.vector.data[i] = clamp(vec.vector.data[i], min, max);
        }
        return clampVec;
    }
    Vector<T, S> clampVec;
    if constexpr (same_type<T, float>) {
        _mm256_store_ps((float*)&clampVec,
        _mm256_max_ps(_mm256_min_ps(*(__m256*)&vec,
        _mm256_set1_ps(max)), _mm256_set1_ps(min)));
    } else {
        _mm256_store_si256((__m256i*)&clampVec,
        _mm256_max_epi32(_mm256_min_epi32(*(__m256i*)&vec,
        _mm256_set1_epi32(max)), _mm256_set1_epi32(min)));
    }
    return clampVec;
}

template<std::size_t S>
EuclidForceinline constexpr Vector<float, S> lerp(const Vector<float, S> a, const Vector<float, S> b, const Vector<float, S> t) noexcept {
    if (__builtin_is_constant_evaluated()) {
        Vector<float, S> lerpVec{};
        for (std::size_t i = 0; i < S; ++i) {
            lerpVec.vector.data[i] = lerp(a.vector.data[i], b.vector.data[i], t.vector.data[i]);
        }
        return lerpVec;
    }
    Vector<float, S> lerpVec;
    _mm256_store_ps((float*)&lerpVec,
    _mm256_fmadd_ps(*(__m256*)&t,
    _mm256_sub_ps(*(__m256*)&b, *(__m256*)&a), *(__m256*)&a));
    return lerpVec;
}

template<std::size_t S>
EuclidForceinline constexpr Vector<float, S> lerp(const Vector<float, S> a, const Vector<float, S> b, const float t) noexcept {
    if (__builtin_is_constant_evaluated()) {
        Vector<float, S> lerpVec{};
        for (std::size_t i = 0; i < S; ++i) {
            lerpVec.vector.data[i] = lerp(a.vector.data[i], b.vector.data[i], t);
        }
        return lerpVec;
    }
    Vector<float, S> lerpVec;
    _mm256_store_ps((float*)&lerpVec,
    _mm256_fmadd_ps(_mm256_set1_ps(t),
    _mm256_sub_ps(*(__m256*)&b, *(__m256*)&a), *(__m256*)&a));
    return lerpVec;
}

template<euclid_type T, std::size_t S>
EuclidForceinline constexpr Vector<T, S> saturate(const Vector<T, S> vec) noexcept {
    if (__builtin_is_constant_evaluated()) {
        Vector<T, S> saturateVec{};
        for (std::size_t i = 0; i < S; ++i) {
            saturateVec.vector.data[i] = saturate(vec.vector.data[i]);
        }
        return saturateVec;
    }
    Vector<T, S> saturateVec;
    if constexpr (same_type<T, float>) {
        _mm256_store_ps((float*)&saturateVec,
        _mm256_max_ps(_mm256_min_ps(*(__m256*)&vec,
        _mm256_set1_ps(1.f)),_mm256_setzero_ps()));
    } else {
        _mm256_store_si256((__m256i*)&saturateVec,
        _mm256_max_epi32(_mm256_min_epi32(*(__m256i*)&vec,
        _mm256_set1_epi32(1.f)), _mm256_setzero_si256()));
    }
    return saturateVec;
}

template<typename T>
concept euclid_component = is_array<T>::value  || is_point<T>::value ||
                           is_vector<T>::value || is_mat2<T>::value  ||
                           is_mat3<T>::value   || is_mat4<T>::value;

}