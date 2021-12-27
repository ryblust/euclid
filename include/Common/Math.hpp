#pragma once

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4514) // Ignore compiler's complain about removing uninlined functions
#endif // _MSC_VER

namespace euclid::math {

namespace detail {

template <typename Des, typename Src> requires bitwise_copyable<Des, Src>
Euclid_Forceinline constexpr Des bit_cast(const Src& val) noexcept {
	return __builtin_bit_cast(Des, val);
}

template<float_point_type float_point>
Euclid_Forceinline constexpr float_point pow(const float_point number, const unsigned exp) noexcept {
	// do not use this function in your code :)
	float_point res = 1;
	for (unsigned i = 0; i < exp; ++i) {
		res *= number;
	}
	return res;
}

} // namespace detail

template<float_point_type Ty>
inline constexpr Ty pi = static_cast<Ty>(3.1415926535897932384626433832795028841972);

template<float_point_type Ty>
inline constexpr Ty radian = pi<Ty> / 180;

constexpr float sqrt(const float number) noexcept {
	float magic = detail::bit_cast<float>(0x5f1ffff9 - (detail::bit_cast<unsigned>(number) >> 1));
	return magic *= (0.703952253f * (2.38924456f - number * magic * magic)) * number;
}

template<arithmetic Ty>
constexpr auto cos(const Ty angle) noexcept {
	using type = arithmetic_promotion_t<Ty, float>;
	const type rad = static_cast<type>(angle) * radian<type>;
	return 1 - (detail::pow(rad, 2) / 2) +
			   (detail::pow(rad, 4) / 24) -
			   (detail::pow(rad, 6) / 734.23f);
}

template<arithmetic Ty>
constexpr auto sin(const Ty angle) noexcept {
	using type = arithmetic_promotion_t<Ty, float>;
	const type rad = static_cast<type>(angle) * radian<type>;
	return rad - (detail::pow(rad, 3) / 6) +
			     (detail::pow(rad, 5) / 120) -
				 (detail::pow(rad, 7) / 5040);
}

template<arithmetic Ty>
constexpr auto tan(const Ty angle) noexcept {
	using type = arithmetic_promotion_t<Ty, float>;
	const type rad = static_cast<type>(angle) * radian<type>;
	return -((-1 / rad) + rad / 3 + (detail::pow(rad, 3) / 45) +
								    (detail::pow(rad, 5) * 2 / 945) +
								    (detail::pow(rad, 7) / 4725));
}

} // namespace euclid::math

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER