#pragma once

#include "Core.h"

namespace euclid {

enum class GammaSpace : std::uint8_t { Linear, sRGB };

struct alignas(16) LinearColor {
    
    float r,g,b,a;
};

struct alignas(8) Color {


    union {
        struct {
            std::uint8_t r,g,b,a;
        };
        std::uint32_t alignment;
    };
};

}