#pragma once
#ifndef _WEEKNDTRANSFORMATION_HPP
#define _WEEKNDTRANSFORMATION_HPP

#include "Matrix.hpp"

namespace euclid {

struct transform2d {

template<arithmetic auto angle>
static inline constexpr mat3f Rotate{
	math::cos(angle), -math::sin(angle), 0,
	math::sin(angle),  math::cos(angle), 0,
	0,                 0,                1
};

template<arithmetic auto x, arithmetic auto y>
static inline constexpr mat3<arithmetic_value_promotion_t<x, y>> Scale{
	x, 0, 0,
	0, y, 0,
	0, 0, 1 
};

template<arithmetic auto x, arithmetic auto y>
static constexpr mat3<arithmetic_value_promotion_t<x, y>> Translate{
	1, 0, x,
	0, 1, y,
	0, 0, 1
};

template<unsigned width, unsigned height>
static inline constexpr mat3u Viewport{
	width / 2,  0, width / 2,
	0, height / 2, height / 2,
	0,          0, 1
};

};

struct transform3d {

template<arithmetic auto x, arithmetic auto y, arithmetic auto z>
static inline constexpr mat4<arithmetic_value_promotion_t<x, y, z>> Translate{
	1, 0, 0, x,
	0, 1, 0, y,
	0, 0, 1, z,
	0, 0, 0, 1
};

template<arithmetic auto x, arithmetic auto y, arithmetic auto z>
static inline constexpr mat4<arithmetic_value_promotion_t<x, y, z>> Scale{
	x, 0, 0, 0,
	0, y, 0, 0,
	0, 0, z, 0,
	0, 0, 0, 1
};
 
template<arithmetic auto fov, arithmetic auto aspectRatio, arithmetic auto zNear, arithmetic auto zFar>
static inline constexpr mat4f Perspective{
    zNear / (aspectRatio * math::tan((fov / 2 * math::radian<float>))), 0, 0, 0,
	0, zNear / math::tan((fov / 2 * math::radian<float>)), 0, 0,
	0, 0, (zFar + zNear) / (zNear - zFar), 2 * zNear * zFar / (zFar - zNear),
	0, 0, 1, 0
};

template<unsigned width, unsigned height>
static inline constexpr mat4u Viewport{
	width / 2,  0, 0, width / 2,
	0, height / 2, 0, height / 2,
	0, 0,          1, 0,
	0, 0,          0, 1
};

};

}

#endif // _WEEKNDTRANSFORMATION_HPP