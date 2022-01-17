#pragma once

namespace euclid {

template<euclid_type Type>
struct alignas(32) Mat2 {
public:
    using value_type = Type;

    constexpr Mat2<float> castTofloat() const noexcept requires(same_type<Type, int>) {
        return { simdMat.castTofloat() };
    }

    constexpr Vector<Type, 2> getFirstCol() const noexcept {
        if (__builtin_is_constant_evaluated()) {
            return { simdMat.data[0], simdMat.data[2] };
        } else {
            Vector<Type, 2> firstCol;
            if constexpr (same_type<Type, float>) {
                _mm256_store_ps((float*)&firstCol, _mm256_permute_ps(*(__m256*)this, 0b1000));
            } else {
                _mm256_store_si256((__m256i*)&firstCol, _mm256_shuffle_epi32(*(__m256i*)this, 0b1000));
            }
            return firstCol;
        }
    }

    constexpr Vector<Type, 2> getSecondCol() const noexcept {
        if (__builtin_is_constant_evaluated()) {
            return { simdMat.data[1], simdMat.data[3] };
        } else {
            Vector<Type, 2> secondCol;
            if constexpr (same_type<Type, float>) {
                _mm256_store_ps((float*)&secondCol, _mm256_permute_ps(*(__m256*)this, 0b1101));
            } else {
                _mm256_store_si256((__m256i*)&secondCol, _mm256_shuffle_epi32(*(__m256i*)this, 0b1101));
            }
            return secondCol;
        }
    }

    static constexpr Mat2 identity() noexcept {
        if (__builtin_is_constant_evaluated()) {
            return { 1,0,0,1 };
        } else {
            Mat2 identityMat;
            if constexpr (same_type<Type, float>) {
                _mm256_store_ps((float*)&identityMat, _mm256_set_ps(0,0,0,0,1,0,0,1));
            } else {
                _mm256_store_si256((__m256i*)&identityMat, _mm256_set_epi32(0,0,0,0,1,0,0,1));
            }
            return identityMat;
        }
    }

    constexpr void negative() noexcept {
        simdMat.negative();
    }

    constexpr std::size_t size() const noexcept {
        return 4;
    }

    constexpr void transposed() noexcept {
        if (__builtin_is_constant_evaluated()) {
            util::swap(simdMat.data[1], simdMat.data[2]);
        } else {
            if constexpr (same_type<Type, float>) {
                _mm256_store_ps((float*)this, _mm256_permute_ps(*(__m256*)this, 0b11011000));
            } else {
                _mm256_store_si256((__m256i*)this, _mm256_shuffle_epi32(*(__m256i*)this, 0b11011000));
            }
        }
    }

    constexpr Mat2 transpose() const noexcept {
        Mat2 transposedMat = *this;
        transposedMat.transposed();
        return transposedMat;
    }

    constexpr void operator+=(const Mat2 otherMat) noexcept {
        simdMat += otherMat.simdMat;
    }

    constexpr void operator-=(const Mat2 otherMat) noexcept {
        simdMat += otherMat.simdMat;
    }

    template<arithmetic Mul> requires acceptable_loss<Type, Mul>
    constexpr void operator*=(const Mul mul) noexcept {
        simdMat *= mul;
    }

    constexpr Mat2 operator+(const Mat2 otherMat) const noexcept {
        return { simdMat + otherMat.simdMat };
    }

    constexpr Mat2 operator-(const Mat2 otherMat) const noexcept {
        return { simdMat - otherMat.simdMat };
    }

    constexpr Mat2 operator*(const Mat2 otherMat) const noexcept {
        if (__builtin_is_constant_evaluated()) {
            const auto [a0, a2] = ((*this) * this->getFirstCol()).vector.data;
            const auto [a1, a3] = ((*this) * this->getSecondCol()).vector.data;
            return { a0,a1,a2,a3 };
        } else {
            Mat2 result;
            if constexpr (same_type<Type, float>) {
                const auto temp = _mm256_mul_ps(_mm256_permute2f128_ps(*(__m256*)this, *(__m256*)this, 0),
                                  _mm256_permutevar8x32_ps(_mm256_permute2f128_ps(*(__m256*)&otherMat, *(__m256*)&otherMat, 0),
                                  _mm256_set_epi32(3,1,3,1,2,0,2,0)));
               _mm256_store_ps((float*)&result, _mm256_permutevar8x32_ps(_mm256_hadd_ps(temp, temp), _mm256_set_epi32(0,0,0,0,5,1,4,0)));
            } else {
                const auto temp = _mm256_mullo_epi32(_mm256_permute2x128_si256(*(__m256i*)this, *(__m256i*)this, 0), 
                                  _mm256_permutevar8x32_epi32(_mm256_permute2x128_si256(*(__m256i*)&otherMat, *(__m256i*)&otherMat, 0),
                                  _mm256_set_epi32(3,1,3,1,2,0,2,0)));
                _mm256_store_si256((__m256i*)&result, _mm256_permutevar8x32_epi32(_mm256_hadd_epi32(temp, temp), _mm256_set_epi32(0,0,0,0,5,1,4,0)));
            }
            return result;
        }
    }

    /*
        Another Implementation of Matrix Multiplication

        constexpr Mat2 operator*(const Mat2 otherMat) const noexcept {
            if (__builtin_is_constant_evaluated()) {
                const auto [a0, a2] = ((*this) * this->getFirstCol()).vector.data;
                const auto [a1, a3] = ((*this) * this->getSecondCol()).vector.data;
                return { a0,a1,a2,a3 };
            } else {
                Mat2<Type> mulMat;
                if constexpr (same_type<Type, float>) {
                    const auto interData1 = _mm256_mul_ps(*(__m256*)this, _mm256_permute_ps(*(__m256*)__builtin_addressof(otherMat), 0b10001000));
                    const auto interData2 = _mm256_mul_ps(*(__m256*)this, _mm256_permute_ps(*(__m256*)__builtin_addressof(otherMat), 0b11011101));
                    const auto haddData1  = _mm256_hadd_ps(interData1, interData1);
                    const auto haddData2  = _mm256_hadd_ps(interData2, interData2);
                    _mm256_store_ps((float*)__builtin_addressof(mulMat), _mm256_unpacklo_ps(haddData1, haddData2));
                } else {
                    const auto interData1 = _mm256_mullo_epi32(*(__m256i*)this, _mm256_shuffle_epi32(*(__m256i*)__builtin_addressof(otherMat), 0b10001000));
                    const auto interData2 = _mm256_mullo_epi32(*(__m256i*)this, _mm256_shuffle_epi32(*(__m256i*)__builtin_addressof(otherMat), 0b11011101));
                    const auto haddData1  = _mm256_hadd_epi32(interData1, interData1);
                    const auto haddData2  = _mm256_hadd_epi32(interData2, interData2);
                    _mm256_store_si256((__m256i*)__builtin_addressof(mulMat), _mm256_unpacklo_epi32(haddData1, haddData2));
                }
                return mulMat;
            }
        }
    */

    constexpr Vector<Type, 2> operator*(const Vector<Type, 2> vec) const noexcept {
        if (__builtin_is_constant_evaluated()) {
            return { 
                simdMat.data[0] * vec.vector.data[0] + simdMat.data[1] * vec.vector.data[1],
                simdMat.data[2] * vec.vector.data[0] + simdMat.data[3] * vec.vector.data[1]
            };
        } else {
            Vector<Type, 2> result;
            if constexpr (same_type<Type, float>) {
                const auto intData = _mm256_mul_ps(*(__m256*)this, _mm256_permute_ps(*(__m256*)&vec, 0b01000100));
                _mm256_store_ps((float*)&result, _mm256_hadd_ps(intData, intData));
            } else {
                const auto intData = _mm256_mullo_epi32(*(__m256i*)this, _mm256_shuffle_epi32(*(__m256i*)&vec, 0b01000100));
                _mm256_store_si256((__m256i*)&result, _mm256_hadd_epi32(intData, intData));
            }
            return result;
        }
    }

    template<arithmetic Mul> requires acceptable_loss<Type, Mul>
    constexpr Mat2 operator*(const Mul mul) const noexcept {
        return { simdMat * mul };
    }

    constexpr Mat2 operator-() const noexcept {
        return { -simdMat };
    }

    union {
        Array<Type, 4> simdMat;
        struct {
            value_type mat[2][2];
            value_type forAlignment[4];
        };
    };

};

template<arithmetic Mul, euclid_type T>
EuclidForceinline constexpr Mat2<T> operator*(const Mul mul, const Mat2<T> matrix) noexcept {
    return matrix * mul;
}

template<euclid_type First, euclid_type... Rest> requires same_type<First, Rest...>
Mat2(First, Rest...)->Mat2<First>;

using mat2i = Mat2<int>;
using mat2f = Mat2<float>;

template<typename T>
struct is_mat2 {
    static constexpr bool value = false;
};

template<euclid_type T>
struct is_mat2<Mat2<T>> {
    static constexpr bool value = true;
};

}