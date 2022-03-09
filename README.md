# Eucild - a Graphics Mathematics Library
> **Euclid** is a SIMD Graphics Mathematics Library based on C++20

## Overview

**Features**
- Cross-compiler
- Compile-time calculation
- SIMD Speed up (not support ARM)
- Head-only

## Introduction
- Compile-time calculation (not support Clang)
  ```c++
  using namespace euclid;
  constexpr Vec4 a { 1,2,3,0 };
  constexpr Vec4 b { 4,5,6,0 };
  constexpr Vec4 res1 = a + b;
  constexpr Vec4 res2 = dot(a, b);
  constexpr Vec4 res3 = cross(a, b);
  ```
  
- `euclid::debug::print(...args)`
  ```c++
  #include <Euclid/Euclid.h>
  #include <Euclid/Print.hpp> // please include this file after <Euclid.h>

  using namespace euclid;

  Vec4 v{ 1,2,3,4 };
  Mat2 m{ 1,2,
          3,4 };
  debug::print(v, m);
  ```

- `Transformation` (WIP)

## Tested Compiler
- MSVC 19.29/19.30
- GCC 11.2
- Clang 13