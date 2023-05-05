#pragma once

#if defined(_MSC_VER) && !defined(__clang__)
    #define EUCLID_CALL __vectorcall
    #define EUCLID_UNREACHABLE() __assume(false)
    #define EUCLID_DISABLE_MSVC_SAFEBUFFERS __declspec(safebuffers)
#else // defined(__GNUC__) || defined(__clang__)
    #define EUCLID_CALL
    #define EUCLID_UNREACHABLE() __builtin_unreachable()
    #define EUCLID_DISABLE_MSVC_SAFEBUFFERS
#endif // defined(_MSC_VER) && !defined(__clang__)