#pragma once

namespace euclid {

template<euclid_type Type, std::size_t Size>
struct alignas(32) Array {
    using value_type = Type;

    constexpr std::size_t size() const noexcept {
        return Size;
    }

    constexpr Array<float, Size> castTofloat() const noexcept requires(same_type<Type, int>) {
        Array<float, Size> castAry;
        if (__builtin_is_constant_evaluated()) {
            for (std::size_t i = 0; i < Size; ++i) {
                castAry.data[i] = static_cast<float>(data[i]);
            }
        } else {
            _mm256_store_ps(castAry.data, _mm256_cvtepi32_ps(*(__m256i*)this));
        }
        return castAry;
    }

    constexpr void negative() noexcept {
        if (__builtin_is_constant_evaluated()) {
            for (std::size_t i = 0; i < Size; ++i) {
                data[i] = -data[i];
            }
        } else {
            if constexpr (same_type<Type, float>) {
                _mm256_store_ps(data, _mm256_sub_ps(_mm256_setzero_ps(), *(__m256*)this));
            } else {
                _mm256_store_si256((__m256i*)data, _mm256_sub_epi32(_mm256_setzero_si256(), *(__m256i*)this));
            }
        }
    }

    constexpr Array& operator+=(const Array otherAry) noexcept {
        if (__builtin_is_constant_evaluated()) {
            for (std::size_t i = 0; i < Size; ++i) {
                data[i] += otherAry.data[0];
            }
        } else {
            if constexpr (same_type<Type, float>) {
                _mm256_store_ps(data, _mm256_add_ps(*(__m256*)this, *(__m256*)__builtin_addressof(otherAry)));
            } else {
                _mm256_store_si256((__m256i*)data, _mm256_add_epi32(*(__m256i*)this, *(__m256i*)__builtin_addressof(otherAry)));
            }
        }
        return *this;
    }

    constexpr Array& operator-=(const Array otherAry) noexcept {
        if (__builtin_is_constant_evaluated()) {
            for (std::size_t i = 0; i < Size; ++i) {
                data[i] -= otherAry.data[i];
            }
        } else {
            if constexpr (same_type<Type, float>) {
                _mm256_store_ps(data, _mm256_sub_ps(*(__m256*)this, *(__m256*)__builtin_addressof(otherAry)));
            } else {
                _mm256_store_si256((__m256i*)data, _mm256_sub_epi32(*(__m256i*)this, *(__m256i*)__builtin_addressof(otherAry)));
            }
        }
        return *this;
    }

    constexpr Array& operator*=(const Array otherAry) noexcept {
        if (__builtin_is_constant_evaluated()) {
            for (std::size_t i = 0; i < Size; ++i) {
                data[i] *= otherAry.data[i];
            }
        } else {
            if constexpr (same_type<Type, float>) {
                _mm256_store_ps(data, _mm256_mul_ps(*(__m256*)this, *(__m256*)__builtin_addressof(otherAry)));
            } else {
                _mm256_store_si256((__m256i*)data, _mm256_mullo_epi32(*(__m256i*)this, *(__m256i*)__builtin_addressof(otherAry)));
            }
        }
        return *this;
    }

    constexpr Array operator+(const Array otherAry) const noexcept {
        Array temp = *this;
        return temp += otherAry;
    }

    constexpr Array operator-(const Array otherAry) const noexcept {
        Array temp = *this;
        return temp -= otherAry;
    }

    constexpr Array operator*(const Array otherAry) const noexcept {
        Array temp = *this;
        return temp *= otherAry;
    }

    template<arithmetic Mul> requires acceptable_loss<Type, Mul>
    constexpr Array& operator*=(const Mul mul) noexcept {
        if (__builtin_is_constant_evaluated()) {
            for (std::size_t i = 0; i < Size; ++i) {
                data[i] *= static_cast<value_type>(mul);
            }
        } else {
            if constexpr (same_type<Type, float>) {
                _mm256_store_ps(data, _mm256_mul_ps(*(__m256*)this, _mm256_set1_ps(static_cast<float>(mul))));
            } else {
                _mm256_store_si256((__m256i*)data, _mm256_mullo_epi32(*(__m256i*)this, ::_mm256_set1_epi32(static_cast<int>(mul))));
            }
        }
        return *this;
    }

    template<arithmetic Div> requires acceptable_loss<Type, Div>
    constexpr Array& operator/=(const Div div) noexcept {
        if (__builtin_is_constant_evaluated()) {
            for (std::size_t i = 0; i < Size; ++i) {
                data[i] /= static_cast<value_type>(div);
            }
        } else {
            _mm256_store_ps((float*)data, _mm256_div_ps(*(__m256*)this, _mm256_set1_ps(static_cast<float>(div))));
        }
        return *this;
    }

    template<arithmetic Mul> requires acceptable_loss<Type, Mul>
    constexpr Array operator*(const Mul mul) const noexcept {
        Array temp = *this;
        return temp *= mul;
    }

    template<arithmetic Div> requires acceptable_loss<Type, Div>
    constexpr Array operator/(const Div div) const noexcept {
        Array temp = *this;
        return temp /= div;
    }

    constexpr Array operator-() const noexcept {
        Array temp = *this;
        temp.negative();
        return temp;
    }

    value_type data[Size];
};

template<arithmetic Mul, euclid_type T, std::size_t S> requires acceptable_loss<T, Mul>
EuclidForceinline constexpr Array<T, S> operator*(const Mul mul, const Array<T, S> array) noexcept {
    return array * mul;
}

template<euclid_type First, euclid_type... Rest> requires same_type<First, Rest...>
Array(First, Rest...)->Array<First, sizeof...(Rest) + 1>;

using float3 = Array<float, 3>;
using float4 = Array<float, 4>;

template<typename T>
struct is_array {
    static constexpr bool value = false;
};

template<euclid_type T, std::size_t S>
struct is_array<Array<T, S>> {
    static constexpr bool value = true;
};

}