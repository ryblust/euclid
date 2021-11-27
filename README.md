# Eucild - a Graphics Mathematics Library
> weeknd's Graphics Mathematics Library

## Overview
**Euclid** 是一个基于 C++20 的图形数学库，提供了完整的编译期计算支持和严格的类型约束

**Features**
- 编译期的类型约束
- ```SIMD```(To-do)
- 面向对象
- Head-only
- 齐次坐标下的```transformation```

## Introduction
- 编译期的类型约束

```c++
template<typename Ty, typename... Rest>
inline constexpr bool is_any_type_of_v = std::disjunction_v<std::is_same<Ty, Rest>...>;

template<typename Ty>
inline constexpr bool is_arithmetic_type_v = is_any_type_of_v<Ty, int, unsigned int, float, double>;

template<typename Ty>
concept arithmetic = detail::is_arithmetic_type_v<Ty>;
```

- 自动算术类型提升

```c++
template<arithmetic First, arithmetic... Rest>
struct arithmetic_promotion {
    using type = decltype((First{} + ... + Rest{}));
};

template<arithmetic auto... val>
struct arithmetic_value_promotion {
    using type = decltype((... + val));
};

scalarf a = 1.1f;
vec2i vec{ 1,2 };
auto res = a * vec; // get vec2f

mat2f { 1, 2,
        2, 1 };
vec2i v1{ 1.2f, 2.5f };
auto res = mat2i * v1; // get ve2f

```

- ```constexpr```数学函数
```c++
constexpr float res1 = math::cos(45);
constexpr float res2 = math::sqrt(2);
```

- 类型转换
```c++
vec3f a{ 1,1f,2,2f,3.3f };
vec3i b = a.cast<int>(); // [1, 2, 3]
```

- ```operator+=, -=, *=```的```concpets```约束
```c++
template<typename Des, typename Src>
concept acceptable_loss = float_point_type<Des> || same_type<Des, Src>;

vec2i v1{ 1, 2 };
vec2f v2{ 1.1f, 2.2f };
vec2f v3{ 1.1, 2.2 };
scalarf a = 1.2f;
v1 *= a; // compiler-tiem error: int *= float too much precision loss 
v1 += v2; // same reason
v2 += v3; // no error: the precision loss is accpetable
```

- Transformation in Homogeneous Coordinates
```c++
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
```

## Example
```c++
#include <Euclid/Eucild.h>

using namespace euclid;

int main()
{
    vec2f a{ 1,2 };
    vec2f b{ 3,4 };
    a.cross(b).print();
    a.dot(b).print();
    constexpr vec3i v1{ 1, 2, 0 };
    constexpr vec4i v2{ 1, 1, 1, 0 };
    constexpr auto res1 = transform2d::Rotate<45> * v1;
    constexpr auto res2 = transform3d::Scale<1, 2, 3> * v2; // res2[1, 2, 3]
}
```
