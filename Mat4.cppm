module;
#include "Core.hpp"

export module Euclid.Mat4;
export import Euclid.Vec4;

export namespace Euclid
{
    struct alignas(32) Mat4
    {
        static constexpr Mat4 EUCLID_CALL Identity() noexcept
        {
            return Mat4
            {
                1, 0, 0, 0,
                0, 1, 0, 0, 
                0, 0, 1, 0,
                0, 0, 0, 1
            };
        }

        constexpr float& operator()(std::size_t row, std::size_t col) noexcept
        {
            if (std::is_constant_evaluated())
            {
            }

            return *(reinterpret_cast<float*>(this) + 4 * row + col);
        }

        constexpr float operator()(std::size_t row, std::size_t col) const noexcept
        {
            return const_cast<Mat4*>(this)->operator()(row, col);
        }

#ifdef __clang__
        union {
            float forconstexpr[16];
            __m256 data[2];
        };
#else
        __m256 data[2];
#endif
    };
}

using namespace Euclid;

export
{

}