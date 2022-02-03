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
- 编译期的计算支持 (暂时只支持 MSVC)
  
  Better with C++23 `(if consteval)`

  暂时使用 `__builtin_is_constant_evaluated()` 来判断是否是常量求值上下文

- 编译期的类型约束
  
- `euclid::debug::print(...args)`
  ```c++
  #include <Euclid/Euclid.h>
  #include <Euclid/Print.hpp> // 需要在 Euclid.h 之后 include

  using namespace euclid;

  vec4i v1 = setVec4i(1, 2, 3, 4);
  vec4f v2 = setVec4f(1, 2, 3, 4);
  mat2i m1 = { 1,2,3,4 };
  debug::print(v1, v2, m1);
  ```

- `constexpr` 数学函数
  ```c++
  using namespace euclid;
  constexpr float res1 = math::cos(45);
  constexpr float res2 = math::sin(35);
  constexpr float res4 = math::sqrt(2);
  ```

- 类型转换

  ```c++
  vec4i ivec = setVec4i(1, 2, 3, 4);
  vec4f fvec = castToVec4f(ivec);
  ```

- `Transformation in Homogeneous Coordinates`

## Tested Compiler
- MSVC 19.29/19.30
- GCC 11.2
- Clang 13.0