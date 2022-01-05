#pragma once

namespace euclid::util {

template<arithmetic T>
EuclidForceinline constexpr void swap(T& left, T& right) noexcept {
	T temp = left;
	left   = right;
	right  = temp;
}

template<arithmetic T, std::size_t S, typename Lambda> requires (S >= 2 && S <= 4)
EuclidForceinline constexpr void transformToArray(T(&array)[S], Lambda transform) noexcept {
	if constexpr (S == 4) {
		transform(array[0]);
		transform(array[1]);
		transform(array[2]);
		transform(array[3]);
	} else if constexpr (S == 3) {
		transform(array[0]);
		transform(array[1]);
		transform(array[2]);
	} else {
		transform(array[0]);
		transform(array[1]);
	}
}

template<arithmetic T, std::size_t S, typename Lambda> requires (S >= 2 && S <= 4)
EuclidForceinline constexpr void transformArrayFromOther(T(&dst)[S], const T(&src)[S], Lambda transform) noexcept {
	if constexpr (S == 4) {
		transform(dst[0], src[0]);
		transform(dst[1], src[1]);
		transform(dst[2], src[2]);
		transform(dst[3], src[3]);
	} else if constexpr (S == 3) {
		transform(dst[0], src[0]);
		transform(dst[1], src[1]);
		transform(dst[2], src[2]);
	} else {
		transform(dst[0], src[0]);
		transform(dst[1], src[1]);
	}
}

template<arithmetic T, std::size_t S, typename Lambda> requires (S >= 2 && S <= 4)
EuclidForceinline constexpr void transformFromTwoArray(const T(&a)[S], const T(&b)[S], Lambda transform) noexcept {
	if constexpr (S == 4) {
		transform(a[0], b[0]);
		transform(a[1], b[1]);
		transform(a[2], b[2]);
		transform(a[3], b[3]);
	} else if constexpr (S == 3) {
		transform(a[0], b[0]);
		transform(a[1], b[1]);
		transform(a[2], b[2]);
	} else {
		transform(a[0], b[0]);
		transform(a[1], b[1]);
	}
}

}