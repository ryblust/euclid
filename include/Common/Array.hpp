#pragma once

namespace euclid {

template<arithmetic Type, std::size_t Size>
struct alignas(32) Array {
	using value_type = Type;

    constexpr std::size_t size() const noexcept {
        return Size;
    }

    Array<float, Size> cast() const noexcept requires(same_type<Type, int>) {
        Array<float, Size> castArray;
        ::_mm256_store_ps(castArray.data, _mm256_cvtepi32_ps(*(__m256i*)this));
        return castArray;
    }

    Array& negative() noexcept {
        if constexpr (same_type<Type, float>) {
            ::_mm256_store_ps(data, _mm256_sub_ps(_mm256_setzero_ps(), *(__m256*)data));
        } else {
            ::_mm256_store_si256((__m256i*)data, _mm256_sub_epi32(_mm256_setzero_si256(), *(__m256i*)data));
        }
        return *this;
    }

    Array& operator+=(const Array otherAry) noexcept {
        if constexpr (same_type<Type, float>) {
            ::_mm256_store_ps(data, _mm256_add_ps(*(__m256*)this, *(__m256*)__builtin_addressof(otherAry)));
        } else {
            ::_mm256_store_si256((__m256i*)data, _mm256_add_epi32(*(__m256i*)this, *(__m256i*)__builtin_addressof(otherAry)));
        }
        return *this;
    }

    Array& operator-=(const Array otherAry) noexcept {
        if constexpr (same_type<Type, float>) {
            ::_mm256_store_ps(data, _mm256_sub_ps(*(__m256*)this, *(__m256*)__builtin_addressof(otherAry)));
        } else {
            ::_mm256_store_si256((__m256i*)data, _mm256_sub_epi32(*(__m256i*)this, *(__m256i*)__builtin_addressof(otherAry)));
        }
        return *this;
    }

    Array& operator*=(const Array otherAry) noexcept {
        if constexpr (same_type<Type, float>) {
            ::_mm256_store_ps(data, _mm256_mul_ps(*(__m256*)this, *(__m256*)__builtin_addressof(otherAry)));
        } else {
            ::_mm256_store_si256((__m256i*)data, _mm256_mullo_epi32(*(__m256i*)this, *(__m256i*)__builtin_addressof(otherAry)));
        }
        return *this;
    }

    Array operator+(const Array otherAry) const noexcept {
        Array temp = *this;
        return temp += otherAry;
    }

    Array operator-(const Array otherAry) const noexcept {
        Array temp = *this;
        return temp -= otherAry;
    }

    Array operator*(const Array otherAry) const noexcept {
        Array temp = *this;
        return temp *= otherAry;
    }

    template<arithmetic Mul> requires acceptable_loss<Type, Mul>
    Array& operator*=(const Mul mul) noexcept {
        if constexpr (same_type<Type, float>) {
            ::_mm256_store_ps(data, _mm256_mul_ps(*(__m256*)this, _mm256_set1_ps(static_cast<float>(mul))));
        } else {
            ::_mm256_store_si256((__m256i*)data, _mm256_mullo_epi32(*(__m256i*)this, ::_mm256_set1_epi32(mul)));
        }
        return *this;
    }

    template<arithmetic Div> requires acceptable_loss<Type, Div>
    Array& operator/=(const Div div) noexcept {
        ::_mm256_store_ps((float*)data, _mm256_div_ps(*(__m256*)this, _mm256_set1_ps(static_cast<float>(div))));
        return *this;
    }

    template<arithmetic Mul> requires acceptable_loss<Type, Mul>
    Array operator*(const Mul mul) const noexcept {
        Array temp = *this;
        return temp *= mul;
    }

    template<arithmetic Div> requires acceptable_loss<Type, Div>
    Array operator/(const Div div) const noexcept {
        Array temp = *this;
        return temp /= div;
    }

    Array operator-() const noexcept {
        Array temp = *this;
        return temp.negative();
    }

	value_type data[Size];
};

template<arithmetic Mul, arithmetic T, std::size_t S> requires acceptable_loss<T, Mul>
EUCLID_FORCEINLINE auto operator*(const Mul mul, const Array<T, S> array) noexcept {
    return array * mul;
}

template<arithmetic First, arithmetic... Rest> requires same_type<First, Rest...>
Array(First, Rest...)->Array<First, sizeof...(Rest) + 1>;

using float3 = Array<float, 3>;
using float4 = Array<float, 4>;

}