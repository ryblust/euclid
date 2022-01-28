#pragma once

#include <type_traits>

#ifdef _MSC_VER
    #define EUCLID_FORCEINLINE [[msvc::forceinline]]
/* 
#elif __has_cpp_attribute(gnu::always_inline)
    #define EUCLID_FORCEINLINE [[gnu::always_inline]]
*/
#else
    #define EUCLID_FORCEINLINE inline __attribute__((always_inline))
#endif // EUCLID_FORCEINLINE

#ifdef _MSC_VER
    #define EUCLID_CALL __vectorcall
#else
    #define EUCLID_CALL
#endif // _MSC_VER

#define EUCLID_ALIGNAS(type, size) alignas((sizeof(type) * (size)) <= 16 ? 16 : 32)

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
concept signed_type = std::is_signed_v<Ty>;

template<typename Ty>
concept float_point_type = std::is_floating_point_v<Ty>;

template<typename Ty>
concept euclid_not_support_type = !euclid_type<Ty>;

#define EUCLID_WORKAROUND(T) __builtin_is_constant_evaluated() || euclid_not_support_type<T>

template<arithmetic First, arithmetic... Rest>
struct arithmetic_promotion {
    using type = decltype((First{} + ... + Rest{}));
};

template<arithmetic... Ty>
using arithmetic_promotion_t = typename arithmetic_promotion<Ty...>::type;

template<typename Des, typename Src>
concept acceptable_loss = float_point_type<Des> || (integer_type<Des> && integer_type<Src>);

}