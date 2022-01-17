#pragma once

namespace euclid {
    
template<euclid_type Type>
struct alignas(64) Mat4 {
public:
    using value_type = Type;

    constexpr Mat4<float> castTofloat() const noexcept requires(same_type<Type, int>) {
        return { first.castTofloat(), second.castTofloat() };
    }

    constexpr Vector<Type, 4> getFirstCol() const noexcept {
        return { first.data[0], first.data[4], second.data[0], second.data[4] };
    }

    constexpr Vector<Type, 4> getSecondCol() const noexcept {
        return { first.data[1], first.data[5], second.data[1], second.data[5] };
    }

    constexpr Vector<Type, 4> getThirdCol() const noexcept {
        return { first.data[2], first.data[6], second.data[2], second.data[6] };
    }

    constexpr Vector<Type, 4> getFourthCol() const noexcept {
        return { first.data[3], first.data[7], second.data[3], second.data[7] };
    }

    static constexpr Mat4 identity() noexcept {
        if (__builtin_is_constant_evaluated()) {
            return {
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            };
        } else {
            Mat4 identityMat;
            if constexpr (same_type<Type, float>) {
                _mm256_store_ps((float*)&identityMat.first,  _mm256_set_ps(0, 0, 1, 0, 0, 0, 0, 1));
                _mm256_store_ps((float*)&identityMat.second, _mm256_set_ps(1, 0, 0, 0, 0, 1, 0, 0));
            } else {
                _mm256_store_si256((__m256i*)&identityMat.first,  _mm256_set_epi32(0, 0, 1, 0, 0, 0, 0, 1));
                _mm256_store_si256((__m256i*)&identityMat.second, _mm256_set_epi32(1, 0, 0, 0, 0, 1, 0, 0));
            }
            return identityMat;
        }
    }

    constexpr void negative() noexcept {
        first.negative();
        second.negative();
    }

    constexpr std::size_t size() const noexcept {
        return 16;
    }

    constexpr void transposed() noexcept {
        if (__builtin_is_constant_evaluated()) {

        } else {

        }
    }

    constexpr Mat4 transpose() const noexcept {
        Mat4 transposedMat = *this;
        transposedMat.transposed();
        return transposedMat;
    }

    constexpr void operator+=(const Mat4 otherMat) noexcept {
        first += otherMat.first;
        second += otherMat.second;
    }

    constexpr void operator-=(const Mat4 otherMat) noexcept {
        first -= otherMat.first;
        second -= otherMat.second;
    }

    template<arithmetic Mul> requires acceptable_loss<Type, Mul>
    constexpr void operator*=(const Mul mul) noexcept {
        first *= mul;
        second *= mul;
    }

    constexpr Mat4 operator+(const Mat4 otherMat) const noexcept {
        return { first + otherMat.first, second + otherMat.second };
    }

    constexpr Mat4 operator-(const Mat4 otherMat) const noexcept {
        return { first - otherMat.first, second - otherMat.second };
    }

    constexpr Mat4 operator*(const Mat4 otherMat) const noexcept {
        if (__builtin_is_constant_evaluated()) {

        } else {
            if constexpr (same_type<Type, float>) {

            } else {

            }
        }
    }

    constexpr Vector<Type, 4> operator*(const Vector<Type, 4> vec) const noexcept {
        if (__builtin_is_constant_evaluated()) {
            Vector<Type, 4> result{};
            // Todo
            return result;
        } else {
            Vector<Type, 4> result;
            if constexpr (same_type<Type, float>) {
                const auto tempData = _mm256_permute2f128_ps(*(__m256*)&vec, *(__m256*)&vec, 0);
                const auto mulData1 = _mm256_mul_ps(*(__m256*)&first,  tempData);
                const auto mulData2 = _mm256_mul_ps(*(__m256*)&second, tempData);
                const auto addData1 = _mm256_hadd_ps(mulData1, mulData2);
                const auto addData2 = _mm256_hadd_ps(addData1, addData1);
                _mm256_store_ps((float*)&result, _mm256_permutevar8x32_ps(addData2, _mm256_set_epi32(0,0,0,0,5,1,4,0)));
            } else {
                const auto tempData = _mm256_permute2x128_si256(*(__m256i*)&vec, *(__m256i*)&vec, 0);
                const auto mulData1 = _mm256_mullo_epi32(*(__m256i*)&first,  tempData);
                const auto mulData2 = _mm256_mullo_epi32(*(__m256i*)&second, tempData);
                const auto addData1 = _mm256_hadd_epi32(mulData1, mulData2);
                const auto addData2 = _mm256_hadd_epi32(addData1, addData1);
                _mm256_store_si256((__m256i*)&result, _mm256_permutevar8x32_epi32(addData2, _mm256_set_epi32(0,0,0,0,5,1,4,0)));
            }
            return result;
        }
    }

    template<arithmetic Mul> requires acceptable_loss<Type, Mul>
    constexpr Mat4 operator*(const Mul mul) const noexcept {
        return { first * mul, second * mul };
    }

    union {
        struct {
            Array<Type, 8> first;
            Array<Type, 8> second;
        };
        value_type mat[4][4];
    };
};

template<arithmetic Mul, euclid_type T> requires acceptable_loss<T, Mul>
EuclidForceinline constexpr Mat4<T> operator*(const Mul mul, const Mat4<T> mat) noexcept {
    return mat * mul;
}

template<euclid_type First, euclid_type... Rest> requires same_type<First, Rest...>
Mat4(First, Rest...)->Mat4<First>;

using mat4i = Mat4<int>;
using mat4f = Mat4<float>;

template<typename T>
struct is_mat4 {
    static constexpr bool value = false;
};

template<euclid_type T>
struct is_mat4<Mat4<T>> {
    static constexpr bool value = true;
};

}