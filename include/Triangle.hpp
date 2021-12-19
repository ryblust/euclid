#pragma once

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