#pragma once

#include "Core.h"
#include <cstdio>

namespace euclid::debug {

namespace detail {

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4514)
// enable /Wall
// C4514: remove unused inline functions
#endif

inline void printImpl(const Vec2 m) noexcept {
    std::printf("[%.3f, %.3f]\n", m.x, m.y);
}

inline void printImpl(const Vec3 m) noexcept {
    std::printf("[%.3f, %.3f, %.3f]\n", m.x, m.y, m.z);
}

inline void printImpl(const Vec4 m) noexcept {
    std::printf("[%.3f, %.3f, %.3f, %.3f]\n",
        getVec4Data(m, 0), getVec4Data(m, 1),
        getVec4Data(m, 2), getVec4Data(m, 3));
}

inline void printImpl(const Vec8 m) noexcept {
    std::printf("[%.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f]\n",
        getVec8Data(m, 0), getVec8Data(m, 1),
        getVec8Data(m, 2), getVec8Data(m, 3),
        getVec8Data(m, 4), getVec8Data(m, 5),
        getVec8Data(m, 6), getVec8Data(m, 7));
}

inline void printImpl(const Mat2 m) noexcept {
    std::printf("%.3f, %.3f\n%.3f, %.3f\n",
        m(0, 0), m(0, 1),
        m(1, 0), m(1, 1));
}

inline void printImpl(const Mat4 m) noexcept {
    std::printf("%.3f, %.3f, %.3f, %.3f\n%.3f, %.3f, %.3f, %.3f\n%.3f, %.3f, %.3f, %.3f\n%.3f, %.3f, %.3f, %.3f\n",
        m(0, 0), m(0, 1), m(0, 2), m(0, 3),
        m(1, 0), m(1, 1), m(1, 2), m(1, 3),
        m(2, 0), m(2, 1), m(2, 2), m(2, 3),
        m(3, 0), m(3, 1), m(3, 2), m(3, 3));
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

}

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif

template<euclid_component T, euclid_component... Rest>
inline void print(const T first, const Rest... rest) noexcept {
    detail::printImpl(first);
    if constexpr (sizeof...(rest) > 0) {
        print(rest...);
    }
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

}
