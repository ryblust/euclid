# Eucild - a Graphics Mathematics Library
> w11knd's Graphics Mathematics Library

## Overview
- **Euclid** 是一个基于 C++20 的图形数学库

**Features**

- 编译期的计算支持
- 编译期的类型约束
- `SIMD` Speed up (Requires AVX2 supported)
- Head-only

## Introduction
- 编译期的计算支持
  
  Better with C++23 `(if consteval)`

  暂时使用 `__builtin_is_constant_evaluated()` 来判断是否是常量求值上下文

- 编译期的类型约束
  
   使用 `concept`来约束某些操作
    
    **Euclid** 支持的算术类型类型有 `int, float`
  ```c++

  ```
- `euclid::debug::printVec(...args)`
  ```c++
  #include <Euclid/Euclid.h>
  #include <Euclid/Print.hpp>

  using namespace euclid;

  vec4i v1 = setVec4i(1, 2, 3, 4);
  vec4f v2 = setVec4f(1, 2, 3, 4);
  debug::printVec(v1, v2);

  ```

- `constexpr` 数学函数
  ```c++
  using namespace euclid;
  constexpr float res1 = math::cos(45);
  constexpr float res2 = math::sin(35.f)
  constexpr float res4 = math::sqrt(2);
  ```

- 类型转换

  ```c++
  vec4i ivec = setVec4i(1, 2, 3, 4);
  vec4f fvec = castVec4iTo4f(ivec); // 暂定名称
  ```

- `operator+=, *=, -=` 的约束
  ```c++
  vec2i v1{ 1, 2 };
  vec2f v2{ 1.1f, 2.2f };
  v1 *= 2.2f; // concept constraint failed : int *= float unacceptable precision loss
  v1 += v2; // same reason
  ```

- `Transformation in Homogeneous Coordinates`

## Tested Compiler
- MSVC 19.29/19.30
- GCC 11.2
- Clang 13.0