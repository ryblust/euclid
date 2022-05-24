# Eucild - a Graphics Mathematics Library
> **Euclid** is a C++20 SIMD Graphics Mathematics Library

## Overview

**Features**
- Compile-time calculation (not support Clang)
- SIMD Speed up

## Introduction
- Compile-time calculation
  ```c++
  using namespace euclid;
  constexpr Vec4 a { 1,2,3,0 };
  constexpr Vec4 b { 4,5,6,0 };
  constexpr Vec4 res1 = a + b;
  constexpr Vec4 res2 = dot(a, b);
  constexpr Vec4 res3 = cross(a, b);
  ```
  
- Print euclid components
  ```c++
  using namespace euclid;

  Vec4 v{ 1,2,3,4 };
  constexpr auto m = Mat4::identity();
  print(v, m);
  ```

- Transformation (WIP)

## Tested Compiler
- MSVC 19.29/19.30
- GCC 11.2
- Clang 13
