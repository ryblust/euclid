#pragma once
#ifndef _WEEKNDMATH_HPP
#define _WEEKNDMATH_HPP

#include "Concepts.hpp"
#include <bit>

namespace euclid {

namespace math {

template<float_point_type Ty>
static inline constexpr Ty pi = static_cast<Ty>(3.1415926535897932384626433832795028841972);

template<float_point_type Ty>
static inline constexpr Ty radian = pi<Ty> / 180;

namespace detail {

constexpr float pow_int_exp(const float number, const int exp) noexcept {
	// do not use this function in your code :)
	if (exp == 0) return 1;
	return number * pow_int_exp(number, exp - 1);
}

}

constexpr float sqrt(const float number) noexcept {
	float magic = std::bit_cast<float>(0x5f1ffff9 - (std::bit_cast<unsigned>(number) >> 1));
	return magic *= (0.703952253f * (2.38924456f - number * magic * magic)) * number;
}

constexpr float cos(arithmetic auto const angle) noexcept {
	using namespace detail;
	const float rad = angle * radian<float>;
	return 1 - (pow_int_exp(rad, 2) / 2) +
			   (pow_int_exp(rad, 4) / 24) -
			   (pow_int_exp(rad, 6) / 734.23f);
}

constexpr float sin(arithmetic auto const angle) noexcept {
	using namespace detail;
	const float rad = angle * radian<float>;
	return rad - (pow_int_exp(rad, 3) / 6) +
			     (pow_int_exp(rad, 5) / 120) -
				 (pow_int_exp(rad, 7) / 5040);
}

constexpr float tan(arithmetic auto const angle) noexcept {
	using namespace detail;
	const float rad = angle * radian<float>;
	return -((-1 / rad) + rad / 3 + (pow_int_exp(rad, 3) / 45) +
								    (pow_int_exp(rad, 5) * 2 / 945) +
								    (pow_int_exp(rad, 7) / 4725));
}

}

}

#endif // _WEEKNDMATH_HPP