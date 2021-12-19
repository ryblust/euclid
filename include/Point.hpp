#pragma once

namespace euclid {

template<arithmetic Type, std::size_t Size>
class alignas(32) Point {
public:
    using value_type = Type;

    template<arithmetic CastT> requires promotion_cast<Type, CastT>
    constexpr Point<CastT, Size> cast() const noexcept {
        if (__builtin_is_constant_evaluated()) {
            return {
                static_cast<CastT>(point[0]),
                static_cast<CastT>(point[1]),
                static_cast<CastT>(point[2]),
                static_cast<CastT>(point[3])
            };
        } else {
            Point<CastT, Size> retPoint;
            if constexpr (same_type<CastT, float>) {
                ::_mm256_store_ps(retPoint.point, _mm256_cvtepi32_ps(*(__m256i*)this));
            } else {
                if constexpr (same_type<Type, float>) {
                    ::_mm256_store_pd(retPoint.point, _mm256_cvtps_pd(*(__m128*)this));
                } else {
                    ::_mm256_store_pd(retPoint.point, _mm256_cvtepi32_pd(*(__m128i*)this));
                }
            }
            return retPoint;
        }
    }

    constexpr Point& negative() noexcept {
        if (__builtin_is_constant_evaluated()) {
            point[0] = -point[0];
            point[1] = -point[1];
            point[2] = -point[2];
            point[3] = -point[3];
        } else {
            if constexpr (same_type<Type, float>) {
                ::_mm256_store_ps(point, _mm256_sub_ps(_mm256_setzero_ps(), *(__m256*)this));
            } else if constexpr (same_type<Type, double>) {
                ::_mm256_store_pd(point, _mm256_sub_pd(_mm256_setzero_pd(), *(__m256d*)this));
            } else {
                ::_mm256_store_si256((__m256i*)this, _mm256_sub_epi32(_mm256_setzero_si256(), *(__m256i*)this));
            }
        }
        return *this;
    }

    constexpr Point operator-() const noexcept {
        Point retPoint = *this;
        return retPoint.negative();
    }

    constexpr Vector<Type, Size> operator-(const Point otherPoint) const noexcept {
        if (__builtin_is_constant_evaluated()) {
            return Vector<Type, Size> {
                point[0] - otherPoint.point[0],
                point[1] - otherPoint.point[1],
                point[2] - otherPoint.point[2],
                point[3] - otherPoint.point[3]
            };
        } else {
            Vector<Type, Size> retVec;
            if constexpr (same_type<Type, float>) {
                ::_mm256_store_ps(retVec.vec, _mm256_sub_ps(*(__m256*)this, *(__m256*)__builtin_addressof(otherPoint)));
            } else if constexpr (same_type<Type, double>) {
                ::_mm256_store_pd(retVec.vec, _mm256_sub_pd(*(__m256d*)this, *(__m256d*)__builtin_addressof(otherPoint)));
            } else {
                ::_mm256_store_si256((__m256i*)&retVec, _mm256_sub_epi32(*(__m256i*)this, *(__m256i*)__builtin_addressof(otherPoint)));
            }
            return retVec;
        }
    }

    constexpr auto distance(const Point otherPoint) const noexcept {
        return ((*this) - otherPoint).norm();
    }

    constexpr value_type operator[](const std::size_t pos) const noexcept {
        return point[pos];
    }

    constexpr value_type& operator[](const std::size_t pos) noexcept {
        return point[pos];
    }

    value_type point[4]{};
};

template<arithmetic First, arithmetic... Rest> requires same_type<First, Rest...>
Point(First, Rest...)->Point<First, sizeof...(Rest) + 1>;

template<arithmetic T>
using point2 = Point<T, 2>;

template<arithmetic T>
using point3 = Point<T, 3>;

template<arithmetic T>
using point4 = Point<T, 4>;

using point2i = Point<int, 2>;
using point3i = Point<int, 3>;
using point4i = Point<int, 4>;

using point2u = Point<unsigned, 2>;
using point3u = Point<unsigned, 3>;
using point4u = Point<unsigned, 4>;

using point2f = Point<float, 2>;
using point3f = Point<float, 3>;
using point4f = Point<float, 4>;

using point2d = Point<double, 2>;
using point3d = Point<double, 3>;
using point4d = Point<double, 4>;

}