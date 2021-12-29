#pragma once

#include <immintrin.h>
#include "Common/Core.hpp"
#include "Common/Math.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4324 4820) // Ignore compiler's complain about padding bytes (Wall-level)
#endif // _MSC_VER

#include "Common/Array.hpp"
#include "Vector.hpp"
#include "Point.hpp"
#include "Matrix.hpp"
#include "Utility.hpp"

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

//#include "Transformation.hpp"
//#include "Triangle.hpp"