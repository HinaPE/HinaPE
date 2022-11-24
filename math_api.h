#ifndef MATH_API_H
#define MATH_API_H

#include "math/vector2.h"
#include "math/vector3.h"
#include "math/vector4.h"
#include "math/matrix3x3.h"
#include "math/matrix4x4.h"
#include "math/quaternion.h"
#include "geometry/bounding_box3.h"

#include <iostream>
#include <iomanip>
#include <memory>

using real = float;
using mVector2 = HinaPE::Vector2<real>;
using mVector3 = HinaPE::Vector3<real>;
using mVector4 = HinaPE::Vector4<real>;
using mVector2i = HinaPE::Vector2<int>;
using mVector3i = HinaPE::Vector3<int>;
using mVector4i = HinaPE::Vector4<int>;
using mVector2ui = HinaPE::Vector2<unsigned int>;
using mVector3ui = HinaPE::Vector3<unsigned int>;
using mVector4ui = HinaPE::Vector4<unsigned int>;
using mMatrix3x3 = HinaPE::Matrix3x3<real>;
using mMatrix4x4 = HinaPE::Matrix4x4<real>;
using mQuaternion = HinaPE::Quaternion<real>;
using mBBox = HinaPE::BoundingBox<real, 3>;


#endif //MATH_API_H
