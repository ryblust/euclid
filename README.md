# Eucild - a Graphics Mathematics Library
> Sorry's Graphics Mathematics Library

## Overview
**Eucild** is a Graphcis Mathematics Libray for the mathematices computation in Computer Graphics

**Features** 
- Strictly differ `Vector`, `Matrix`, `Point` and `Scalar` in the compiler-time
- OOP
- Head-only
- Single inheritance
- SIMD speed up(To-do)

## Introdution
- Compile-time type constraints by using C++20 concept

```c++
template<typename T>
concept arithmetic = std::is_arithmetic_v<T>;
```
## Example
```c++
#include <Euclid/Eucild.h>

using namespace sorry;

int main()
{
    vec2f a{ 1,2 };
    vec2f b{ 3,4 };
    a.cross(b).print();
    a.dot(b).print();
}
