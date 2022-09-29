#pragma once

#include "Core.hpp"

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(push)
#pragma warning(disable: 4514 5246)
#endif // _MSC_VER && !__clang__

namespace euclid {

EUCLID_QUALIFIER double getVec4Data(const Vec4& a, std::size_t i) noexcept {
  #if defined(_MSC_VER) && !defined(__clang__)
    return a.m256_f64[i];
  #else // __clang__ || __GNUC__
    return a.v[i];
  #endif
}

EUCLID_QUALIFIER double& getVec4dData(Vec4d& a, std::size_t i) noexcept {
#if defined(_MSVC_VER) && !defined (__clang__)
  return a.m256_f64[i];
#elif __clang__
  return *(reinterpret_cast<double*>(&a) + i);
#else // __GNUC__
  return a.v[i];
#endif
}

} // namespace euclid

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(pop)
#endif // _MSC_VER && !__clang__