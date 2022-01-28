#pragma once

namespace euclid {

template<euclid_type Type>
struct Mat3 {
    using value_type = Type;

    constexpr Mat3<float> castTofloat() const noexcept requires(same_type<Type, int>) {
        return { simdMat.castTofloat(), static_cast<float>(lastElem) };
    }

    constexpr Vector<Type, 3> getFirstCol() const noexcept {
        if(__builtin_is_constant_evaluated()) {
            return { simdMat.data[0], simdMat.data[3], simdMat.data[6] };
        }
        if constexpr (same_type<Type, float>) {

        } else {

        }
    }

    constexpr Vector<Type, 3> getSecondCol() const noexcept {
        if(__builtin_is_constant_evaluated()) {
            return { simdMat.data[1], simdMat.data[4], simdMat.data[7] };
        }
        if constexpr (same_type<Type, float>) {

        } else {
            
        } 
    }

    constexpr Vector<Type, 3> getThirdCol() const noexcept {
        if(__builtin_is_constant_evaluated()) {
            return { simdMat.data[2], simdMat.data[5], lastElem };
        }
        if constexpr (same_type<Type, float>) {

        } else {
            
        }
    }

    static constexpr Mat3 identity() noexcept {
        if (__builtin_is_constant_evaluated()) {
            return {
                1, 0, 0,
                0, 1, 0,
                0, 0, 1
            };
        }
        if constexpr (same_type<Type, float>) {

        } else {

        }
    }

    constexpr void negative() noexcept {
        simdMat.negative();
        lastElem = -lastElem;
    }
    
    constexpr void transposed() noexcept {

    }

    constexpr Mat3 transpose() const noexcept {
        Mat3 transposedMat = *this;
        transposedMat.transposed();
        return transposedMat;
    }

    constexpr Mat3& operator+=(const Mat3 otherMat) noexcept {
        simdMat  += otherMat.simdMat;
        lastElem += otherMat.lastElem;
        return *this;
    }

    constexpr Mat3& operator-=(const Mat3 otherMat) noexcept {
        simdMat  -= otherMat.simdMat;
        lastElem -= otherMat.lastElem;
        return *this;
    }

    constexpr Mat3& operator+(const Mat3 otherMat) const noexcept {
        Mat3 temp = *this;
        return temp += otherMat;
    }

    constexpr Mat3& operator-(const Mat3 otherMat) const noexcept {
        Mat3 temp = *this;
        return temp -= otherMat;
    }

    template<arithmetic Mul> requires acceptable_loss<Type, Mul>
    constexpr Mat3& operator*=(const Mul mul) noexcept {
        simdMat  *= mul;
        lastElem *= mul;
        return *this;
    }

    template<arithmetic Mul> requires acceptable_loss<Type, Mul>
    constexpr Mat3 operator*(const Mul mul) const noexcept {
        Mat3 temp = *this;
        return temp *= mul;
    }

    union {
        struct {
            Array<Type, 8> simdMat;
            value_type lastElem;
        };
        value_type mat[3][3];
    };
};

template<arithmetic Mul, euclid_type T> requires acceptable_loss<T, Mul>
EUCLID_FORCEINLINE constexpr Mat3<T> operator*(const Mul mul, const Mat3<T> mat) noexcept {
    return mat * mul;
}

template<euclid_type First, euclid_type... Rest> requires same_type<First, Rest...>
Mat3(First, Rest...)->Mat3<First>;

using mat3i = Mat3<int>;
using mat3f = Mat3<float>;

template<typename T>
struct is_mat3 {
    static constexpr bool value = false;
};

template<euclid_type T>
struct is_mat3<Mat3<T>> {
    static constexpr bool value = true;
};

}