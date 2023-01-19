# Eucild - constexpr SIMD Graphics Mathematics Library

## Features
- constexpr and SIMD
- C++20 Module

## Introduction
- Euclid uses **row-major-order** to store matrix's elements
- Euclid uses **col-major-order** to do matrix multiply
- Euclid requires C++20
- Euclid requires AVX2 support

- Compile-time calculation
  ```c++
  constexpr Euclid::Vec4 a{ 1,2,3,0 };
  constexpr Euclid::Vec4 b{ 4,5,6,0 };
  constexpr auto res1 = a + b;
  constexpr auto res2 = Euclid::Dot(a, b);
  constexpr auto res3 = Euclid::Cross(a, b);
  ```
  
- Swizzle
  ```c++
  constexpr Euclid::Vec4 a{ 1,2,3,0 };
  constexpr Euclid::Vec4 b{ 4,5,6,0 };
  constexpr auto res1 = Euclid::Permute<0,2,1,3>(a);
  // the first two template paramters
  // are used for a and rest is for b
  constexpr auto res2 = Euclid::Shuffle<0,2,1,3>(a, b);
  ```

- Transformation (WIP)