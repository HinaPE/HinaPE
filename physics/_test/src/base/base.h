#ifndef HINAPE_BASE_H
#define HINAPE_BASE_H

#include "size.h"
#include "array.h"
#include "vector.h"
#include "quaternion.h"
#include "matrix.h"
#include "bbox.h"

#include <memory>

namespace Hina::Base
{
using real = double;
static constexpr real Zero = static_cast<real>(0);
using mVector3 = Vector3<real>;
using mQuaternion = Quaternion<real>;
using mMatrix3x3 = Matrix3x3<real>;
using mBBox3 = BoundingBox3<real>;
}

#endif //HINAPE_BASE_H
