#pragma once

namespace euclid::io {

template<arithmetic T, std::size_t Size>
Euclid_Forceinline constexpr void print_vec(const Vector<T, Size> vec) noexcept {
	if constexpr (same_type<T, int>) {
		std::printf("[%d, %d, %d, %d]\n", vec[0], vec[1], vec[2], vec[3]);
	} else if constexpr (same_type<T, unsigned>) {
		std::printf("[%u, %u, %u, %u]\n", vec[0], vec[1], vec[2], vec[3]);
	} else {
		std::printf("[%.3f, %.3f, %.3f, %.3f]\n", vec[0], vec[1], vec[2], vec[3]);
	}
}

// Working on euclid::io::print()
template<typename... T>
Euclid_Forceinline constexpr void print(const T... obj) noexcept {
	(print_vec(obj), ...);
}

}