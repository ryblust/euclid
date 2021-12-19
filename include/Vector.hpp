#pragma once
#pragma warning(disable : 4324 4820)

/*

                  Why Stop Working on the Automatic type convertion
               (e.g., vec4f + vec4i = vec4i, vec4i * 2.f = vec4f, etc.)
    ----------------------------------------------------------------------------

    Reasons I concerned:

    1. Hard to Maintain & Costs Many Time to Write
       It's hard to keep code readable and clean,
       you have to consider every type combination and have to give two impls,
       a compile-time one and a run-time one

    2. Performance Overhead & Tradeoff
       Return Vector<double, Size> type, it generates a lot of extra codes,
       which may due to the x64 'calling convention' I think, So you need to
       decide whether let double type join the type convertion
       Automatical type convertion is just a simple warpper of .cast<type>()
       It needs a lot of if constexpr and more time in the compile-time
    
    3. Potential Errors
       Automatical type convertion essentially is a implicit type convertion,
       So it may cause the unexpected behavior

    4. Not So Common
       I think Vec4f and Vec3f is the most common use one, So it makes
       automatical type convertion a little bit redundant

    5. The Truth
       I was too lazy! :)

    -----------------------------------------------------------------------------

    Basic Implementations:

    template<typename Des, typename Src>
    concept acceptable_loss = signed_type<Des> &&
                              same_type<Des, arithmetic_promotion_t<Des, Src>> &&
                              !same_type<Des, double>;

    template<arithmetic T> requires acceptable_loss_in_return<Type, T>
    constexpr Vector& operator+=(const Vector<T, Size> otherVec) noexcept {
        return (*this += otherVec.cast<Type>());
    }

    template<arithmetic T> requires acceptable_loss_in_return<Type, T>
    constexpr Vector& operator-=(const Vector<T, Size> otherVec) noexcept {
        return (*this -= otherVec.cast<Type>());
    }

    template<arithmetic T> requires (!same_type<Type, double> && !same_type<T, double>)
    constexpr auto operator*(const Vector<T, Size> otherVec) const noexcept {
        using type = arithmetic_promotion_t<Type, T>;
        if constexpr (same_type<Type, type>) {
            return (*this) * otherVec.cast<type>();
        } else {
            return this->cast<type>() * otherVec;
        }
    }

    template<arithmetic T> requires acceptable_loss<Type, T>
    constexpr Vector& operator*=(const T mul) noexcept {
        if (__builtin_is_constant_evaluated()) {
            vec[0] *= mul;
            vec[1] *= mul;
            vec[2] *= mul;
            vec[3] *= mul;
        } else {
            if constexpr (same_type<Type, float>) {
                ::_mm256_store_ps(vec, _mm256_mul_ps(*(__m256*)this, ::_mm256_set1_ps(mul)));
            } else if constexpr (same_type<Type, double>) {
                ::_mm256_store_pd(vec, _mm256_mul_pd(*(__m256d*)this, ::_mm256_set1_pd(mul)));
            } else {
                ::_mm256_store_si256((__m256i*)this, _mm256_mullo_epi32(*(__m256i*)this, ::_mm256_set1_epi32(static_cast<int>(mul))));
            }
        }
        return *this;
    }

    --------------------------------------------------------------------------

    I left my shit here and hope one day i could finish it :)
                                                                
                                      THE END

*/

namespace euclid {

template<arithmetic Type, std::size_t Size>
struct alignas(32) Vector {
public:
    using value_type = Type;

    template<arithmetic CastT> requires promotion_cast<Type, CastT>
    constexpr Vector<CastT, Size> cast() const noexcept {
        if (__builtin_is_constant_evaluated()) {
            return {
                static_cast<CastT>(vec[0]),
                static_cast<CastT>(vec[1]),
                static_cast<CastT>(vec[2]),
                static_cast<CastT>(vec[3])
            };
        } else {
            Vector<CastT, Size> retVec;
            if constexpr (same_type<CastT, float>) {
                ::_mm256_store_ps(retVec.vec, _mm256_cvtepi32_ps(*(__m256i*)this));
            } else {
                if constexpr (same_type<Type, float>) {
                    ::_mm256_store_pd(retVec.vec, _mm256_cvtps_pd(*(__m128*)this));
                } else {
                    ::_mm256_store_pd(retVec.vec, _mm256_cvtepi32_pd(*(__m128i*)this));
                }
            }
            return retVec;
        }
    }

    constexpr Vector& negative() noexcept requires (signed_type<Type>) {
        if (__builtin_is_constant_evaluated()) {
            vec[0] = -vec[0];
            vec[1] = -vec[1];
            vec[2] = -vec[2];
            vec[3] = -vec[3];
        } else {
            if constexpr (same_type<Type, float>) {
                ::_mm256_store_ps(vec, _mm256_sub_ps(_mm256_setzero_ps(), *(__m256*)this));
            } else if constexpr (same_type<Type, double>) {
                ::_mm256_store_pd(vec, _mm256_sub_pd(_mm256_setzero_pd(), *(__m256d*)this));
            } else {
                ::_mm256_store_si256((__m256i*)this, _mm256_sub_epi32(_mm256_setzero_si256(), *(__m256i*)this));
            }
        }
        return *this;
    }

    constexpr auto norm() const noexcept {
        if (__builtin_is_constant_evaluated()) {
            const value_type squreSum = vec[0] * vec[0] + vec[1] * vec[1] + 
                                        vec[2] * vec[2] + vec[3] * vec[3];
            return math::sqrt(static_cast<float>(squreSum));
        } else {
            if constexpr (same_type<Type, float>) {
                const auto dot_temp1 = _mm256_mul_ps(*(__m256*)this, *(__m256*)this);
                const auto dot_temp2 = _mm256_hadd_ps(dot_temp1, dot_temp1);
                return _mm256_sqrt_ps(_mm256_hadd_ps(dot_temp2, dot_temp2)).m256_f32[0];
            } else if constexpr (same_type<Type, double>) {
                const auto dot_temp1 = _mm256_mul_pd(*(__m256d*)this, *(__m256d*)this);
                const auto dot_temp2 = _mm256_hadd_pd(dot_temp1, dot_temp1);
                return _mm256_sqrt_pd(_mm256_hadd_pd(dot_temp2, dot_temp2)).m256d_f64[0];
            } else {
                const auto dot_temp1 = _mm256_mullo_epi32(*(__m256i*)this, *(__m256i*)this);
                const auto dot_temp2 = _mm256_hadd_epi32(dot_temp1, dot_temp1);
                return _mm256_sqrt_ps(_mm256_cvtepi32_ps(_mm256_hadd_epi32(dot_temp2, dot_temp2))).m256_f32[0];
            }
        }
    }

    constexpr auto distance(const Vector otherVec) const noexcept {
        return ((*this) - otherVec).norm();
    }

    constexpr auto normalize() const noexcept {
        return this->cast<arithmetic_promotion_t<Type, float>>() / this->norm();
    }

    constexpr value_type dot(const Vector otherVec) const noexcept {
        if (__builtin_is_constant_evaluated()) {
            return vec[0] * otherVec.vec[0] +
                   vec[1] * otherVec.vec[1] +
                   vec[2] * otherVec.vec[2];
        } else {
            if constexpr (same_type<Type, float>) {
                const auto dot_temp1 = _mm256_mul_ps(*(__m256*)this, *(__m256*)__builtin_addressof(otherVec));
                const auto dot_temp2 = _mm256_hadd_ps(dot_temp1, dot_temp1);
                return _mm256_hadd_ps(dot_temp2, dot_temp2).m256_f32[0];
            } else if constexpr (same_type<Type, double>) {
                const auto dot_temp1 = _mm256_mul_pd(*(__m256d*)this, *(__m256d*)__builtin_addressof(otherVec));
                const auto dot_temp2 = _mm256_hadd_pd(dot_temp1, dot_temp1);
                return _mm256_hadd_pd(dot_temp2, dot_temp2).m256d_f64[0];
            } else {
                const auto dot_temp1 = _mm256_mullo_epi32(*(__m256i*)this, *(__m256i*)__builtin_addressof(otherVec));
                const auto dot_temp2 = _mm256_hadd_epi32(dot_temp1, dot_temp1);
                if constexpr (same_type<Type, int>) {
                    return _mm256_hadd_epi32(dot_temp2, dot_temp2).m256i_i32[0];
                } else {
                    return _mm256_hadd_epi32(dot_temp2, dot_temp2).m256i_u32[0];
                }
            }
        }
    }

    constexpr value_type operator*(const Vector otherVec) const noexcept {
        if (__builtin_is_constant_evaluated()) {
            return vec[0] * otherVec.vec[0] +
                   vec[1] * otherVec.vec[1] +
                   vec[2] * otherVec.vec[2] +
                   vec[3] * otherVec.vec[3];
        } else {
            if constexpr (same_type<Type, float>) {
                const auto dot_temp1 = _mm256_mul_ps(*(__m256*)this, *(__m256*)__builtin_addressof(otherVec));
                const auto dot_temp2 = _mm256_hadd_ps(dot_temp1, dot_temp1);
                return _mm256_hadd_ps(dot_temp2, dot_temp2).m256_f32[0];
            } else if constexpr (same_type<Type, double>) {
                const auto dot_temp1 = _mm256_mul_pd(*(__m256d*)this, *(__m256d*)__builtin_addressof(otherVec));
                const auto dot_temp2 = _mm256_hadd_pd(dot_temp1, dot_temp1);
                return _mm256_hadd_pd(dot_temp2, dot_temp2).m256d_f64[0];
            } else {
                const auto dot_temp1 = _mm256_mullo_epi32(*(__m256i*)this, *(__m256i*)__builtin_addressof(otherVec));
                const auto dot_temp2 = _mm256_hadd_epi32(dot_temp1, dot_temp1);
                if constexpr (same_type<Type, int>) {
                    return _mm256_hadd_epi32(dot_temp2, dot_temp2).m256i_i32[0];
                } else {
                    return _mm256_hadd_epi32(dot_temp2, dot_temp2).m256i_u32[0];
                }
            }
        }
    }

    constexpr Vector& operator+=(const Vector otherVec) noexcept {
        if (__builtin_is_constant_evaluated()) {
            vec[0] += otherVec.vec[0];
            vec[1] += otherVec.vec[1];
            vec[2] += otherVec.vec[2];
            vec[3] += otherVec.vec[3];
        } else {
            if constexpr (same_type<Type, float>) {
                ::_mm256_store_ps(vec, _mm256_add_ps(*(__m256*)this, *(__m256*)__builtin_addressof(otherVec)));
            } else if constexpr (same_type<Type, double>) {
                ::_mm256_store_pd(vec, _mm256_add_pd(*(__m256d*)this, *(__m256d*)__builtin_addressof(otherVec)));
            } else {
                ::_mm256_store_si256((__m256i*)this, _mm256_add_epi32(*(__m256i*)this, *(__m256i*)__builtin_addressof(otherVec)));
            }
        }
        return *this;
    }

    constexpr Vector& operator-=(const Vector otherVec) noexcept {
        if (__builtin_is_constant_evaluated()) {
            vec[0] -= otherVec.vec[0];
            vec[1] -= otherVec.vec[1];
            vec[2] -= otherVec.vec[2];
            vec[3] -= otherVec.vec[3];
        } else {
            if constexpr (same_type<Type, float>) {
                ::_mm256_store_ps(vec, _mm256_sub_ps(*(__m256*)this, *(__m256*)__builtin_addressof(otherVec)));
            } else if constexpr (same_type<Type, double>) {
                ::_mm256_store_pd(vec, _mm256_sub_pd(*(__m256d*)this, *(__m256d*)__builtin_addressof(otherVec)));
            } else {
                ::_mm256_store_si256((__m256i*)this, _mm256_sub_epi32(*(__m256i*)this, *(__m256i*)__builtin_addressof(otherVec)));
            }
        }
        return *this;
    }

    constexpr Vector operator+(const Vector otherVec) const noexcept {
        Vector temp = *this;
        return temp += otherVec;
    }

    constexpr Vector operator-(const Vector otherVec) const noexcept {
        Vector temp = *this;
        return temp -= otherVec;
    }

    template<arithmetic T> requires acceptable_loss_basic<Type, T>
    constexpr Vector& operator*=(const T mul) noexcept  {
        if (__builtin_is_constant_evaluated()) {
            vec[0] *= mul;
            vec[1] *= mul;
            vec[2] *= mul;
            vec[3] *= mul;
        } else {
            if constexpr (same_type<Type, float>) {
                ::_mm256_store_ps(vec, _mm256_mul_ps(*(__m256*)this, ::_mm256_set1_ps(mul)));
            } else if constexpr (same_type<Type, double>) {
                ::_mm256_store_pd(vec, _mm256_mul_pd(*(__m256d*)this, ::_mm256_set1_pd(mul)));
            } else {
                ::_mm256_store_si256((__m256i*)this, _mm256_mullo_epi32(*(__m256i*)this, ::_mm256_set1_epi32(static_cast<int>(mul))));
            }
        }
        return *this;
    }

    template<arithmetic T> requires acceptable_loss_basic<Type, T>
    constexpr Vector& operator/=(const T div) noexcept {
        if (__builtin_is_constant_evaluated()) {
            vec[0] /= div;
            vec[1] /= div;
            vec[2] /= div;
            vec[3] /= div;
        } else {
            if constexpr (same_type<Type, double>) {
                ::_mm256_store_pd(vec, _mm256_div_pd(*(__m256d*)this, ::_mm256_set1_pd(div)));
            } else {
                ::_mm256_store_ps(vec, _mm256_div_ps(*(__m256*)this, ::_mm256_set1_ps(static_cast<float>(div))));
            }
        }
        return *this;
    }

    template<arithmetic T> requires acceptable_loss_basic<Type, T>
    constexpr Vector operator*(const T mul) const noexcept {
        Vector temp = *this;
        return temp *= mul;
    }

    template<arithmetic T> requires acceptable_loss_basic<Type, T>
    constexpr Vector operator/(const T div) const noexcept {
        Vector temp = *this;
        return temp /= div;
    }

    constexpr Vector operator-() const noexcept {
        Vector temp = *this;
        return temp.negative();
    }

    constexpr value_type operator[](const std::size_t pos) const noexcept {
        return vec[pos];
    }

    constexpr value_type& operator[](const std::size_t pos) noexcept {
        return vec[pos];
    }

    value_type vec[4];
};

template<arithmetic T>
using vec2 = Vector<T, 2>;

template<arithmetic T>
using vec3 = Vector<T, 3>;

template<arithmetic T>
using vec4 = Vector<T, 4>;

using vec2i = Vector<int, 2>;
using vec3i = Vector<int, 3>;
using vec4i = Vector<int, 4>;

using vec2u = Vector<unsigned, 2>;
using vec3u = Vector<unsigned, 3>;
using vec4u = Vector<unsigned, 4>;

using vec2f = Vector<float, 2>;
using vec3f = Vector<float, 3>;
using vec4f = Vector<float, 4>;

using vec2d = Vector<double, 2>;
using vec3d = Vector<double, 3>;
using vec4d = Vector<double, 4>;

template<arithmetic First, arithmetic... Rest> requires same_type<First, Rest...>
Vector(First, Rest...)->Vector<First, sizeof...(Rest) + 1>;

}