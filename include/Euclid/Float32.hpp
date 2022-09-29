#pragma once

#include "Core.hpp"
#include <bit>

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(push)
#pragma warning(disable: 4514)
#endif // _MSC_VER && !__clang__

namespace euclid {

namespace detail {

template<std::endian>
struct float_bits {};

template<>
struct float_bits<std::endian::little> {
  std::uint32_t mantissa : 23;
  std::uint32_t exponent :  8;
  std::uint32_t sign     :  1;
};

template<>
struct float_bits<std::endian::big> {
  std::uint32_t sign     :  1;
  std::uint32_t exponent :  8;
  std::uint32_t mantissa : 23;
};

} // namespace euclid::detail

struct Float32 {
  constexpr Float32() noexcept = default;

  constexpr Float32(float val) noexcept : value(val) {}

  using float_bits = detail::float_bits<std::endian::native>;
  
  union {
    float_bits bits;
    float value;
  };
};

} // namespace euclid

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(pop)
#endif // _MSC_VER && !__clang__