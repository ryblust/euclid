#pragma once

#include "Core.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4514)
// enable /Wall
// C4514: remove unused inline functions
#endif

namespace euclid {

struct Float32 {
  union {
    struct {
      std::uint32_t mantissa : 23;
      std::uint32_t exponent :  8;
      std::uint32_t sign     :  1;
    } bits; // default little-endian
    float value;
  };
  constexpr Float32(const float val) noexcept : value(val) {}
};

} // namespace euclid

#ifdef _MSC_VER
#pragma warning(pop)
#endif