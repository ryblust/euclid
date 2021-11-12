# Eucild - a Graphics Mathematics Library
> Sorry's Graphics Mathematics Library

## Overview
**Eucild** is a Graphcis Mathematics Libray for the mathematices computation in Computer Graphics requires C++20 supported compiler

**Features** 
- Strictly differ `Vector`, `Matrix`, `Point` and `Scalar` in the compiler-time
- OOP
- Head-only
- Single inheritance
- SIMD speed up(To-do)

## Introdution
- Compile-time type constraints

```c++
template<typename T>
concept arithmetic = std::is_arithmetic_v<T>;
```

- Automatically arithmetic type promotion

```c++
scalarf a = 1.1f;
vec2i vec{ 1,2 };
auto res = a * vec; // get vec2f
```
- Type convertion
```c++
vec3f a{ 1,1f,2,2f,3.3f };
vec2i b = a.cast<int, 2>();
b.print(); // [1,2]
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
}
```
## To Do
1. ```euclid::point```
2. ```euclid::Matrix```
3. ```std::conjunction_v``` in ```Vector``` type deduction guide
4. compare 3 instructions vs 4 instructions in assembly
5. SIMD speed up
