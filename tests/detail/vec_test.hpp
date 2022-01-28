#pragma once

#include "Euclid/Print.hpp"
#include "Euclid/Vec4.hpp"

namespace euclid::test_case::vec {

inline void test_set_vec() noexcept {
    vec4i v4i = setVec4i(1, 2, 3, 4);
    vec4f v4f = setVec4f(1, 2, 3, 4);
    vec4d v4d = setVec4d(1, 2, 3, 4);
    vec8i v8i = setVec8i(1, 2, 3, 4, 5, 6, 7, 8);
    vec8f v8f = setVec8f(1, 2, 3, 4, 5, 6, 7, 8);
    constexpr vec4i cv4i = setVec4i(1, 2, 3, 4);
    constexpr vec4f cv4f = setVec4f(1, 2, 3, 4);
    constexpr vec4d cv4d = setVec4d(1, 2, 3, 4);
    constexpr vec8i cv8i = setVec8i(1, 2, 3, 4, 5, 6, 7, 8);
    constexpr vec8f cv8f = setVec8f(1, 2, 3, 4, 5, 6, 7, 8);
    debug::printVec(v4i, v4f, v4d, v8i, v8f);
    std::putchar('\n');
}

inline void test_set1_vec() noexcept {
    vec4i v4i  = set1Vec4i(1);
    vec4f v4f  = set1Vec4f(2);
    vec4d v4d  = set1Vec4d(3);
    vec8i v8i  = set1Vec8i(4);
    vec8f v8f  = set1Vec8f(5);
    vec4i nv4i = set1Vec4i(-1);
    vec4f nv4f = set1Vec4f(-2);
    vec4d nv4d = set1Vec4d(-3);
    vec8i nv8i = set1Vec8i(-4);
    vec8f nv8f = set1Vec8f(-5);
    constexpr vec4i cv4i  = set1Vec4i(1);
    constexpr vec4f cv4f  = set1Vec4f(2);
    constexpr vec4d cv4d  = set1Vec4d(3);
    constexpr vec8i cv8i  = set1Vec8i(4);
    constexpr vec8f cv8f  = set1Vec8f(5);
    constexpr vec4i cnv4i = set1Vec4i(-1);
    constexpr vec4f cnv4f = set1Vec4f(-2);
    constexpr vec4d cnv4d = set1Vec4d(-3);
    constexpr vec8i cnv8i = set1Vec8i(-4);
    constexpr vec8f cnv8f = set1Vec8f(-5);
    debug::printVec(v4i,  v4f,  v4d,  v8i,  v8f);
    std::putchar('\n');
}

inline void test_add_vec() noexcept {
    vec4i v4i = setVec4i(1, 2, 3, 4);
    vec4f v4f = setVec4f(1, 2, 3, 4);
    vec4d v4d = setVec4d(1, 2, 3, 4);
    vec8i v8i = setVec8i(1, 2, 3, 4, 5, 6, 7, 8);
    vec8f v8f = setVec8f(1, 2, 3, 4, 5, 6, 7, 8);
    vec4i resv4i = addVec4i(v4i, v4i);
    vec4f resv4f = addVec4f(v4f, v4f);
    vec4d resv4d = addVec4d(v4d, v4d);
    vec8i resv8i = addVec8i(v8i, v8i);
    vec8f resv8f = addVec8f(v8f, v8f);
    constexpr vec4i cv4i = setVec4i(1, 2, 3, 4);
    constexpr vec4f cv4f = setVec4f(1, 2, 3, 4);
    constexpr vec4d cv4d = setVec4d(1, 2, 3, 4);
    constexpr vec8i cv8i = setVec8i(1, 2, 3, 4, 5, 6, 7, 8);
    constexpr vec8f cv8f = setVec8f(1, 2, 3, 4, 5, 6, 7, 8);
    constexpr vec4i cresv4i = addVec4i(cv4i, cv4i);
    constexpr vec4f cresv4f = addVec4f(cv4f, cv4f);
    constexpr vec4d cresv4d = addVec4d(cv4d, cv4d);
    constexpr vec8i cresv8i = addVec8i(cv8i, cv8i);
    constexpr vec8f cresv8f = addVec8f(cv8f, cv8f);
    debug::printVec(resv4i, resv4f, resv4d, resv8i, resv8f);
    std::putchar('\n');
}

inline void test_sub_vec() noexcept {
    vec4i v4i = setVec4i(1, 2, 3, 4);
    vec4f v4f = setVec4f(1, 2, 3, 4);
    vec4d v4d = setVec4d(1, 2, 3, 4);
    vec8i v8i = setVec8i(1, 2, 3, 4, 5, 6, 7, 8);
    vec8f v8f = setVec8f(1, 2, 3, 4, 5, 6, 7, 8);
    vec4i resv4i = subVec4i(v4i, v4i);
    vec4f resv4f = subVec4f(v4f, v4f);
    vec4d resv4d = subVec4d(v4d, v4d);
    vec8i resv8i = subVec8i(v8i, v8i);
    vec8f resv8f = subVec8f(v8f, v8f);
    constexpr vec4i cv4i = setVec4i(1, 2, 3, 4);
    constexpr vec4f cv4f = setVec4f(1, 2, 3, 4);
    constexpr vec4d cv4d = setVec4d(1, 2, 3, 4);
    constexpr vec8i cv8i = setVec8i(1, 2, 3, 4, 5, 6, 7, 8);
    constexpr vec8f cv8f = setVec8f(1, 2, 3, 4, 5, 6, 7, 8);
    constexpr vec4i cresv4i = subVec4i(cv4i, cv4i);
    constexpr vec4f cresv4f = subVec4f(cv4f, cv4f);
    constexpr vec4d cresv4d = subVec4d(cv4d, cv4d);
    constexpr vec8i cresv8i = subVec8i(cv8i, cv8i);
    constexpr vec8f cresv8f = subVec8f(cv8f, cv8f);
    debug::printVec(resv4i, resv4f, resv4d, resv8i, resv8f);
    std::putchar('\n');
}

inline void test_mul_vec() noexcept {
    vec4i v4i = setVec4i(1, 2, 3, 4);
    vec4f v4f = setVec4f(1, 2, 3, 4);
    vec4d v4d = setVec4d(1, 2, 3, 4);
    vec8i v8i = setVec8i(1, 2, 3, 4, 5, 6, 7, 8);
    vec8f v8f = setVec8f(1, 2, 3, 4, 5, 6, 7, 8);
    vec4i resv4i = mulVec4i(v4i, v4i);
    vec4f resv4f = mulVec4f(v4f, v4f);
    vec4d resv4d = mulVec4d(v4d, v4d);
    vec8i resv8i = mulVec8i(v8i, v8i);
    vec8f resv8f = mulVec8f(v8f, v8f);
    constexpr vec4i cv4i = setVec4i(1, 2, 3, 4);
    constexpr vec4f cv4f = setVec4f(1, 2, 3, 4);
    constexpr vec4d cv4d = setVec4d(1, 2, 3, 4);
    constexpr vec8i cv8i = setVec8i(1, 2, 3, 4, 5, 6, 7, 8);
    constexpr vec8f cv8f = setVec8f(1, 2, 3, 4, 5, 6, 7, 8);
    constexpr vec4i cresv4i = mulVec4i(cv4i, cv4i);
    constexpr vec4f cresv4f = mulVec4f(cv4f, cv4f);
    constexpr vec4d cresv4d = mulVec4d(cv4d, cv4d);
    constexpr vec8i cresv8i = mulVec8i(cv8i, cv8i);
    constexpr vec8f cresv8f = mulVec8f(cv8f, cv8f);
    debug::printVec(resv4i, resv4f, resv4d, resv8i, resv8f);
    std::putchar('\n');
}

inline void test_div_vec() noexcept {
    vec4i v4i = setVec4i(1, 2, 3, 4);
    vec4f v4f = setVec4f(1, 2, 3, 4);
    vec4d v4d = setVec4d(1, 2, 3, 4);
    vec8i v8i = setVec8i(1, 2, 3, 4, 5, 6, 7, 8);
    vec8f v8f = setVec8f(1, 2, 3, 4, 5, 6, 7, 8);
    vec4i resv4i = divVec4i(v4i, v4i);
    vec4f resv4f = divVec4f(v4f, v4f);
    vec4d resv4d = divVec4d(v4d, v4d);
    vec8i resv8i = divVec8i(v8i, v8i);
    vec8f resv8f = divVec8f(v8f, v8f);
    constexpr vec4i cv4i = setVec4i(1, 2, 3, 4);
    constexpr vec4f cv4f = setVec4f(1, 2, 3, 4);
    constexpr vec4d cv4d = setVec4d(1, 2, 3, 4);
    constexpr vec8i cv8i = setVec8i(1, 2, 3, 4, 5, 6, 7, 8);
    constexpr vec8f cv8f = setVec8f(1, 2, 3, 4, 5, 6, 7, 8);
    constexpr vec4i cresv4i = divVec4i(cv4i, cv4i);
    constexpr vec4f cresv4f = divVec4f(cv4f, cv4f);
    constexpr vec4d cresv4d = divVec4d(cv4d, cv4d);
    constexpr vec8i cresv8i = divVec8i(cv8i, cv8i);
    constexpr vec8f cresv8f = divVec8f(cv8f, cv8f);
    debug::printVec(resv4i, resv4f, resv4d, resv8i, resv8f);
    std::putchar('\n');
}

inline void test_four_operation() noexcept {
    test_add_vec();
    test_sub_vec();
    test_mul_vec();
    test_div_vec();
}

inline void test_all() noexcept {
    test_set_vec();
    test_set1_vec();
    test_four_operation();
}

}