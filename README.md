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
- Compile-time type constraints by using C++20 concept

```c++
template<typename T>
concept arithmetic = std::is_arithmetic_v<T>;
```
- Type convertion in `Vector` 
```c++
	// implicitly
	vec3f a{ 1.1f,2.2f,3.3f };
	vec2i b = a;
	b.print(); // [1,2]
	
	// explicitly
	vec3f a{ 1,1f,2,2f,3.3f };
	vec2i b = a.cast<int, 2>();
	b.print(); // [1,2]
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
