#pragma once

#include "Core.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4514)
// enable /Wall
// C4514 : Ignore the compiler's warning about removing unused inline functions
#endif

namespace euclid {

class Float32 {
public:
	union {
		struct {
			std::uint32_t mantissa : 23;
			std::uint32_t exponent : 8;
			std::uint32_t sign     : 1;
		} bits; // Default Little-endian
		float value;
	};
    constexpr Float32(const float val) noexcept : value(val) {}
};

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif