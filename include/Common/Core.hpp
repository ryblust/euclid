#pragma once

#include <type_traits>

#ifdef _MSC_VER
    #define Euclid_Forceinline [[msvc::forceinline]]
/* 
#elif __has_cpp_attribute(gnu::always_inline)
    #define Euclid_Forceinline [[gnu::always_inline]]
*/
#else
    #define Euclid_Forceinline inline __attribute__((always_inline))
#endif // Euclid_Forceinline

namespace euclid {

namespace detail {

template<typename First, typename... Rest>
inline constexpr bool is_same_type_v = std::conjunction_v<std::is_same<First, Rest>...>;

template<typename Ty, typename... Rest>
inline constexpr bool is_any_type_of_v = std::disjunction_v<std::is_same<Ty, Rest>...>;

template<typename Ty>
inline constexpr bool is_arithmetic_type_v = is_any_type_of_v<Ty, int, float>;

template<typename Ty>
inline constexpr bool is_float_point_type_v = is_any_type_of_v<Ty, float>;

}

template<typename First, typename... Rest>
concept same_type = detail::is_same_type_v<First, Rest...>;

// Euclid only supports arithmetic_type
template<typename Ty>
concept arithmetic = detail::is_arithmetic_type_v<Ty>;

template<typename Ty>
concept float_point_type = detail::is_float_point_type_v<Ty>;

template<arithmetic First, arithmetic... Rest>
struct arithmetic_promotion {
    using type = decltype((First{} + ... + Rest{}));
};

template<arithmetic... Ty>
using arithmetic_promotion_t = typename arithmetic_promotion<Ty...>::type;

template<typename Des, typename Src>
concept acceptable_loss = float_point_type<Des> || same_type<Des, Src>;

template<typename Des, typename Src>
concept bitwise_copyable = (sizeof(Des) == sizeof(Src)) && 
                           std::is_trivially_copyable_v<Des> && 
                           std::is_trivially_copyable_v<Src>;

}