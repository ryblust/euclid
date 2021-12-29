#pragma once

namespace euclid {

struct transform3d {

template<arithmetic T>
static constexpr mat3<arithmetic_promotion_t<T, float>> rotate(const T angle) noexcept {
	return {
		math::cos(angle), -math::sin(angle), 0,
		math::sin(angle),  math::cos(angle), 0,
		      0,                 0,          1
	};
}

template<arithmetic T, arithmetic S>
static constexpr auto scale(const T x, const S y) noexcept {
	using type = arithmetic_promotion_t<T, S>;
	return mat3<type> {
		static_cast<type>(x),          0,            0,
		         0,           static_cast<type>(y),  0,
		         0,                    0,            1
	};
}

template<arithmetic T, arithmetic S>
static constexpr auto translate(const T x, const S y) noexcept {
	using type = arithmetic_promotion_t<T, S>;
	return mat3<type> {
		1, 0, static_cast<type>(x),
		0, 1, static_cast<type>(y),
		0, 0,          1
	};
}

static constexpr mat3i viewport(const unsigned width, const unsigned height) noexcept {
	return {
		width / 2,      0,      width / 2,
		    0,     height / 2, height / 2,
		    0,          0,          1
	};
}

};

struct transform4d {

template<arithmetic X, arithmetic Y, arithmetic Z>
static constexpr auto translate(const X x, const Y y, const Z z) noexcept {
	using type = arithmetic_promotion_t<X, Y, Z>;
	return mat4<type> { 
		1, 0, 0, static_cast<type>(x),
		0, 1, 0, static_cast<type>(y),
		0, 0, 1, static_cast<type>(z),
		0, 0, 0,          1
	};
};

template<arithmetic X, arithmetic Y, arithmetic Z>
static constexpr auto scale(const X x, const Y y, const Z z) noexcept {
	using type = arithmetic_promotion_t<X, Y, Z>;
	return mat4<type> {
		static_cast<type>(x),          0,                    0,           0,
			     0,           static_cast<type>(y),          0,           0,
				 0,                    0,           static_cast<type>(z), 0,
		         0,                    0,                    0,           1
	};
};

template<arithmetic T, arithmetic Angle>
static constexpr auto rotate(const vec3<T>& axis, const Angle angle) noexcept {
	Matrix<T, 4, 1> axisMat{ axis[0], axis[1], axis[2], axis[3] };
	return mat4<arithmetic_promotion_t<Angle, float>>::identity(math::cos(angle)) + (1 - math::cos(angle)) * (axisMat * axisMat.transpose()) +
		   math::sin(angle) * mat4<T>{     0,    -axis[2],  axis[1], 0,
									    axis[2],     0,    -axis[0], 0,
									   -axis[1],  axis[0],     0,    0,
										   0,        0,        0,    1 };
}

template<arithmetic Fov, arithmetic AspectRatio, arithmetic ZNear, arithmetic ZFar>
static constexpr auto perspective(const Fov fov, const AspectRatio aspectRatio, const ZNear zNear, const ZFar zFar) noexcept {
	using type = arithmetic_promotion_t<Fov, AspectRatio, ZNear, ZFar, float>;
	const auto top = math::tan(fov / 2);
	const auto right = top * aspectRatio;
	return mat4<type> {
		static_cast<type>(zNear / right),              0,                                            0,                                                  0,
	                 0,					  static_cast<type>(zNear / top),                            0,                                                  0,
			         0,                                0,				  static_cast<type>((zFar + zNear) / (zNear - zFar)),  static_cast<type>(2 * zNear * zFar / (zFar - zNear)),
			         0,                                0,                                            1,                                                  0
	};
}

static constexpr mat4i viewport(unsigned width, unsigned height) noexcept {
	return {
		width / 2,      0,      0,   width / 2,
		    0,     height / 2,  0,  height / 2,
			0,          0,      1,       0,
		    0,          0,      0,       1
	};
}

};

}