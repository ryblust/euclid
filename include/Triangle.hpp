#pragma once
#ifndef _WEEKNDTRIANGLE_HPP
#define _WEEKNDTRIANGLE_HPP

#include "Point.hpp"

namespace euclid {

template<arithmetic Type>
class Triangle {
public:

public:
	point3<Type> vertex[3];
};

using trianglei = Triangle<int>;
using triangleu = Triangle<unsigned>;
using trianglef = Triangle<float>;
using triangled = Triangle<double>;

}

#endif // _WEEKNDTRIANGLE_HPP