#pragma once

#include "Core.hpp"

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(push)
#pragma warning(disable: 4514 5246)
#endif // _MSC_VER && !__clang__

namespace euclid {

#if defined(_MSC_VER) && !defined(__clang__)
using __m256 = Vec8;
#else
struct alignas(32) Vec4d final {
  constexpr EUCLID_CALL operator __m256d() const noexcept {
    return data;
  }
  __m256d data;
};
#endif // _MSC_VER && !__clang__

EUCLID_QUALIFIER double getVec4dData(const Vec4d& a, std::size_t i) noexcept {
#if defined(_MSC_VER) && !defined(__clang__)
  return a.m256_f64[i];
#else // __clang__ || __GNUC__
  return a.data[i];
#endif
}

EUCLID_QUALIFIER double& getVec4dData(Vec4d& a, std::size_t i) noexcept {
#if defined(_MSVC_VER) && !defined (__clang__)
  return a.m256_f64[i];
#elif __clang__
  return *(reinterpret_cast<double*>(&a) + i);
#else // __GNUC__
  return a.data[i];
#endif
}

} // namespace euclid

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(pop)
#endif // _MSC_VER && !__clang__