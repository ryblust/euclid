#pragma once

#include <limits>
#include <type_traits>
#include <immintrin.h>

#if defined(_MSC_VER) || defined(__clang__)
    #define EUCLID_CALL __vectorcall
#else
    #define EUCLID_CALL
#endif

#if defined(_MSC_VER) && !defined(__clang__)
    #define EUCLID_DISABLE_MSVC_SAFEBUFFERS __declspec(safebuffers)
    #define EUCLID_UNREACHABLE() __assume(false)
#else
    #define EUCLID_CALL
    #define EUCLID_DISABLE_MSVC_SAFEBUFFERS
    #define EUCLID_UNREACHABLE() __builtin_unreachable()
#endif