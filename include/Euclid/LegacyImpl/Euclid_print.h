#pragma once

#include <cstdio> // Waiting for std::print

namespace euclid::io {

namespace detail {

template<euclid_type T, std::size_t S> requires (S >= 2 && S <= 4)
EUCLID_FORCEINLINE void printImpl(const T(&data)[S]) noexcept {
    if constexpr (S == 4) {
        if constexpr (same_type<T, float>) {
            std::printf("%.3f, %.3f, %.3f, %.3f", data[0], data[1], data[2], data[3]);
        } else {
            std::printf("%d, %d, %d, %d", data[0], data[1], data[2], data[3]);
        }
    } else if constexpr (S == 3) {
        if constexpr (same_type<T, float>) {
            std::printf("%.3f, %.3f, %.3f", data[0], data[1], data[2]);
        } else {
            std::printf("%d, %d, %d", data[0], data[1], data[2]);
        }
    } else if constexpr (S == 2) {
        if constexpr (same_type<T, float>) {
            std::printf("%.3f, %.3f", data[0], data[1]);
        } else {
            std::printf("%d, %d", data[0], data[1]);
        }
    }
}

}

template<util::euclid_component Container, util::euclid_component... Rest>
void print(const Container container, const Rest... rest) noexcept {
    if constexpr (is_array<Container>::value) {
        detail::printImpl(container.data);
        std::putchar('\n');
    } else if constexpr (is_point<Container>::value) {
        std::putchar('(');
        detail::printImpl(container.point.data);
        std::puts(")");
    } else if constexpr (is_vector<Container>::value) {
        std::putchar('[');
        detail::printImpl(container.vector.data);
        std::puts("]");
    } else if constexpr (is_mat2<Container>::value) {
        if constexpr (same_type<typename Container::value_type, float>) {
            std::printf("%.3f, %.3f\n%.3f, %.3f\n",
                container.mat.data[0], container.mat.data[1],
                container.mat.data[2], container.mat.data[3]);
        } else {
            std::printf("%d, %d\n%d, %d\n",
                container.mat.data[0], container.mat.data[1],
                container.mat.data[2], container.mat.data[3]);
        }
    } else if constexpr (is_mat3<Container>::value) {
        if constexpr (same_type<typename Container::value_type, float>) {
        //    std::printf("%.3f, %.3f, %.3f\n%.3f, %.3f, %.3f\n%.3f, %.3f, %.3f\n",
        //        
        //    );
        //} else {
        //    std::printf("%d, %d, %d\n%d, %d, %d\n%d, %d, %d\n",
        //        
        //    );
        }
    } else if constexpr (is_mat4<Container>::value) {
        if constexpr (same_type<typename Container::value_type, float>) {
            std::printf("%.3f, %.3f, %.3f, %.3f\n%.3f, %.3f, %.3f, %.3f\n%.3f, %.3f, %.3f, %.3f\n%.3f, %.3f, %.3f, %.3f\n",
                container.first.data[0],  container.first.data[1],
                container.first.data[2],  container.first.data[3],
                container.first.data[4],  container.first.data[5],
                container.first.data[6],  container.first.data[7],
                container.second.data[0], container.second.data[1],
                container.second.data[2], container.second.data[3],
                container.second.data[4], container.second.data[5],
                container.second.data[6], container.second.data[7]);
        } else {
            std::printf("%d, %d, %d, %d\n%d, %d, %d, %d\n%d, %d, %d, %d\n%d, %d, %d, %d\n",
                container.first.data[0],  container.first.data[1],
                container.first.data[2],  container.first.data[3],
                container.first.data[4],  container.first.data[5],
                container.first.data[6],  container.first.data[7],
                container.second.data[0], container.second.data[1],
                container.second.data[2], container.second.data[3],
                container.second.data[4], container.second.data[5],
                container.second.data[6], container.second.data[7]);
        }
    }
    if constexpr (sizeof...(rest) != 0) {
        std::putchar('\n');
        io::print(rest...);
    }
}

}