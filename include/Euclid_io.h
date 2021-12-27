#pragma once

#include <cstdio>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 5045) // Ignore compiler's complain about Qspectre
#endif // _MSC_VER

namespace euclid::io {

namespace detail {

template<arithmetic T>
Euclid_Forceinline void printContainerImpl(const T* data, std::size_t size) noexcept {
	if constexpr (same_type<T, int>) {
		std::printf("[");
		for (std::size_t i = 0; i < size - 1; ++i) {
			std::printf("%d, ", data[i]);
		}
		std::printf("%d]\n", data[size - 1]);
	} else {
		std::printf("[");
		for (std::size_t i = 0; i < size - 1; ++i) {
			std::printf("%.3f, ", data[i]);
		}
		std::printf("%.3f]\n", data[size - 1]);
	}
}

template<arithmetic T>
Euclid_Forceinline void printImpl(const T value) noexcept {
	if constexpr (same_type<T, int>) {
		std::printf("%d ", value);
	} else {
		std::printf("%.3f ", value);
	}
}

template<arithmetic T>
Euclid_Forceinline void printlnImpl(const T value) noexcept {
	if constexpr (same_type<T, int>) {
		std::printf("%d\n", value);
	}
	else {
		std::printf("%.3f\n", value);
	}
}

}

template<typename... Container> 
Euclid_Forceinline void print(const Container... container) noexcept {
	(detail::printContainerImpl((typename Container::value_type*)__builtin_addressof(container), container.size()), ...);
}

template<arithmetic... T>
Euclid_Forceinline void print(const T... value) noexcept {
	(detail::printImpl(value), ...);
}

template<arithmetic... T>
Euclid_Forceinline void println(const T... value) noexcept {
	(detail::printlnImpl(value), ...);
}

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER