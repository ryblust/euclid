#pragma once

#include "Mat2.hpp"
#include "Mat3.hpp"
#include "Mat4.hpp"
#include <cstdio>
#include <ostream>

namespace euclid {

namespace detail {

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4514)
// enable /Wall
// C4514: remove unused inline functions
#endif

inline void print_impl(Vec2 m) noexcept {
  std::printf("[%.3f, %.3f]", m.x, m.y);
}

inline void print_impl(Vec3 m) noexcept {
  std::printf("[%.3f, %.3f, %.3f]", m.x, m.y, m.z);
}

inline void print_impl(Vec4 m) noexcept {
  std::printf("[%.3f, %.3f, %.3f, %.3f]",
    getVec4Data(m, 0), getVec4Data(m, 1),
    getVec4Data(m, 2), getVec4Data(m, 3));
}

inline void print_impl(Vec8 m) noexcept {
  std::printf("[%.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f]",
    getVec8Data(m, 0), getVec8Data(m, 1),
    getVec8Data(m, 2), getVec8Data(m, 3),
    getVec8Data(m, 4), getVec8Data(m, 5),
    getVec8Data(m, 6), getVec8Data(m, 7));
}

inline void print_impl(Mat2 m) noexcept {
  std::printf("%.3f, %.3f\n%.3f, %.3f",
    m(0, 0), m(0, 1),
    m(1, 0), m(1, 1));
}

inline void print_impl(Mat4 m) noexcept {
  std::printf("%.3f, %.3f, %.3f, %.3f\n%.3f, %.3f, %.3f, %.3f\n%.3f, %.3f, %.3f, %.3f\n%.3f, %.3f, %.3f, %.3f",
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
inline void print(T first, Rest... rest) noexcept {
  detail::print_impl(first);
  if constexpr (sizeof...(rest) > 0) {
    print(rest...);
  }
}

template<euclid_component T, euclid_component... Rest>
inline void println(T first, Rest... rest) noexcept {
  detail::print_impl(first);
  std::putchar('\n');
  if constexpr (sizeof...(rest) > 0) {
    println(rest...);
  }
}

inline std::ostream& operator<<(std::ostream& output, euclid_component auto a) noexcept {
  detail::print_impl(a);
  return output;
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

}