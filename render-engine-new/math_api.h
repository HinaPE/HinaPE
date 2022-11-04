#ifndef HINAPE_RENDER_ENGINE_PRO_MATH_API_H
#define HINAPE_RENDER_ENGINE_PRO_MATH_API_H

#include "math/vector2.h"
#include "math/vector3.h"
#include "math/vector4.h"
#include "math/matrix3x3.h"
#include "math/matrix4x4.h"
#include "math/quaternion.h"
#include "geometry/bounding_box3.h"

namespace HinaPE::RenderEngine
{
using real = float;
using mVector2 = Vector2<real>;
using mVector3 = Vector3<real>;
using mVector4 = Vector4<real>;
using mVector2i = Vector2<int>;
using mVector3i = Vector3<int>;
using mVector4i = Vector4<int>;
using mVector2ui = Vector2<unsigned int>;
using mVector3ui = Vector3<unsigned int>;
using mVector4ui = Vector4<unsigned int>;
using mMatrix3x3 = Matrix3x3<real>;
using mMatrix4x4 = Matrix4x4<real>;
using mBBox = BoundingBox3F;
}

#endif //HINAPE_RENDER_ENGINE_PRO_MATH_API_H
