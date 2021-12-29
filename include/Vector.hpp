#pragma once

#define _euclid_shuffle_ps(vec, imm8) _mm256_shuffle_ps(vec, vec, imm8)

namespace euclid {

template<arithmetic Type, std::size_t Size>
struct alignas(32) Vector {
public:
    using value_type = Type;

    constexpr std::size_t size() const noexcept {
        return Size;
    }

    Vector<float, Size> cast() const noexcept requires (same_type<Type, int>) {
        return { vector.cast() };
    }

    Vector& negative() noexcept {
        vector.negative();
        return *this;
    }

    float norm() const noexcept {
        if constexpr (same_type<Type, float>) {
            #ifdef _MSC_VER
                return _mm256_sqrt_ps(_mm256_dp_ps(*(__m256*)this, *(__m256*)this, 0b11111111)).m256_f32[0];
            #else
                return _mm256_sqrt_ps(_mm256_dp_ps(*(__m256*)this, *(__m256*)this, 0b11111111))[0];
            #endif // _MSC_VER
        } else {
            #ifdef _MSC_VER
                return _mm256_sqrt_ps(_mm256_dp_ps(_mm256_cvtepi32_ps(*(__m256i*)this), _mm256_cvtepi32_ps(*(__m256i*)this), 0b1111111)).m256_f32[0];
            #else
                return _mm256_sqrt_ps(_mm256_dp_ps(_mm256_cvtepi32_ps(*(__m256i*)this), _mm256_cvtepi32_ps(*(__m256i*)this), 0b1111111))[0]; 
            #endif // _MSC_VER
        }
    }

    float distance(const Vector otherVec) const noexcept {
        return ((*this) - otherVec).norm();
    }

    Vector& normalized() noexcept requires(float_point_type<Type>) {
        ::_mm256_store_ps((float*)this,
            _mm256_mul_ps(*(__m256*)this,
                _mm256_rsqrt_ps(
                    _mm256_dp_ps(*(__m256*)this, *(__m256*)this, 0b11111111))));
        return *this;
    }

    Vector<float, Size> normalize() const noexcept {
        Vector normalizedVec = *this;
        if constexpr (same_type<Type, float>) {
            return normalizedVec.normalized();
        } else {
            return normalizedVec.cast().normalized();
        }
    }

    Vector cross(const Vector otherVec) const noexcept requires(Size >= 3) {
        Vector crossVec;
        if constexpr (same_type<Type, float>) {
            ::_mm256_store_ps((float*)__builtin_addressof(crossVec),
                _euclid_shuffle_ps(
                    _mm256_sub_ps(
                        _mm256_mul_ps(*(__m256*)this,
                            _euclid_shuffle_ps(*(__m256*)__builtin_addressof(otherVec), 0b11001001)),
                        _mm256_mul_ps(
                            _euclid_shuffle_ps(*(__m256*)this, 0b11001001), *(__m256*)__builtin_addressof(otherVec))), 0b11001001));
        } else {
            ::_mm256_store_si256((__m256i*)__builtin_addressof(crossVec),
                _mm256_shuffle_epi32(
                    _mm256_sub_epi32(
                        _mm256_mullo_epi32(*(__m256i*)this, 
                            _mm256_shuffle_epi32(*(__m256i*)__builtin_addressof(otherVec), 0b11001001)),
                        _mm256_mullo_epi32(
                            _mm256_shuffle_epi32(*(__m256i*)this, 0b11001001), *(__m256i*)__builtin_addressof(otherVec))), 0b11001001));
        }
        return crossVec;
    }

    value_type dot(const Vector otherVec) const noexcept {
        return (*this) * otherVec;
    }

    value_type operator*(const Vector otherVec) const noexcept {
        if constexpr (same_type<Type, int>) {
            #ifdef _MSC_VER
                return static_cast<int>(_mm256_dp_ps(_mm256_cvtepi32_ps(*(__m256i*)this), _mm256_cvtepi32_ps(*(__m256i*)__builtin_addressof(otherVec)), 0b1111111).m256_f32[0]);
            #else
                return static_cast<int>(_mm256_dp_ps(_mm256_cvtepi32_ps(*(__m256i*)this), _mm256_cvtepi32_ps(*(__m256i*)__builtin_addressof(otherVec)), 0b1111111)[0]);
            #endif // _MSC_VER           
        } else {
            #ifdef _MSC_VER
                return _mm256_dp_ps(*(__m256*)this, *(__m256*)__builtin_addressof(otherVec), 0b11111111).m256_f32[0];
            #else
                return _mm256_dp_ps(*(__m256*)this, *(__m256*)__builtin_addressof(otherVec), 0b11111111)[0];
            #endif // _MSC_VER
        }
    }

    Vector& operator+=(const Vector otherVec) noexcept {
        vector += otherVec.vector;
        return *this;
    }

    Vector& operator-=(const Vector otherVec) noexcept {
        vector -= otherVec.vector;
        return *this;
    }

    Vector operator+(const Vector otherVec) const noexcept {
        Vector temp = *this;
        return temp += otherVec;
    }

    Vector operator-(const Vector otherVec) const noexcept {
        Vector temp = *this;
        return temp -= otherVec;
    }

    template<arithmetic Mul> requires acceptable_loss<Type, Mul>
    Vector& operator*=(const Mul mul) noexcept  {
        vector *= mul;
        return *this;
    }

    template<arithmetic Div> requires acceptable_loss<Type, Div>
    Vector& operator/=(const Div div) noexcept {
        vector /= div;
        return *this;
    }

    template<arithmetic Mul> requires acceptable_loss<Type, Mul>
    Vector operator*(const Mul mul) const noexcept {
        Vector temp = *this;
        return temp *= mul;
    }

    template<arithmetic Div> requires acceptable_loss<Type, Div>
    Vector operator/(const Div div) const noexcept {
        Vector temp = *this;
        return temp /= div;
    }

    Vector operator-() const noexcept {
        Vector temp = *this;
        return temp.negative();
    }

    Array<Type, Size> vector;
};

template<arithmetic Mul, arithmetic T, std::size_t S> requires acceptable_loss<T, Mul>
EuclidForceinline auto operator*(const Mul mul, const Vector<T, S> vector) noexcept {
    return vector * mul;
}

template<arithmetic T>
using vec2 = Vector<T, 2>;

template<arithmetic T>
using vec3 = Vector<T, 3>;

template<arithmetic T>
using vec4 = Vector<T, 4>;

using vec2i = Vector<int, 2>;
using vec3i = Vector<int, 3>;
using vec4i = Vector<int, 4>;

using vec2f = Vector<float, 2>;
using vec3f = Vector<float, 3>;
using vec4f = Vector<float, 4>;

template<arithmetic First, arithmetic... Rest> requires same_type<First, Rest...>
Vector(First, Rest...)->Vector<First, sizeof...(Rest) + 1>;

}