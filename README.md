# Eucild - a Graphics Mathematics Library
> w11knd's Graphics Mathematics Library

## Overview
**Euclid** 是一个基于 C++20 的图形数学库，提供编译期的计算支持和严格的类型约束

**Features**
- 编译期的类型约束
- ```SIMD```(Working on)
- Head-only
- 齐次坐标下的```transformation```

## Introduction
- 编译期的类型约束 - using ```concept```
    
    Euclid 支持的算术类型类型有 ```int, unsigned int, float, double```
```c++
Vecotr<int, 3>; // OK
Point<float, 3>; // OK
Vector<char, 3>; // compile-time error
```
- ```.print()```
```c++
// 如要将数据打印到 Console，请先 #include <Euclid_io.h>
#include <Euclid_io.h>
#include <Euclid.h>

vec3i v1{ 1,2,0 };
v1.print();

point3f p1 { 1.2f, 2.3f, 1.f };
p1.print();

mat2i m1{ 1,  0,
          0, -1 };
m1.print();
```


- 自动算术类型提升
    
    Euclid 的类型提升规则（仅在需要类型提升的情况下）\
    函数参数仅有 ```int, float``` 则得到 ```float``` 的结果 \
    函数参数中有 ```double``` 则得到 ```double``` 的结果

```c++
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
// 返回类型依据 Euclid 类型提升规则
constexpr auto res1 = math::cos(45); // res1 : float
constexpr auto res2 = math::sin(35.f) // res2 : float
constexpr auto res3 = math::tan(45.2) // res3 : double
constexpr float res2 = math::sqrt(2);
```

- 类型转换
```c++
vec3f a{ 1,1f,2,2f,3.3f };
vec3i b = a.cast<int>(); // [1, 2, 3]
vec3i c = a; // Euclid 并不提供隐式的类型转换
```

- ```operator+=, *=, -=``` 的约束
```c++
vec2i v1{ 1, 2 };
vec2f v2{ 1.1f, 2.2f };
vec2f v3{ 1.1, 2.2 };
scalarf a = 1.2f;
v1 *= a; // compile-time error: int *= float unacceptable precision loss
v1 += v2; // same reason
v2 += v3; // no error: acceptable precision loss
```

- ```Transformation in Homogeneous Coordinates```
```c++
constexpr auto res = transform4d::scale(1, 2, 3); // res : mat4i 这里没有必要提升类型
constexpr vec4i axis{ 0,0,1,0 };
constexpr auto res1 = transform4d::rotate(axis, 45); // res1 : float
constexpr auto res2 = transform4d::translate(1, 2, 3); // res2 : mat4i same reason with res
constexpr auto res3 = transform4d::perspective(fov, aspectRatio, zNear, zFar); // 返回类型依据 Euclid 类型提升原则
```
## Tested Compiler
- MSVC 16.10/17.0
- GCC 11.2
