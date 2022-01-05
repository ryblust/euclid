#pragma once

#include <cstdio>

namespace euclid::io {

namespace detail {

template<arithmetic T>
EuclidForceinline void printContainerImpl(const T* data, std::size_t size) noexcept {
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
EuclidForceinline void printImpl(const T value) noexcept {
	if constexpr (same_type<T, int>) {
		std::printf("%d ", value);
	} else {
		std::printf("%.3f ", value);
	}
}

template<arithmetic T>
EuclidForceinline void printlnImpl(const T value) noexcept {
	if constexpr (same_type<T, int>) {
		std::printf("%d\n", value);
	} else {
		std::printf("%.3f\n", value);
	}
}

}

template<typename... Container> 
EuclidForceinline void print(const Container... container) noexcept {
	(detail::printContainerImpl((typename Container::value_type*)__builtin_addressof(container), container.size()), ...);
}

template<arithmetic... T>
EuclidForceinline void print(const T... value) noexcept {
	(detail::printImpl(value), ...);
}

template<arithmetic... T>
EuclidForceinline void println(const T... value) noexcept {
	(detail::printlnImpl(value), ...);
}

}