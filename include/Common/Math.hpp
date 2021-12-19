#pragma once

namespace euclid {

namespace math {

template<float_point_type Ty>
inline constexpr Ty pi = static_cast<Ty>(3.1415926535897932384626433832795028841972);

template<float_point_type Ty>
inline constexpr Ty radian = pi<Ty> / 180;

namespace detail {

Euclid_Forceinline constexpr float pow_int_exp(const float number, const unsigned exp) noexcept {
	// do not use this function in your code :)
	float res = 1;
	for (unsigned i = 0; i < exp; ++i) {
		res *= number;
	}
	return res;
}

Euclid_Forceinline constexpr double pow_int_exp(const double number, const unsigned exp) noexcept {
	// do not use this function in your code :)
	double res = 1;
	for (unsigned i = 0; i < exp; ++i) {
		res *= number;
	}
	return res;
}

}

Euclid_Forceinline constexpr float sqrt(const float number) noexcept {
	float magic = std::bit_cast<float>(0x5f1ffff9 - (std::bit_cast<unsigned>(number) >> 1));
	return magic *= (0.703952253f * (2.38924456f - number * magic * magic)) * number;
}

template<arithmetic Ty>
Euclid_Forceinline constexpr auto cos(const Ty angle) noexcept {
	using namespace detail;
	using type = arithmetic_promotion_t<Ty, float>;
	const type rad = static_cast<type>(angle) * radian<type>;
	return 1 - (pow_int_exp(rad, 2) / 2) +
			   (pow_int_exp(rad, 4) / 24) -
			   (pow_int_exp(rad, 6) / 734.23f);
}

template<arithmetic Ty>
Euclid_Forceinline constexpr auto sin(const Ty angle) noexcept {
	using namespace detail;
	using type = arithmetic_promotion_t<Ty, float>;
	const type rad = static_cast<type>(angle) * radian<type>;
	return rad - (pow_int_exp(rad, 3) / 6) +
			     (pow_int_exp(rad, 5) / 120) -
				 (pow_int_exp(rad, 7) / 5040);
}

template<arithmetic Ty>
Euclid_Forceinline constexpr auto tan(const Ty angle) noexcept {
	using namespace detail;
	using type = arithmetic_promotion_t<Ty, float>;
	const type rad = static_cast<type>(angle) * radian<type>;
	return -((-1 / rad) + rad / 3 + (pow_int_exp(rad, 3) / 45) +
								    (pow_int_exp(rad, 5) * 2 / 945) +
								    (pow_int_exp(rad, 7) / 4725));
}

}

}