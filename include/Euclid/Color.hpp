#pragma once

#include <cstdint>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4201)
// enable: /Wall
// C4201:  use the non-standard extension anonymous union
#endif

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

#ifdef _MSC_VER
#pragma warning(pop)
#endif