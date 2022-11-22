# Eucild - constexpr SIMD Graphics Mathematics Library

**Features**
- Compile-time calculation
- C++20 Module support (WIP)
- SIMD speed up

## Introduction
- **Notice:** \
  Euclid uses **row-major-order** only to store matrix \
  Euclid uses **col-major-order** to do matrix transform
- Euclid requires C++20
- Euclid requires AVX2 support

- Compile-time calculation \
  **notice: clang only support for a few of functions**
  ```c++
  constexpr euclid::Vec4 a{ 1,2,3,0 };
  constexpr euclid::Vec4 b{ 4,5,6,0 };
  constexpr auto res1 = a + b;
  constexpr auto res2 = euclid::dot(a, b);
  constexpr auto res3 = euclid::cross(a, b);
  ```
  
- Swizzle
  ```c++
  constexpr euclid::Vec4 a{ 1,2,3,0 };
  constexpr euclid::Vec4 b{ 4,5,6,0 };
  constexpr auto res1 = euclid::permute<0,2,1,3>(a);
  // the first two template paramters
  // are used for a and rest is for b
  constexpr auto res2 = euclid::shuffle<0,2,1,3>(a, b);
  ```

- Transformation (WIP)

## Tested Compiler
- MSVC 19.29
- GCC 11.3
- Clang 13.0