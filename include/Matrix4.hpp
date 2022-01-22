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
        }
        Mat4 identityMat;
        if constexpr (same_type<Type, float>) {
            _mm256_store_ps((float*)&identityMat.first,
            _mm256_set_ps(0,0,1,0,0,0,0,1));
            _mm256_store_ps((float*)&identityMat.second,
            _mm256_set_ps(1,0,0,0,0,1,0,0));
        } else {
            _mm256_store_si256((__m256i*)&identityMat.first,
            _mm256_set_epi32(0,0,1,0,0,0,0,1));
            _mm256_store_si256((__m256i*)&identityMat.second,
            _mm256_set_epi32(1,0,0,0,0,1,0,0));
        }
        return identityMat;
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
            util::swap(first.data[1],   first.data[4]);
            util::swap(first.data[2],  second.data[0]);
            util::swap(first.data[3],  second.data[4]);
            util::swap(first.data[6],  second.data[1]);
            util::swap(first.data[7],  second.data[5]);
            util::swap(second.data[3], second.data[6]);
            return;
        }
        const auto mask = _mm256_set_epi32(7,3,6,2,5,1,4,0);
        if constexpr (same_type<Type, float>) {
            const auto unpackFirst      = _mm256_unpacklo_ps(*(__m256*)&first, *(__m256*)&second);
            const auto unpackSecond     = _mm256_unpackhi_ps(*(__m256*)&first, *(__m256*)&second);
            const auto transposedFirst  = _mm256_permutevar8x32_ps(unpackFirst,  mask);
            const auto transposedSecond = _mm256_permutevar8x32_ps(unpackSecond, mask);
            _mm256_store_ps((float*)&first,  transposedFirst);
            _mm256_store_ps((float*)&second, transposedSecond);
        } else {
            const auto unpackFirst      = _mm256_unpacklo_epi32(*(__m256i*)&first, *(__m256i*)&second);
            const auto unpackSecond     = _mm256_unpackhi_epi32(*(__m256i*)&first, *(__m256i*)&second);
            const auto transposedFirst  = _mm256_permutevar8x32_epi32(unpackFirst,  mask);
            const auto transposedSecond = _mm256_permutevar8x32_epi32(unpackSecond, mask);
            _mm256_store_si256((__m256i*)&first,  transposedFirst);
            _mm256_store_si256((__m256i*)&second, transposedSecond);
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
            const auto [a00, a10, a20, a30] = ((*this) * this->getFirstCol()).vector.data;
            const auto [a01, a11, a21, a31] = ((*this) * this->getSecondCol()).vector.data;
            const auto [a02, a12, a22, a32] = ((*this) * this->getThirdCol()).vector.data;
            const auto [a03, a13, a23, a33] = ((*this) * this->getFourthCol()).vector.data;
            return {
                a00, a01, a02, a03,
                a10, a11, a12, a13,
                a20, a21, a22, a23,
                a30, a31, a32, a33
            };
        }
        Mat4 result;
        const auto transposedMat = otherMat.transpose();
        const auto resultMask    = _mm256_set_epi32(6, 3, 4, 1, 7, 2, 5, 0);
        if constexpr (same_type<Type, float>) {
            const auto invertedFirst     = _mm256_permute2f128_ps(*(__m256*)&first,  *(__m256*)&first,  1);
            const auto invertedSecond    = _mm256_permute2f128_ps(*(__m256*)&second, *(__m256*)&second, 1);
            const auto firstMulTrFirst   = _mm256_mul_ps(*(__m256*)&first,  *(__m256*)&transposedMat.first);
            const auto firstMulTrSecond  = _mm256_mul_ps(*(__m256*)&first,  *(__m256*)&transposedMat.second);
            const auto secondMulTrFirst  = _mm256_mul_ps(*(__m256*)&second, *(__m256*)&transposedMat.first);
            const auto secondMulTrSecond = _mm256_mul_ps(*(__m256*)&second, *(__m256*)&transposedMat.second);
            const auto invertedFirstMulTrFirst   = _mm256_mul_ps(invertedFirst,  *(__m256*)&transposedMat.first);
            const auto invertedFirstMulTrSecond  = _mm256_mul_ps(invertedFirst,  *(__m256*)&transposedMat.second);
            const auto invertedSecondMulTrFirst  = _mm256_mul_ps(invertedSecond, *(__m256*)&transposedMat.first);
            const auto invertedSecondMulTrSecond = _mm256_mul_ps(invertedSecond, *(__m256*)&transposedMat.second);
            const auto halfFirst1   = _mm256_hadd_ps(firstMulTrFirst,   invertedFirstMulTrFirst);
            const auto halfFirst2   = _mm256_hadd_ps(firstMulTrSecond,  invertedFirstMulTrSecond);
            const auto halfSecond1  = _mm256_hadd_ps(secondMulTrFirst,  invertedSecondMulTrFirst);
            const auto halfSecond2  = _mm256_hadd_ps(secondMulTrSecond, invertedSecondMulTrSecond);
            const auto tempFirst    = _mm256_hadd_ps(halfFirst1,  halfFirst2);
            const auto tempSecond   = _mm256_hadd_ps(halfSecond1, halfSecond2);
            const auto resultFirst  = _mm256_permutevar8x32_ps(tempFirst,  resultMask);
            const auto resultSecond = _mm256_permutevar8x32_ps(tempSecond, resultMask);
            _mm256_store_ps((float*)&result.first,  resultFirst);
            _mm256_store_ps((float*)&result.second, resultSecond);
        } else {
            const auto invertedFirst     = _mm256_permute2x128_si256(*(__m256i*)&first,  *(__m256i*)&first,  1);
            const auto invertedSecond    = _mm256_permute2x128_si256(*(__m256i*)&second, *(__m256i*)&second, 1);
            const auto firstMulTrFirst   = _mm256_mullo_epi32(*(__m256i*)&first,  *(__m256i*)&transposedMat.first);
            const auto firstMulTrSecond  = _mm256_mullo_epi32(*(__m256i*)&first,  *(__m256i*)&transposedMat.second);
            const auto secondMulTrFirst  = _mm256_mullo_epi32(*(__m256i*)&second, *(__m256i*)&transposedMat.first);
            const auto secondMulTrSecond = _mm256_mullo_epi32(*(__m256i*)&second, *(__m256i*)&transposedMat.second);
            const auto invertedFirstMulTrFirst   = _mm256_mullo_epi32(invertedFirst,  *(__m256i*)&transposedMat.first);
            const auto invertedFirstMulTrSecond  = _mm256_mullo_epi32(invertedFirst,  *(__m256i*)&transposedMat.second);
            const auto invertedSecondMulTrFirst  = _mm256_mullo_epi32(invertedSecond, *(__m256i*)&transposedMat.first);
            const auto invertedSecondMulTrSecond = _mm256_mullo_epi32(invertedSecond, *(__m256i*)&transposedMat.second);
            const auto halfFirst1   = _mm256_hadd_epi32(firstMulTrFirst,   invertedFirstMulTrFirst);
            const auto halfFirst2   = _mm256_hadd_epi32(firstMulTrSecond,  invertedFirstMulTrSecond);
            const auto halfSecond1  = _mm256_hadd_epi32(secondMulTrFirst,  invertedSecondMulTrFirst);
            const auto halfSecond2  = _mm256_hadd_epi32(secondMulTrSecond, invertedSecondMulTrSecond);
            const auto tempFirst    = _mm256_hadd_epi32(halfFirst1,  halfFirst2);
            const auto tempSecond   = _mm256_hadd_epi32(halfSecond1, halfSecond2);
            const auto resultFirst  = _mm256_permutevar8x32_epi32(tempFirst,  resultMask);
            const auto resultSecond = _mm256_permutevar8x32_epi32(tempSecond, resultMask);
            _mm256_store_si256((__m256i*)&result.first,  resultFirst);
            _mm256_store_si256((__m256i*)&result.second, resultSecond);
        }
        return result;
    }

    constexpr Vector<Type, 4> operator*(const Vector<Type, 4> vec) const noexcept {
        if (__builtin_is_constant_evaluated()) {
            value_type a[4]{};
            for (std::size_t i = 0; i < 2; ++i) {
                const std::size_t offset = i * 4;
                a[i]   = first.data[0 + offset]  * vec.vector.data[0] +
                         first.data[1 + offset]  * vec.vector.data[1] +
                         first.data[2 + offset]  * vec.vector.data[2] +
                         first.data[3 + offset]  * vec.vector.data[3];

                a[i+2] = second.data[0 + offset] * vec.vector.data[0] +
                         second.data[1 + offset] * vec.vector.data[1] +
                         second.data[2 + offset] * vec.vector.data[2] +
                         second.data[3 + offset] * vec.vector.data[3];
            }
            return { a[0], a[1], a[2], a[3] };
        }
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