#pragma once

#include "Vec4.hpp"
#include <cstdint>

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(push)
#pragma warning(disable: 4514)
#endif // _MSC_VER && !__clang__

namespace euclid {

namespace detail {

template<std::size_t X, std::size_t Y, std::size_t Z, std::size_t W>
struct PermuteHelper {

  static EUCLID_CONSTEXPR Vec4 EUCLID_CALL default_permute(Vec4 v) noexcept {
#ifndef __clang__
    if (std::is_constant_evaluated()) {
      return {
        getVec4Data(v, X),
        getVec4Data(v, Y),
        getVec4Data(v, Z),
        getVec4Data(v, W)
      };
    }
#endif
    return { _mm_permute_ps(v, _MM_SHUFFLE(W, Z, Y, X)) };
  }

  static EUCLID_CONSTEXPR Vec4 EUCLID_CALL permute(Vec4 v) noexcept {
#ifndef __clang__
    if (std::is_constant_evaluated()) {
      return default_permute(v);
    }
#endif
    if constexpr (is_index_match<0, 1, 2, 3>) {
      return v;
    } else if constexpr (is_index_match<0, 1, 0, 1>) {
      return { _mm_movelh_ps(v, v) };
    } else if constexpr (is_index_match<2, 3, 2, 3>) {
      return { _mm_movehl_ps(v, v) };
    } else if constexpr (is_index_match<0, 0, 1, 1>) {
      return { _mm_unpacklo_ps(v, v) };
    } else if constexpr (is_index_match<2, 2, 3, 3>) {
      return { _mm_unpackhi_ps(v, v) };
    } else if constexpr (is_index_match<0, 0, 2, 2>) {
      return { _mm_moveldup_ps(v) }; // sse3
    } else if constexpr (is_index_match<1, 1, 3, 3>) {
      return { _mm_movehdup_ps(v) }; // sse3
    } else if constexpr (is_index_match<0, 0, 0, 0>) {
      return { _mm_broadcastss_ps(v) }; // avx2
    }
    return default_permute(v);
  }

  template<std::size_t x, std::size_t y, std::size_t z, std::size_t w>
  static constexpr bool is_index_match = 
    std::is_same_v<PermuteHelper, PermuteHelper<x, y, z, w>>;
};

template<std::size_t X, std::size_t Y, std::size_t Z, std::size_t W>
struct ShuffleHelper {

  template<std::uint8_t Shuffle, bool WhichX, bool WhichY, bool WhichZ, bool WhichW>
  static Vec4 EUCLID_CALL shuffle_helper(Vec4 a, Vec4 b) noexcept {
    static constexpr __m128 mask {
      WhichX ? 0xffffffff : 0,
      WhichY ? 0xffffffff : 0,
      WhichZ ? 0xffffffff : 0,
      WhichW ? 0xffffffff : 0
    };
    const __m128 shuffled1 = _mm_permute_ps(a, Shuffle);
    const __m128 shuffled2 = _mm_permute_ps(b, Shuffle);
    const __m128 m128mask1 = _mm_andnot_ps(mask, shuffled1);
    const __m128 m128mask2 = _mm_and_ps(mask, shuffled2);
    return { _mm_or_ps(m128mask1, m128mask2) };
  }

  static EUCLID_CONSTEXPR Vec4 EUCLID_CALL default_shuffle(Vec4 a, Vec4 b) noexcept {
#ifndef __clang__
    if (std::is_constant_evaluated()) {
      auto select = []<std::size_t I>(Vec4 v1, Vec4 v2) {
        if constexpr (I < 4) {
          return getVec4Data(v1, I);
        }
        return getVec4Data(v2, I - 4);
      };
      return {
        select.template operator()<X>(a, b),
        select.template operator()<Y>(a, b),
        select.template operator()<Z>(a, b),
        select.template operator()<W>(a, b)
      };
    }
#endif
    constexpr bool x = X > 3;
    constexpr bool y = Y > 3;
    constexpr bool z = Z > 3;
    constexpr bool w = W > 3;
    return shuffle_helper<_MM_SHUFFLE(W, Z, Y, X), x, y, z, w>(a, b);
  }

  // static EUCLID_CONSTEXPR Vec4 EUCLID_CALL shuffle(Vec4 a, Vec4 b) noexcept {
  //
  // }
};

} // namespace euclid::detail

template<std::size_t X, std::size_t Y, std::size_t Z, std::size_t W>
  requires(X < 4 && Y < 4 && Z < 4 && W < 4)
EUCLID_QUALIFIER Vec4 EUCLID_CALL permute(Vec4 v) noexcept {
  return detail::PermuteHelper<X, Y, Z, W>::permute(v);
}

template<std::size_t X, std::size_t Y, std::size_t Z, std::size_t W>
  requires(X < 8 && Y < 8 && Z < 8 && W < 8)
EUCLID_QUALIFIER Vec4 EUCLID_CALL shuffle(Vec4 a, Vec4 b) noexcept {
  return detail::ShuffleHelper<X, Y, Z, W>::shuffle(a, b);
}

// template<std::size_t X, std::size_t Y, std::size_t Z, std::size_t W>
//     requires(X < 8 && Y < 8 && Z < 8 && W < 8)
// EUCLID_QUALIFIER Vec4 EUCLID_CALL shuffle(Vec4 a,Vec4 b) noexcept {
//     if (std::is_constant_evaluated()) {
//         auto select = []<std::size_t I>(Vec4 v1, Vec4 v2) {
//             if constexpr (I < 4) {
//                 return getVec4Data(v1, I);
//             }
//             return getVec4Data(v2, I - 4);
//         };
//         return {
//             select.template operator()<X>(a, b),
//             select.template operator()<Y>(a, b),
//             select.template operator()<Z>(a, b),
//             select.template operator()<W>(a, b)
//         };
//     }
//     static constexpr auto x = X % 4;
//     static constexpr auto y = Y % 4;
//     static constexpr auto z = Z % 4;
//     static constexpr auto w = W % 4;
//     return { _mm_shuffle_ps(a, b, _MM_SHUFFLE(w, z, y, x)) };
// }

} // namespace euclid

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(pop)
#endif // _MSC_VER && !__clang__