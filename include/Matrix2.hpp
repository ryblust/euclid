#pragma once

namespace euclid {

template<euclid_type Type>
struct alignas(32) Mat2 {
public:
    using value_type = Type;

    constexpr Mat2<float> castTofloat() const noexcept requires(same_type<Type, int>) {
        return { mat.castTofloat() };
    }

    constexpr Vector<Type, 2> getColVec(const std::size_t pos) const noexcept {
        if (pos == 0) {
            return getFirstCol();
        } else if (pos == 1) {
            return getSecondCol();
        } else [[unlikely]] return {};
    }

    constexpr Vector<Type, 2> getFirstCol() const noexcept {
        if (__builtin_is_constant_evaluated()) {
            return { mat.data[0], mat.data[2] };
        } else {
            Vector<Type, 2> firstCol;
            if constexpr (same_type<Type, float>) {
                _mm256_store_ps((float*)__builtin_addressof(firstCol), _mm256_permute_ps(*(__m256*)this, 0b1000));
            } else {
                _mm256_store_si256((__m256i*)__builtin_addressof(firstCol), _mm256_shuffle_epi32(*(__m256i*)this, 0b1000));
            }
            return firstCol;
        }
    }

    constexpr Vector<Type, 2> getSecondCol() const noexcept {
        if (__builtin_is_constant_evaluated()) {
            return { mat.data[1], mat.data[3] };
        } else {
            Vector<Type, 2> secondCol;
            if constexpr (same_type<Type, float>) {
                _mm256_store_ps((float*)__builtin_addressof(secondCol), _mm256_permute_ps(*(__m256*)this, 0b1101));
            } else {
                _mm256_store_si256((__m256i*)__builtin_addressof(secondCol), _mm256_shuffle_epi32(*(__m256i*)this, 0b1101));
            }
            return secondCol;
        }
    }

    static constexpr Mat2 identity() noexcept {
        return { 1,0,0,1 };
    }

    constexpr void negative() noexcept {
        mat.negative();
    }

    constexpr std::size_t size() const noexcept {
        return 4;
    }

    constexpr void transposed() noexcept {
        if (__builtin_is_constant_evaluated()) {
            util::swap(mat.data[1], mat.data[2]);
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
        mat += otherMat.mat;
    }

    constexpr void operator-=(const Mat2 otherMat) noexcept {
        mat += otherMat.mat;
    }

    template<arithmetic Mul> requires acceptable_loss<Type, Mul>
    constexpr void operator*=(const Mul mul) noexcept {
        mat *= mul;
    }

    constexpr Mat2 operator+(const Mat2 otherMat) const noexcept {
        return { mat + otherMat.mat };
    }

    constexpr Mat2 operator-(const Mat2 otherMat) const noexcept {
        return { mat - otherMat.mat };
    }

    constexpr Mat2 operator*(const Mat2 otherMat) const noexcept {
        if (__builtin_is_constant_evaluated()) {
            // Todo
        } else {
            Mat2<Type> mulMat;
            if constexpr (same_type<Type, float>) {
                const auto interData1 = _mm256_mul_ps(*(__m256*)this, _mm256_permute_ps(*(__m256*)__builtin_addressof(otherMat), 0b10001000));
                const auto interData2 = _mm256_mul_ps(*(__m256*)this, _mm256_permute_ps(*(__m256*)__builtin_addressof(otherMat), 0b11011101));
                const auto haddData1  = _mm256_hadd_ps(interData1, interData1);
                const auto haddData2  = _mm256_hadd_ps(interData2, interData2);
                _mm256_store_ps((float*)__builtin_addressof(mulMat), _mm256_unpacklo_ps(haddData1, haddData2));
            } else {
                // Todo
            }
            return mulMat;
        }
    }

    constexpr Vector<Type, 2> operator*(const Vector<Type, 2> vec) const noexcept {
        if (__builtin_is_constant_evaluated()) {
            return { 
                mat.data[0] * vec.vector.data[0] + mat.data[1] * vec.vector.data[1],
                mat.data[2] * vec.vector.data[0] + mat.data[3] * vec.vector.data[1]
            };
        } else {
            Vector<Type, 2> resVec;
            if constexpr (same_type<Type, float>) {
                const auto interVec = _mm256_mul_ps(*(__m256*)__builtin_addressof(mat),
                                      _mm256_permute_ps(*(__m256*)__builtin_addressof(vec), 0b01000100));
                _mm256_store_ps((float*)__builtin_addressof(resVec), _mm256_hadd_ps(interVec, interVec));
            } else {
                const auto interVec = _mm256_mullo_epi32(*(__m256i*)__builtin_addressof(mat),
                                      _mm256_shuffle_epi32(*(__m256i*)__builtin_addressof(vec), 0b01000100));
                _mm256_store_si256((__m256i*)__builtin_addressof(resVec), _mm256_hadd_epi32(interVec, interVec));
            }
            return resVec;
        }
    }

    template<arithmetic Mul> requires acceptable_loss<Type, Mul>
    constexpr Mat2 operator*(const Mul mul) const noexcept {
        return { mat * mul };
    }

    constexpr Mat2 operator-() const noexcept {
        return { -mat };
    }

    Array<Type, 4> mat;
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