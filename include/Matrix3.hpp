#pragma once

namespace euclid {

template<euclid_type Type>
struct Mat3 {


};

template<euclid_type First, euclid_type... Rest> requires same_type<First, Rest...>
Mat3(First, Rest...)->Mat3<First>;

using mat3i = Mat3<int>;
using mat3f = Mat3<float>;

template<typename T>
struct is_mat3 {
    static constexpr bool value = false;
};

template<euclid_type T>
struct is_mat3<Mat3<T>> {
    static constexpr bool value = true;
};

}