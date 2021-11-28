#pragma once
#ifndef _SORRYCONCEPTS_HPP
#define _SORRYCONCEPTS_HPP

#include <type_traits>

namespace euclid {

namespace detail {

template<typename First, typename... Rest>
inline constexpr bool is_same_type_v = std::conjunction_v<std::is_same<First, Rest>...>;

template<typename Ty, typename... Rest>
inline constexpr bool is_any_type_of_v = std::disjunction_v<std::is_same<Ty, Rest>...>;

template<typename Ty>
inline constexpr bool is_arithmetic_type_v = is_any_type_of_v<Ty, int, unsigned int, float, double>;

template<typename Ty>
inline constexpr bool is_float_point_type_v = is_any_type_of_v<Ty, float, double>;

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

template<arithmetic auto... val>
struct arithmetic_value_promotion {
    using type = decltype((... + val));
};

template<arithmetic... Ty>
using arithmetic_promotion_t = typename arithmetic_promotion<Ty...>::type;

template<arithmetic auto... val>
using arithmetic_value_promotion_t = typename arithmetic_value_promotion<val...>::type;

template<typename Des, typename Src>
concept acceptable_loss = float_point_type<Des> || same_type<Des, Src>;

}

#endif // _SORRYCONCEPTS_HPP