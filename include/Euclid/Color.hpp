#pragma once

#include "Core.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4201)
// C4201 : Use the non-standard extension : anonymous union
#endif

namespace euclid {

enum class GammaSpace : std::uint8_t { Linear, sRGB };

struct LinearColor {
    float r,g,b,a;
};

struct Color {
    union {
        struct {
            std::uint8_t r,g,b,a;
        };
        std::uint32_t alignment;
    };
};

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif