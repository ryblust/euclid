#pragma once

#include <type_traits>

#ifdef _MSC_VER
    #define EuclidForceinline [[msvc::forceinline]]
/* 
#elif __has_cpp_attribute(gnu::always_inline)
    #define EuclidForceinline [[gnu::always_inline]]
*/
#else
    #define EuclidForceinline inline __attribute__((always_inline))
#endif // EuclidForceinline

namespace euclid {

template<typename First, typename... Rest>
concept same_type = std::conjunction_v<std::is_same<First, Rest>...>;

template<typename First, typename... Rest>
concept is_any_type_of = std::disjunction_v<std::is_same<First, Rest>...>;

template<typename Ty>
concept arithmetic = std::is_arithmetic_v<Ty>;

template<typename Ty>
concept euclid_type = is_any_type_of<Ty, int, float>;

template<typename Ty>
concept integer_type = std::is_integral_v<Ty>;

template<typename Ty>
concept float_point_type = std::is_floating_point_v<Ty>;

template<arithmetic First, arithmetic... Rest>
struct arithmetic_promotion {
    using type = decltype((First{} + ... + Rest{}));
};

template<arithmetic... Ty>
using arithmetic_promotion_t = typename arithmetic_promotion<Ty...>::type;

template<typename Des, typename Src>
concept acceptable_loss = float_point_type<Des> || (integer_type<Des> && integer_type<Src>);

}