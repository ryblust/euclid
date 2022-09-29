#pragma once

#include "Mat2.hpp"
#include "Mat3.hpp"
#include "Mat4.hpp"
#include <cstdio>
#include <format>
#include <ostream>

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(push)
#pragma warning(disable: 4514)
#endif // _MSC_VER && !__clang__

namespace euclid {

namespace detail {

inline void print_impl(Vec2 m) noexcept {
  std::printf("[%.3f, %.3f]\n", m.x, m.y);
}

inline void print_impl(Vec3 m) noexcept {
  std::printf("[%.3f, %.3f, %.3f]\n", m.x, m.y, m.z);
}

inline void print_impl(Vec4 m) noexcept {
  std::printf("[%.3f, %.3f, %.3f, %.3f]\n",
    getVec4Data(m, 0), getVec4Data(m, 1),
    getVec4Data(m, 2), getVec4Data(m, 3));
}

inline void print_impl(Vec8 m) noexcept {
  std::printf("[%.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f]\n",
    getVec8Data(m, 0), getVec8Data(m, 1),
    getVec8Data(m, 2), getVec8Data(m, 3),
    getVec8Data(m, 4), getVec8Data(m, 5),
    getVec8Data(m, 6), getVec8Data(m, 7));
}

inline void print_impl(Mat2 m) noexcept {
  std::printf("%.3f, %.3f\n%.3f, %.3f\n",
    m(0, 0), m(0, 1),
    m(1, 0), m(1, 1));
}

inline void print_impl(Mat4 m) noexcept {
  std::printf("%.3f, %.3f, %.3f, %.3f\n%.3f, %.3f, %.3f, %.3f\n%.3f, %.3f, %.3f, %.3f\n%.3f, %.3f, %.3f, %.3f\n",
    m(0, 0), m(0, 1), m(0, 2), m(0, 3),
    m(1, 0), m(1, 1), m(1, 2), m(1, 3),
    m(2, 0), m(2, 1), m(2, 2), m(2, 3),
    m(3, 0), m(3, 1), m(3, 2), m(3, 3));
}

} // namespace euclid::detail

template<euclid_component T, euclid_component... Rest>
inline void print(T first, Rest... rest) noexcept {
  detail::print_impl(first);
  if constexpr (sizeof...(rest) > 0) {
    print(rest...);
  }
}

inline std::ostream& operator<<(std::ostream& output, euclid_component auto a) noexcept {
  detail::print_impl(a);
  return output;
}

} // namespace euclid

template<>
struct std::formatter<euclid::Vec2> {
  constexpr auto parse(auto&& context) const noexcept {
    return context.begin();
  }
  auto format(euclid::Vec2 v, auto&& context) const noexcept {
    return std::format_to(context.out(), "[{}, {}]", v.x, v.y);
  }
};

template<>
struct std::formatter<euclid::Vec3> {
  constexpr auto parse(auto&& context) const noexcept {
    return context.begin();
  }
  auto format(euclid::Vec3 v, auto&& context) const noexcept {
    return std::format_to(context.out(), "[{}, {}, {}]", v.x, v.y, v.z);
  }
};

template<>
struct std::formatter<euclid::Vec4> {
  constexpr auto parse(auto&& context) const noexcept {
    return context.begin();
  }
  auto format(euclid::Vec4 v, auto&& context) const noexcept {
    return std::format_to(context.out(), "[{}, {}, {}, {}]",
      euclid::getVec4X(v), euclid::getVec4Y(v),
      euclid::getVec4Z(v), euclid::getVec4W(v));
  }
};

template<>
struct std::formatter<euclid::Vec8> {
  constexpr auto parse(auto&& context) const noexcept {
    return context.begin();
  }
  auto format(euclid::Vec8 v, auto&& context) const noexcept {
    return std::format_to(context.out(),
      "[{}, {}, {}, {}, {}, {}, {}, {}]",
      euclid::getVec8Data(v, 0), euclid::getVec8Data(v, 1),
      euclid::getVec8Data(v, 2), euclid::getVec8Data(v, 3),
      euclid::getVec8Data(v, 4), euclid::getVec8Data(v, 5),
      euclid::getVec8Data(v, 6), euclid::getVec8Data(v, 7));
  }
};

template<>
struct std::formatter<euclid::Mat2> {
  constexpr auto parse(auto&& context) const noexcept {
    return context.begin();
  }
  auto format(euclid::Mat2 m, auto&& context) const noexcept {
    return std::format_to(context.out(),
      "|{}, {}|\n|{}, {}|",
      m(0, 0), m(0, 1),
      m(1, 0), m(1, 1));
  }
};

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(pop)
#endif // _MSC_VER && !__clang__