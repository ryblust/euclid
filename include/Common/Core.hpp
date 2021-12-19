#pragma once

#ifdef _MSC_VER
    #define Euclid_Forceinline [[msvc::forceinline]]
#elif __has_cpp_attribute(gnu::always_inline)
    #define Euclid_Force_Inline [[gnu::always_inline]]
#else
    #define Euclid_Force_Inline inline __attribute__((always_inline))
#endif // Euclid_Forceinline

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

template<arithmetic... Ty>
using arithmetic_promotion_t = typename arithmetic_promotion<Ty...>::type;

template<typename Ty>
concept signed_type = detail::is_any_type_of_v<Ty, int, float, double>;

template<typename Des, typename Src>
concept acceptable_loss = signed_type<Des> && same_type<Des, arithmetic_promotion_t<Des, Src>>;

template<typename Ty, typename CastT>
concept promotion_cast = acceptable_loss<CastT, Ty> && !same_type<Ty, CastT>;

template<typename Des, typename Src>
concept acceptable_loss_basic = float_point_type<Des> || same_type<Des, Src>;

}