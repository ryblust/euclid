#pragma once

#include <immintrin.h>

#include "Common/Core.hpp"
#include "Common/Math.hpp"
#include "Common/Util.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4324 4820 4201) // struct alignment & anonymous union
#endif

#include "Common/Array.hpp"
#include "Vector.hpp"
#include "Point.hpp"
#include "Matrix2.hpp"
#include "Matrix3.hpp"
#include "Matrix4.hpp"

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

#include "Utility.hpp"

#include "Transformation.hpp"
#include "Triangle.hpp"