#pragma once

namespace euclid {

template<arithmetic Type, std::size_t Size>
struct EUCLID_ALIGNAS(Type, Size) Array {
    using value_type = Type;

    constexpr std::size_t size() const noexcept {
        return Size;
    }

    constexpr Array<float, Size> castTofloat() const noexcept requires(same_type<Type, int>) {
        if (EUCLID_WORKAROUND(Type)) {
            Array<float, Size> floatAry{}; // constexpr return value must be initialized;
            for (std::size_t i = 0; i < Size; ++i) {
                floatAry.data[i] = static_cast<float>(data[i]);
            }
            return floatAry;
        }
        Array<float, Size> floatAry;
        if constexpr (Size <= 4) {
            _mm_store_ps(floatAry.data, _mm_cvtepi32_ps(*(__m128i*)data));
        } else {
            _mm256_store_ps(floatAry.data, _mm256_cvtepi32_ps(*(__m256i*)data));
        }
        return floatAry;
    }

    constexpr Array<int, Size> castToint() const noexcept requires(same_type<Type, float>) {
        if (EUCLID_WORKAROUND(Type)) {
            Array<int, Size> intAry{}; // constexpr return value must be initialized;
            for (std::size_t i = 0; i < Size; ++i) {
                intAry.data[i] = static_cast<int>(data[i]);
            }
            return intAry;
        }
        Array<int, Size> intAry;
        if constexpr (Size <= 4) {
            _mm_store_si128((__m128i*)intAry.data, _mm_cvtps_epi32(*(__m128*)data));
        } else {
            _mm256_store_si256((__m256i*)intAry.data, _mm256_cvtps_epi32(*(__m256*)data));
        }
        return intAry;
    }

    constexpr void negative() noexcept {
        if (EUCLID_WORKAROUND(Type)) {
            for (std::size_t i = 0; i < Size; ++i) {
                data[i] = -data[i];
            }
            return;
        }
        if constexpr (same_type<Type, float>) {
            if constexpr (Size <= 4) {
                _mm_store_ps(data, _mm_sub_ps(_mm_setzero_ps(), *(__m128*)data));
            } else {
                _mm256_store_ps(data, _mm256_sub_ps(_mm256_setzero_ps(), *(__m256*)data));
            }
        } else {
            if constexpr (Size <= 4) {
                _mm_store_si128((__m128i*)data, _mm_sub_epi32(_mm_setzero_si128(), *(__m128i*)data));
            } else {
                _mm256_store_si256((__m256i*)data, _mm256_sub_epi32(_mm256_setzero_si256(), *(__m256i*)data));
            }
        }
    }

    constexpr Array& operator+=(const Array otherAry) noexcept {
        if (EUCLID_WORKAROUND(Type)) {
            for (std::size_t i = 0; i < Size; ++i) {
                data[i] += otherAry.data[i];
            }
            return *this;
        }
        if constexpr (same_type<Type, float>) {
            if constexpr (Size <= 4) {
                _mm_store_ps(data, _mm_add_ps(*(__m128*)data, *(__m128*)&otherAry));
            } else {
                _mm256_store_ps(data, _mm256_add_ps(*(__m256*)data, *(__m256*)&otherAry));
            }
        } else {
            if constexpr (Size <= 4) {
                _mm_store_si128((__m128i*)data, _mm_add_epi32(*(__m128i*)data, *(__m128i*)&otherAry));
            } else {
                _mm256_store_si256((__m256i*)data, _mm256_add_epi32(*(__m256i*)data, *(__m256i*)&otherAry));
            }
        }
        return *this;
    }

    constexpr Array& operator-=(const Array otherAry) noexcept {
        if (EUCLID_WORKAROUND(Type)) {
            for (std::size_t i = 0; i < Size; ++i) {
                data[i] -= otherAry.data[i];
            }
            return *this;
        }
        if constexpr (same_type<Type, float>) {
            if constexpr (Size <= 4) {
                _mm_store_ps(data, _mm_sub_ps(*(__m128*)data, *(__m128*)&otherAry));
            } else {
                _mm256_store_ps(data, _mm256_sub_ps(*(__m256*)data, *(__m256*)&otherAry));
            }
        } else {
            if constexpr (Size <= 4) {
                _mm_store_si128((__m128i*)data, _mm_sub_epi32(*(__m128i*)data, *(__m128i*)&otherAry));
            } else {
                _mm256_store_si256((__m256i*)data, _mm256_sub_epi32(*(__m256i*)data, *(__m256i*)&otherAry));
            }
        }
        return *this;
    }

    constexpr Array& operator*=(const Array otherAry) noexcept {
        if (EUCLID_WORKAROUND(Type)) {
            for (std::size_t i = 0; i < Size; ++i) {
                data[i] *= otherAry.data[i];
            }
            return *this;
        }
        if constexpr (same_type<Type, float>) {
            if constexpr (Size <= 4) {
                _mm_store_ps(data, _mm_mul_ps(*(__m128*)data, *(__m128*)&otherAry));
            } else {
                _mm256_store_ps(data, _mm256_mul_ps(*(__m256*)data, *(__m256*)&otherAry));
            }
        } else {
            if constexpr (Size <= 4) {
                _mm_store_si128((__m128i*)data, _mm_mullo_epi32(*(__m128i*)data, *(__m128i*)&otherAry));
            } else {
                _mm256_store_si256((__m256i*)data, _mm256_mullo_epi32(*(__m256i*)data, *(__m256i*)&otherAry));
            }
        }
        return *this;
    }

    constexpr Array& operator/=(const Array otherAry) noexcept {
        if (EUCLID_WORKAROUND(Type)) {
            for (std::size_t i = 0; i < Size; ++i) {
                data[i] /= otherAry.data[i];
            }
            return *this;
        }
        if constexpr (same_type<Type, float>) {
            if constexpr (Size <= 4) {
                _mm_store_ps(data, _mm_div_ps(*(__m128*)data, *(__m128*)&otherAry));
            } else {
                _mm256_store_ps(data, _mm256_div_ps(*(__m256*)data, *(__m256*)&otherAry));
            }
        } else {
            if constexpr (Size <= 4) {
                _mm_store_si128((__m128i*)data, _mm_div_epi32(*(__m128i*)data, *(__m128i*)&otherAry));
            } else {
                _mm256_store_si256((__m128i*)data, _mm256_div_epi32(*(__m256i*)data, *(__m256i*)&otherAry));
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

    constexpr Array operator/(const Array otherAry) const noexcept {
        Array temp = *this;
        return temp /= otherAry;
    }

    template<arithmetic Mul> requires acceptable_loss<Type, Mul>
    constexpr Array& operator*=(const Mul mul) noexcept {
        if (EUCLID_WORKAROUND(Type)) {
            for (std::size_t i = 0; i < Size; ++i) {
                data[i] *= static_cast<Type>(mul);
            }
            return *this;
        }
        if constexpr (same_type<Type, float>) {
            if constexpr (Size <= 4) {
                _mm_store_ps(data, _mm_mul_ps(*(__m128*)data, _mm_set1_ps(static_cast<float>(mul))));
            } else {
                _mm256_store_ps(data, _mm256_mul_ps(*(__m256*)data, _mm256_set1_ps(static_cast<float>(mul))));
            }
        } else {
            if constexpr (Size <= 4) {
                _mm_store_si128((__m128i*)data, _mm_mullo_epi32(*(__m128i*)data, _mm_set1_epi32(static_cast<int>(mul))));
            } else {
                _mm256_store_si256((__m256i*)data, _mm256_mullo_epi32(*(__m256i*)data, _mm256_set1_epi32(static_cast<int>(mul))));
            }
        }
        return *this;
    }

    template<arithmetic Div> requires acceptable_loss<Type, Div>
    constexpr Array& operator/=(const Div div) noexcept {
        if (EUCLID_WORKAROUND(Type)) {
            for (std::size_t i = 0; i < Size; ++i) {
                data[i] /= static_cast<Type>(div);
            }
            return *this;
        }
        if constexpr (same_type<Type, float>) {
            if constexpr (Size <= 4) {
                _mm_store_ps(data, _mm_div_ps(*(__m128*)data, _mm_set1_ps(static_cast<float>(div))));
            } else {
                _mm256_store_ps(data, _mm256_div_ps(*(__m256*)data, _mm256_set1_ps(static_cast<float>(div))));
            }
        } else {
            if constexpr (Size <= 4) {
                _mm_store_si128((__m128i*)data, _mm_div_epi32(*(__m128i*)data, _mm_set1_epi32(static_cast<int>(div))));
            } else {
                _mm256_store_si256((__m256i*)data, _mm256_div_epi32(*(__m256i*)data, _mm256_set1_epi32(static_cast<int>(div))));
            }
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
EUCLID_FORCEINLINE constexpr Array<T, S> operator*(const Mul mul, const Array<T, S> array) noexcept {
    return array * mul;
}

template<euclid_type First, euclid_type... Rest> requires same_type<First, Rest...>
Array(First, Rest...)->Array<First, sizeof...(Rest) + 1>;

template<typename T>
struct is_array {
    static constexpr bool value = false;
};

template<euclid_type T, std::size_t S>
struct is_array<Array<T, S>> {
    static constexpr bool value = true;
};

}