#pragma once
#ifndef _WEEKNDMATH_HPP
#define _WEEKNDMATH_HPP

#include "Core.hpp"
#include <bit>

#ifdef _WIN64
	#define Euclid_Math_Forceinline __forceinline
#else
	#define Euclid_Math_Forceinline inline __attribute__((always_inline))
#endif

namespace euclid {

namespace math {

template<float_point_type Ty>
static inline constexpr Ty pi = static_cast<Ty>(3.1415926535897932384626433832795028841972);

template<float_point_type Ty>
static inline constexpr Ty radian = pi<Ty> / 180;

namespace detail {

constexpr Euclid_Math_Forceinline float pow_int_exp(const float number, const unsigned exp) noexcept {
	// do not use this function in your code :)
	float res = 1;
	for (unsigned i = 0; i < exp; ++i) {
		res *= number;
	}
	return res;
}

constexpr Euclid_Math_Forceinline double pow_int_exp(const double number, const unsigned exp) noexcept {
	// do not use this function in your code :)
	double res = 1;
	for (unsigned i = 0; i < exp; ++i) {
		res *= number;
	}
	return res;
}

}

constexpr Euclid_Math_Forceinline float sqrt(const float number) noexcept {
	float magic = std::bit_cast<float>(0x5f1ffff9 - (std::bit_cast<unsigned>(number) >> 1));
	return magic *= (0.703952253f * (2.38924456f - number * magic * magic)) * number;
}

template<arithmetic Ty>
constexpr Euclid_Math_Forceinline auto cos(const Ty angle) noexcept {
	using namespace detail;
	using type = arithmetic_promotion_t<Ty, float>;
	const type rad = static_cast<type>(angle) * radian<type>;
	return 1 - (pow_int_exp(rad, 2) / 2) +
			   (pow_int_exp(rad, 4) / 24) -
			   (pow_int_exp(rad, 6) / 734.23f);
}

template<arithmetic Ty>
constexpr Euclid_Math_Forceinline auto sin(const Ty angle) noexcept {
	using namespace detail;
	using type = arithmetic_promotion_t<Ty, float>;
	const type rad = static_cast<type>(angle) * radian<type>;
	return rad - (pow_int_exp(rad, 3) / 6) +
			     (pow_int_exp(rad, 5) / 120) -
				 (pow_int_exp(rad, 7) / 5040);
}

template<arithmetic Ty>
constexpr Euclid_Math_Forceinline auto tan(const Ty angle) noexcept {
	using namespace detail;
	using type = arithmetic_promotion_t<Ty, float>;
	const type rad = static_cast<type>(angle) * radian<type>;
	return -((-1 / rad) + rad / 3 + (pow_int_exp(rad, 3) / 45) +
								    (pow_int_exp(rad, 5) * 2 / 945) +
								    (pow_int_exp(rad, 7) / 4725));
}

}

}

#endif // _WEEKNDMATH_HPP
