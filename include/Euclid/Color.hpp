#pragma once

#include "Core.hpp"
#include <cstdint>

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(push)
#pragma warning(disable: 4201)
#endif // _MSC_VER && !__clang__

namespace euclid {

enum class GammaSpace : std::uint8_t {
  Linear, sRGB
};

struct LinearColor {
  float r, g, b, a;
};

struct Color {
  std::uint8_t r, g, b, a;
};

} // namespace euclid

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(pop)
#endif // _MSC_VER && !__clang__