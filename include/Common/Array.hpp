#pragma once

namespace euclid {

template<arithmetic Type, std::size_t Size>
struct alignas(32) Array {
	using value_type = Type;
    using reference  = Type&;

    constexpr std::size_t size() const noexcept {
        return Size;
    }

    constexpr Array<float, Size> castTofloat() const noexcept requires(same_type<Type, int>) {
        Array<float, Size> castArray;
        if (__builtin_is_constant_evaluated()) {
            for (std::size_t i = 0; i < Size; ++i) {
                castArray.data[i] = static_cast<float>(data[i]);
            }
        } else {
            ::_mm256_store_ps(castArray.data, _mm256_cvtepi32_ps(*(__m256i*)this));
        }
        return castArray;
    }

    constexpr void negative() noexcept {
        if (__builtin_is_constant_evaluated()) {
            util::transformToArray(data, [](reference val) noexcept { val = -val; });
        } else {
            if constexpr (same_type<Type, float>) {
                ::_mm256_store_ps(data, _mm256_sub_ps(_mm256_setzero_ps(), *(__m256*)this));
            } else {
                ::_mm256_store_si256((__m256i*)data, _mm256_sub_epi32(_mm256_setzero_si256(), *(__m256i*)this));
            }
        }
    }

    constexpr void operator+=(const Array otherAry) noexcept {
        if (__builtin_is_constant_evaluated()) {
            util::transformArrayFromOther(data, otherAry.data, [](reference dst, value_type src) noexcept { dst += src; });
        } else {
            if constexpr (same_type<Type, float>) {
                ::_mm256_store_ps(data, _mm256_add_ps(*(__m256*)this, *(__m256*)__builtin_addressof(otherAry)));
            } else {
                ::_mm256_store_si256((__m256i*)data, _mm256_add_epi32(*(__m256i*)this, *(__m256i*)__builtin_addressof(otherAry)));
            }
        }
    }

    constexpr void operator-=(const Array otherAry) noexcept {
        if (__builtin_is_constant_evaluated()) {
            util::transformArrayFromOther(data, otherAry.data, [](reference dst, value_type src) noexcept { dst -= src; });
        } else {
            if constexpr (same_type<Type, float>) {
                ::_mm256_store_ps(data, _mm256_sub_ps(*(__m256*)this, *(__m256*)__builtin_addressof(otherAry)));
            } else {
                ::_mm256_store_si256((__m256i*)data, _mm256_sub_epi32(*(__m256i*)this, *(__m256i*)__builtin_addressof(otherAry)));
            }
        }
    }

    constexpr void operator*=(const Array otherAry) noexcept {
        if (__builtin_is_constant_evaluated()) {
            util::transformArrayFromOther(data, otherAry.data, [](reference dst, value_type src) noexcept { dst *= src; });
        } else {
            if constexpr (same_type<Type, float>) {
                ::_mm256_store_ps(data, _mm256_mul_ps(*(__m256*)this, *(__m256*)__builtin_addressof(otherAry)));
            } else {
                ::_mm256_store_si256((__m256i*)data, _mm256_mullo_epi32(*(__m256i*)this, *(__m256i*)__builtin_addressof(otherAry)));
            }
        }
    }

    constexpr Array operator+(const Array otherAry) const noexcept {
        Array temp = *this;
        temp += otherAry;
        return temp;
    }

    constexpr Array operator-(const Array otherAry) const noexcept {
        Array temp = *this;
        temp -= otherAry;
        return temp;
    }

    constexpr Array operator*(const Array otherAry) const noexcept {
        Array temp = *this;
        temp *= otherAry;
        return temp;
    }

    template<arithmetic Mul> requires acceptable_loss<Type, Mul>
    constexpr void operator*=(const Mul mul) noexcept {
        if (__builtin_is_constant_evaluated()) {
            util::transformToArray(data, [=](reference val) noexcept { val *= mul; });
        } else {
            if constexpr (same_type<Type, float>) {
                ::_mm256_store_ps(data, _mm256_mul_ps(*(__m256*)this, _mm256_set1_ps(static_cast<float>(mul))));
            } else {
                ::_mm256_store_si256((__m256i*)data, _mm256_mullo_epi32(*(__m256i*)this, ::_mm256_set1_epi32(static_cast<int>(mul))));
            }
        }
    }

    template<arithmetic Div> requires acceptable_loss<Type, Div>
    constexpr void operator/=(const Div div) noexcept {
        if (__builtin_is_constant_evaluated()) {
            util::transformToArray(data, [=](reference val) noexcept { val /= div; });
        } else {
            ::_mm256_store_ps((float*)data, _mm256_div_ps(*(__m256*)this, _mm256_set1_ps(static_cast<float>(div))));
        }
    }

    template<arithmetic Mul> requires acceptable_loss<Type, Mul>
    constexpr Array operator*(const Mul mul) const noexcept {
        Array temp = *this;
        temp *= mul;
        return temp;
    }

    template<arithmetic Div> requires acceptable_loss<Type, Div>
    constexpr Array operator/(const Div div) const noexcept {
        Array temp = *this;
        temp /= div;
        return temp;
    }

    constexpr Array operator-() const noexcept {
        Array temp = *this;
        temp.negative();
        return temp;
    }

	value_type data[Size];
};

template<arithmetic Mul, arithmetic T, std::size_t S> requires acceptable_loss<T, Mul>
EuclidForceinline constexpr Array<T, S> operator*(const Mul mul, const Array<T, S> array) noexcept {
    return array * mul;
}

template<arithmetic First, arithmetic... Rest> requires same_type<First, Rest...>
Array(First, Rest...)->Array<First, sizeof...(Rest) + 1>;

using float3 = Array<float, 3>;
using float4 = Array<float, 4>;

}